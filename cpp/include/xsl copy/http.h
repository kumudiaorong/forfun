#pragma once
#ifndef XSL_HTTP
#define XSL_HTTP
#include<xsl/map.hpp>
#include<xsl/string.hpp>
// #include<xsl/buffer.hpp>
#include<regex>
#include<zlib/zlib.h>
#include<openssl/ssl.h>
#include<openssl/err.h>
#ifdef _MSC_VER
#pragma comment (lib,"zlibstatic.lib")
#endif // _MSC_VER
#ifdef __GNUC__
#pragma comment (lib,"z")
#endif // _MSC_VER
#pragma comment (lib, "ssl")
#pragma comment (lib, "crypto")
#ifdef UNICODE
#define CSTR(str) L##str
#else
#define CSTR(str) str
#endif // UNICODE
//using string = basic_string<TCHAR>;
#ifdef _WIN32
#include<winsock2.h>
#include<ws2tcpip.h>
#pragma comment (lib, "Ws2_32")
#else  
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netdb.h>
#include<netinet/in.h>
#endif // _WIN32
XSL_BEGIN
using Buffer = buffer<char>;
using String = basic_string<char>;
using String_View = basic_string_view<char>;
using Map = map<String, String, functor::less<void>>;
using Multimap = multimap<String, String, functor::less<void>>;
const String CRLF = "/r/n";
class decompressor {
	z_stream Stream;
	bool Is_valid;

public:
	decompressor()
		:Stream(), Is_valid(inflateInit2(&Stream, MAX_WBITS + 32) == Z_OK) {}
	String decompress(const String_View& src) {
		String dest{ alloc_tag{},src.size() * 4 }, buf(alloc_tag{}, 1024 * 256);
		int err = Z_OK;
		Stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src.data()));
		Stream.avail_in = static_cast<uInt>(src.size());
		while (err != Z_STREAM_END) {
			Stream.next_out = reinterpret_cast<Bytef*>(buf.data());
			Stream.avail_out = static_cast<uInt>(buf.capacity());
			err = inflate(&Stream, Z_NO_FLUSH);
			if (err != Z_OK && err != Z_STREAM_END)
				throw "error zlib";
			dest.append(buf.data(), buf.capacity() - Stream.avail_out);
		}
		return as_rreference(dest);
	}
	constexpr bool is_valid() {
		return Is_valid;
	}
	~decompressor() {
		inflateEnd(&Stream);
	}
};
#ifdef _WIN32
class httpstartup {
public:
	WSADATA data;
	bool is_valid;
	httpstartup():is_valid(WSAStartup(0x0202, &data)) {
		// SSL_load_error_strings();
		// OpenSSL_add_all_algorithms();
	}
	~httpstartup() { if (!is_valid)WSACleanup(); }
};
static httpstartup httpsu{};
#endif
//protocol
//http 0
//https 1
inline String percent_encode(const String_View& url) {
	String result;
	result.reserve(url.size());
	for (auto c : url) {
		uint_8 tmp = c;
		if (tmp <= 0X20 || tmp >= 0X7F) {
			result += '%';
			char hex[4];
			auto len = snprintf(hex, sizeof(hex) - 1, "%02X", tmp);
			result.append(hex, len);
		}
		switch (c) {
		case '"': result += "%22"; break;
		case '%': result += "%25"; break;
		case '<': result += "%3C"; break;
		case '>': result += "%3E"; break;
		case '\\': result += "%5C"; break;
		case '^': result += "%5E"; break;
		case '`': result += "%60"; break;
		case '{': result += "%7B"; break;
		case '|': result += "%7C"; break;
		case '}': result += "%7D"; break;
		default:result += c;
		}
	}
	return result;
}
class authority {
public:
	static std::regex Re;
	static std::cmatch Cm;
	String Userinfo, Host, Port;
	constexpr authority() = default;
	authority(const String& auth) {
		*this = auth;
	}
	authority& operator=(const String& auth) {
		return *this = auth.data();
	}
	authority& operator=(const char* auth) {
		std::regex_search(auth, Cm, Re);
		if (Cm[1].matched)
			Userinfo.assign(Cm[1].first, Cm[1].second);
		if (Cm[2].matched)
			Host.assign(Cm[2].first, Cm[2].second);
		if (Cm[3].matched)
			Port.assign(Cm[3].first, Cm[3].second);
		return *this;
	}
};
std::regex authority::Re = std::regex(R"((?:([^@]+)@){0,1}([^:]+)(?::([0-9]+)){0,1})");
std::cmatch authority::Cm{};

constexpr String decode_Transfer_Encoding_chunked(const String_View& buf) {
	String tmp(alloc_tag{}, buf.size());
	size_t nstart = 0;
	while (buf[nstart] != '0') {
		auto l = strtol(&buf[nstart], nullptr, 16);
		size_t nend = buf.find("\r\n", nstart + 1);
		tmp.append(buf.data() + nend + 2, l);
		nstart = nend + 2 + l + 2;
	}
	// buf.clear();
	// buf << tmp;
	return as_rreference(tmp);
}
#ifdef _WIN32
class socket {
protected:
	ADDRINFOA Hints;
	SOCKET Socket;
	constexpr WSABUF to_wbuf(const char* buf, size_t len) {
		return { static_cast<uLong>(len),const_cast<char*>(buf) };
	}
public:
	socket():Hints(), Socket(INVALID_SOCKET) {};
	socket(const char* host, const char* port):socket() {
		connect(host, port);
	}
	virtual~socket() {
		closesocket(Socket);
	}
	virtual void connect(const char* host, const char* port) {
		ADDRINFOA* result{}, * ptr{};
		MSet(&Hints, sizeof(Hints), '\0');
		Hints.ai_family = AF_UNSPEC;
		Hints.ai_socktype = SOCK_STREAM;
		Hints.ai_protocol = IPPROTO_TCP;
		int ec = GetAddrInfoA(host, port, &Hints, &result);
		if (ec != 0)return;
		Socket = WSASocket(Hints.ai_family, Hints.ai_socktype, Hints.ai_protocol, nullptr, 0, WSA_FLAG_NO_HANDLE_INHERIT | WSA_FLAG_OVERLAPPED);
		if (Socket == INVALID_SOCKET)return;
		for (ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
			ec = ::connect(Socket, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
			if (ec == 0)
				break;
		}
		FreeAddrInfoA(result);
		if (ptr == nullptr)
			closesocket(Socket);
		else Hints = *ptr;
	}
	virtual void send(const Buffer& buf) {
		WSABUF wbuf = to_wbuf(buf.data(), buf.size());
		DWORD num{};
		if (WSASend(Socket, &wbuf, 1, &num, 0, nullptr, nullptr) != 0)
			throw GetLastError();
	}

	virtual bool recv_able(int_64 timeout) {
		WSAPOLLFD pollfd{ Socket,POLLIN };
		auto ret = WSAPoll(&pollfd, 1, timeout);
		if (ret >= 1) {
			if ((pollfd.revents & POLLIN) != 0)
				return true;
			else if ((pollfd.revents & POLLHUP) != 0)
				return false;
		}
		else if (ret == 0)
			return false;
		else {
			auto err = GetLastError();
			switch (err) {
			case WSAENETDOWN:throw "poll WSAENETDOWN";
			case WSAEFAULT:throw "poll WSAEFAULT";
			case WSAEINVAL:throw "poll WSAEINVAL";
			case WSAENOBUFS:throw "poll WSAENOBUFS";
			default:break;
			}
		}
		return false;
	}

	virtual void recv(Buffer& buf) {
		if (buf.full())
			buf.reserve(buf.capacity() << 1);
		WSABUF wbuf = to_wbuf(buf.data() + buf.size(), buf.capacity() - buf.size());
		DWORD num{}, flag{};
		if (WSARecv(Socket, &wbuf, 1, &num, &flag, nullptr, nullptr) != 0)
			throw "error recv";
		buf.resize(buf.size() + num);
	}
	virtual void shutdown() {
		::shutdown(Socket, SD_BOTH);
	}
	virtual void close() {
		::closesocket(Socket);
		Socket = INVALID_SOCKET;
	}
};
#else
class socket {
protected:

	addrinfo Hints;
	int sfd, eplfd;//-1
	// int epfds
	// constexpr WSABUF to_wbuf(const char* buf, size_t len) {
		// return { static_cast<uLong>(len),const_cast<char*>(buf) };
	// }
public:
	socket():Hints(), sfd(-1), eplfd(epoll_create1(0)) {};
	socket(const char* host, const char* port):socket() {
		connect(host, port);
	}
	int fd() {
		return sfd;
	}
	virtual ~socket() {
		if (sfd != -1)
			::close(sfd);
	}
	virtual void connect(const char* host, const char* port) {
		addrinfo* result{}, * ptr{};
		MSet(&Hints, sizeof(Hints), '\0');
		Hints.ai_family = AF_UNSPEC;
		Hints.ai_socktype = SOCK_STREAM;
		Hints.ai_protocol = IPPROTO_TCP;
		int ec = getaddrinfo(host, port, &Hints, &result);
		// int ec = GetAddrInfoA(host, port, &Hints, &result);
		if (ec != 0)throw "error";
		// Socket = WSASocket(Hints.ai_family, Hints.ai_socktype, Hints.ai_protocol, nullptr, 0, WSA_FLAG_NO_HANDLE_INHERIT | WSA_FLAG_OVERLAPPED);
		// if (Socket == INVALID_SOCKET)return;
		for (ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
			sfd = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (sfd == -1)
				continue;
			ec = ::connect(sfd, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
			if (ec == 0)
				break;
			::close(sfd);
		}
		freeaddrinfo(result);
		// FreeAddrInfoA(result);
		if (ptr == nullptr)throw "error";
		Hints = *ptr;
	}
	virtual void send(const String& buf) {
		if (write(sfd, buf.data(), buf.size()) != buf.size())
			throw "error";
	}
	virtual bool recv_able(int_64 timeout) {

		// WSAPOLLFD pollfd{ Socket,POLLIN };
		// auto ret = WSAPoll(&pollfd, 1, timeout);
		// if (ret >= 1) {
		// 	if ((pollfd.revents & POLLIN) != 0)
		// 		return true;
		// 	else if ((pollfd.revents & POLLHUP) != 0)
		// 		return false;
		// }
		// else if (ret == 0)
		// 	return false;
		// else {
		// 	auto err = GetLastError();
		// 	switch (err) {
		// 	case WSAENETDOWN:throw "poll WSAENETDOWN";
		// 	case WSAEFAULT:throw "poll WSAEFAULT";
		// 	case WSAEINVAL:throw "poll WSAEINVAL";
		// 	case WSAENOBUFS:throw "poll WSAENOBUFS";
		// 	default:break;
		// 	}
		// }
		// return false;
	}
	virtual void recv(String& buf) {
		// if (buf.full())
			// buf.reserve(buf.capacity() << 1);
		// WSABUF wbuf = to_wbuf(buf.data() + buf.size(), buf.capacity() - buf.size());
		// DWORD num{}, flag{};
		buf.resize(buf.capacity() - 1);
		ssize_t count = read(sfd, buf.data(), buf.size());
		if (count == -1)throw "error";
		buf.resize(count);
		// if (WSARecv(Socket, &wbuf, 1, &num, &flag, nullptr, nullptr) != 0)
			// throw "error recv";
		// buf.resize(buf.size() + num);
	}
	virtual void shutdown() {
		::shutdown(sfd, SHUT_RDWR);
	}
	virtual void close() {
		::close(sfd);
		sfd = -1;
	}
};
#endif
class ssocket:public socket {
	SSL_CTX* Ctx;
	SSL* Ssl;
public:
	using base_type = socket;
	ssocket():socket(), Ctx(SSL_CTX_new(TLS_client_method())) {
	};
	ssocket(const char* host, const char* port):ssocket() {
		connect(host, port);
	}
	~ssocket()override {
		SSL_CTX_free(Ctx);
		if (sfd != -1)
			::close(sfd);
	}
	void connect(const char* host, const char* port)override {
		this->base_type::connect(host, port);
		Ssl = SSL_new(Ctx);
		SSL_set_fd(Ssl, static_cast<int>(sfd));
		SSL_connect(Ssl);
	}
	void send(const String& buf) override {
		int count = SSL_write(Ssl, buf.data(), static_cast<int>(buf.size()));
	}
	void recv(String& buf) override {
		buf.resize(buf.capacity() - 1);
		int count = SSL_read(Ssl, buf.data(), buf.size());
		while (count <= 0) {
			int err = SSL_get_error(Ssl, count);
			switch (err) {
			case SSL_ERROR_ZERO_RETURN:
				return;
			case SSL_ERROR_WANT_READ:count = SSL_read(Ssl, buf.data() + buf.size(), static_cast<int>(buf.capacity() - buf.size()));
				break;
			default:
				throw "error ssl recv";
			}
		}
		buf.resize(count);
	}
	void shutdown()override {
		SSL_shutdown(Ssl);
		::shutdown(sfd, SHUT_RDWR);
	}
	void close()override {
		SSL_free(Ssl);
		::close(sfd);
		sfd = -1;
	}
};

namespace detail {
	class resp {};
	class request {};
	class http_request:public request {
		String Line;
		Map Header;
		String Body;
		http_request() = default;
		void set_line(const char* method, const char* url) {
			Line = method;
			Line += ' ';
			Line += url;
			Line += ' ';
			Line += " HTTP/1.1\r\n";
		}
		String assemble() {
			Header.try_emplace("Connection", "close");
			//Header.try_emplace("Host",Host + ':' + Port);
			Header.try_emplace("Accept", "*/* ");
			Header.try_emplace("User-Agent", "xsl");
			String all = Line;
			for (auto& kv : Header)
				all += kv.first + ':' + kv.second + "\r\n";
			all += "\r\n" + Body;
			return as_rreference(all);
		}
	};
	void assemble_http_request_line(const char* method, const char* url, String& buf) {
		buf = xsl::as_rreference(xsl::as_rreference(buf) + method + ' ' + url + " HTTP/1.1\r\n");
	}
	void assemble_http_request_header(const Map& header, String& buf) {
		for (auto& kv : header)
			buf += kv.first + ':' + kv.second + "\r\n";
		buf.append("\r\n", 2);
	}
	void assemble_http_request_body(const String& content, String& buf) {
		buf.append(content) += "\r\n";
	}
	void assemble_http_request_body(const char* content, size_t count, Buffer& buf) {
		buf.write(content, count) << "\r\n";
	}
	class http_resp:public resp {
		uint_8 Sign;
		//line-0,header-1,body-2,ok-3
	public:
		String Version, Description, Body;
		uint_16 Status;
		Multimap Header;
		constexpr http_resp()
			:Body(alloc_tag{}, 1024 * 8) {}
		constexpr http_resp(http_resp&& ano) = default;
		size_t analysis_line(const String_View& buf) {
			static const std::regex re(R"((HTTP/1\.[01]) ([0-9]{3})(?: (.*?)){0,1}\r\n)");
			std::cmatch cm{};
			if (std::regex_search(buf.data(), cm, re)) {
				Sign = 1;
				Version.assign(cm[1].first, cm[1].second);
				Status = std::stoi(cm[2]);
				Description.assign(cm[3].first, cm[3].second);
				return cm.suffix().first - buf.data() + 2;
			}
			return 0;
		}
		size_t analysis_header(const String_View& buf) {
			size_t kstart{};//CRLF
			while (buf[kstart] != '\r') {
				size_t kend = buf.find(':', kstart + 1);
				size_t vstart = buf.find_first_not_of(' ', kend + 1);
				size_t vend = buf.find("\r\n", vstart);
				Header.emplace(String{ buf.data() + kstart,kend - kstart }, buf.data() + vstart, vend - vstart);
				kstart = vend + 2;
			}
			return kstart + 2;
			// buf.flush(kstart + 2);
		}
		String decode(const String_View& buf) {
			auto iter = Header.find(String_View{ "Content-Encoding" });
			if (iter != Header.end()) {
				if (iter->second == "gzip" || iter->second == "deflate") {
					decompressor de{};
					if (!de.is_valid())
						throw "error decompress";
					return as_rreference(de.decompress(buf));
				}
			}
		}
		String analysis_body(const String_View& buf) {
			auto iter = Header.find(String_View{ "Transfer-Encoding" });
			if (iter != Header.end() && iter->second == "chunked") {
				String tmp = decode_Transfer_Encoding_chunked(buf);
				return as_rreference(decode(tmp));
			}
			// else {
			// 	iter = Header.find(String_View{ "Content-Length" });
			// 	if (iter != Header.end()) {
			// 		buf.resize(strtol(iter->second.data(), nullptr, 10));
			// 		decode(buf);
			// 	}
			// }
			// Body.assign(buf.data(), buf.size());
		}
		size_t analysis(String_View sv) {
			size_t start{};
			switch (Sign) {
			case 0: {
				//start = analysis_line(sv);
				if (Sign == 0)
					return start;
				sv.remove_prefix(start);
			}
			case 1: {
				//auto [start,ok] = analysis_header(sv);
				if (Sign == 1)
					return start;
				sv.remove_prefix(start);
			}
			case 2: {
				//start = analysis_header(sv);
				if (Sign == 1)
					return start;
				sv.remove_prefix(start);
			}
			default:break;
			}
		}
	};
	class scheme {};
	class http_client:public scheme {
		String Host, Port;
		map<int,socket> sfds;
		bool is_ssl;
		bool proxy;
		//ADDRINFOT
	public:
		http_client(const char* shp):sfds() {
			static std::regex re(R"((?:([a-z]+)://)([^:]+)(?::([0-9]+)){0,1})");
			std::cmatch cm{};
			std::regex_search(shp, cm, re);
			if (shp[4] == 's')
				is_ssl = true;
			Host.assign(cm[2].first, cm[2].second);
			if (cm[3].matched)
				Port.assign(cm[3].first, cm[3].second);
			else if (is_ssl) {
				Port.assign("443", 3);
			}
			else {
				Port.assign("80", 2);
			}
		}
		//Scheme,host,port
		http_resp get(const char* path, bool ipe = false/*is percent encoding*/) {
			sfds.try_emplace()
			psfd->connect(Host.data(), Port.data());
			String tmp = (is_ssl ? "https://" : "http://") + Host + ':' + Port + '/' + (ipe ? path : percent_encode(path));
			String buf{ xsl::alloc_tag{},1024 * 8 };
			assemble_http_request_line("GET", tmp.data(), buf);
			Map header{};
			header.try_emplace("Connection", "close");
			header.try_emplace("Host", Host + ':' + Port);
			header.try_emplace("Accept", "*/* ");
			header.try_emplace("Accept-Encoding", "gzip,deflate");
			header.try_emplace("User-Agent", "xsl");
			assemble_http_request_header(header, buf);
			psfd->send(buf);
			buf.clear();
			http_resp resp{};
			while (psfd->recv_able(500))
				psfd->recv(buf);

			String_View sv{ buf };
			size_t tail = resp.analysis_line(sv);
			sv.remove_prefix(tail);
			tail = resp.analysis_header(sv);
			sv.remove_prefix(tail);
			resp.analysis_body(sv);
			auto iter = resp.Header.find(String_View{ "Connection" });
			if (iter->second == "close") {
				psfd->shutdown();
				psfd->close();
			}
			return as_rreference(resp);
		}
		http_resp post(const char* path/*is percent encoding*/, Map& hdr, const char* content, size_t count, bool ipe = false, bool ifr = true) {
			psfd->connect(Host.data(), Port.data());
			String tmp = (is_ssl ? "https://" : "http://") + Host + ':' + Port + '/' + (ipe ? path : percent_encode(path));
			String buf{ alloc_tag{},1024 * 8 };
			assemble_http_request_line("POST", tmp.data(), buf);
			Map header = hdr;
			header.try_emplace("Connection", "close");
			header.try_emplace("Host", Host + ':' + Port);
			header.try_emplace("Accept", "*/* ");
			header.try_emplace("Accept-Encoding", "gzip,deflate");
			header.try_emplace("User-Agent", "xsl");
			assemble_http_request_header(header, buf);
			assemble_http_request_body(content, buf);
			psfd->send(buf);
			http_resp resp{};
			if (ifr) {
				buf.clear();
				while (psfd->recv_able(500))
					psfd->recv(buf);
				resp.analysis_line(buf);
				resp.analysis_header(buf);
				resp.analysis_body(buf);
				auto iter = resp.Header.find(String_View{ "Connection" });
				if (iter->second == "close") {
					psfd->shutdown();
					psfd->close();
				}
			}
			return as_rreference(resp);
		}
		void close() {
			psfd->shutdown();
			psfd->close();
		}
	};
}

XSL_END
#endif // !XSL_HTTP
