#include <unistd.h>

#include "xsl/bits/pf/def.h"
#include "xsl/bits/xsl_container.hpp"
#include "xsl/bits/xsl_tuple.hpp"
#include "xsl/bits/xsl_utility.hpp"
#define XSL_NET_ZLIB_SUPPORT

#pragma once
#ifndef XSL_NET
#define XSL_NET
#include <openssl/err.h>
#include <openssl/ssl.h>

#ifdef XSL_NET_ZLIB_SUPPORT
#include <zlib/zlib.h>
#endif

#include <ranges>
#include <regex>
#include <xsl/functional.hpp>
#include <xsl/map.hpp>
#include <xsl/set.hpp>
#include <xsl/string.hpp>
#include <xsl/vector.hpp>
#ifdef _MSC_VER
#pragma comment(lib, "zlibstatic.lib")
#endif  // _MSC_VER
// #ifdef __GNUC__
// #pragma comment(lib, "z")
// #endif  // _MSC_VER
// #pragma comment(lib, "ssl")
// #pragma comment(lib, "crypto")
#ifdef UNICODE
#define CSTR(str) L##str
#else
#define CSTR(str) str
#endif  // UNICODE
// using string = basic_string<TCHAR>;
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32")
#else
#include <netdb.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif  // _WIN32
XSL_BEGIN
template <class T>
using Vec = xsl::vector<T>;
using String = xsl::basic_string<char>;
using String_View = xsl::basic_string_view<char>;
using MS2S = xsl::map<String, String, xsl::functor::less<void>>;
using MI2S = xsl::map<int, String>;
using Multimap = xsl::multimap<String, String, xsl::functor::less<void>>;
template <class F>
using Func = xsl::functional::function<F>;
class decompressor {
  z_stream Stream;
  bool Is_valid;
public:
  decompressor()
    : Stream()
    , Is_valid(inflateInit2(&Stream, MAX_WBITS + 32) == Z_OK) {}
  String decompress(const String_View& src) {
    String dest{xsl::tag_alloc{}, src.size() * 4}, buf(xsl::tag_alloc{}, 1024 * 256);
    int err = Z_OK;
    Stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(src.data()));
    Stream.avail_in = static_cast<uInt>(src.size());
    while(err != Z_STREAM_END) {
      Stream.next_out = reinterpret_cast<Bytef *>(buf.data());
      Stream.avail_out = static_cast<uInt>(buf.capacity());
      err = inflate(&Stream, Z_NO_FLUSH);
      if(err != Z_OK && err != Z_STREAM_END) throw "error zlib";
      dest.append(buf.data(), buf.capacity() - Stream.avail_out);
    }
    return as_rreference(dest);
  }
  constexpr bool is_valid() { return Is_valid; }
  ~decompressor() { inflateEnd(&Stream); }
};
#ifdef _WIN32
class httpstartup {
public:
  WSADATA data;
  bool is_valid;
  httpstartup()
    : is_valid(WSAStartup(0x0202, &data)) {
    // SSL_load_error_strings();
    // OpenSSL_add_all_algorithms();
  }
  ~httpstartup() {
    if(!is_valid) WSACleanup();
  }
};
static httpstartup httpsu{};
#endif
// protocol
// http 0
// https 1
inline String percent_encode(const String_View& url) {
  String result;
  result.reserve(url.size());
  for(auto c : url) {
    uint_8 tmp = c;
    if(tmp <= 0X20 || tmp >= 0X7F) {
      result += '%';
      char hex[4];
      auto len = snprintf(hex, sizeof(hex) - 1, "%02X", tmp);
      result.append(hex, len);
    }
    switch(c) {
      case '"' :
        result += "%22";
        break;
      case '%' :
        result += "%25";
        break;
      case '<' :
        result += "%3C";
        break;
      case '>' :
        result += "%3E";
        break;
      case '\\' :
        result += "%5C";
        break;
      case '^' :
        result += "%5E";
        break;
      case '`' :
        result += "%60";
        break;
      case '{' :
        result += "%7B";
        break;
      case '|' :
        result += "%7C";
        break;
      case '}' :
        result += "%7D";
        break;
      default :
        result += c;
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
  authority(const String& auth) { *this = auth; }
  authority& operator=(const String& auth) { return *this = auth.data(); }
  authority& operator=(const char *auth) {
    std::regex_search(auth, Cm, Re);
    if(Cm[1].matched) Userinfo.assign(Cm[1].first, Cm[1].second);
    if(Cm[2].matched) Host.assign(Cm[2].first, Cm[2].second);
    if(Cm[3].matched) Port.assign(Cm[3].first, Cm[3].second);
    return *this;
  }
};

std::regex authority::Re{R"((?:([^@]+)@){0,1}([^:]+)(?::([0-9]+)){0,1})"};
std::cmatch authority::Cm{};

constexpr String decode_Transfer_Encoding_chunked(const String_View& buf) {
  String tmp(xsl::tag_alloc{}, buf.size());
  size_t nstart = 0;
  while(buf[nstart] != '0') {
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
  constexpr WSABUF to_wbuf(const char *buf, size_t len) { return {static_cast<uLong>(len), const_cast<char *>(buf)}; }
public:
  socket()
    : Hints()
    , Socket(INVALID_SOCKET){};
  socket(const char *host, const char *port)
    : socket() {
    connect(host, port);
  }
  virtual ~socket() { closesocket(Socket); }
  virtual void connect(const char *host, const char *port) {
    ADDRINFOA *result{}, *ptr{};
    MSet(&Hints, sizeof(Hints), '\0');
    Hints.ai_family = AF_UNSPEC;
    Hints.ai_socktype = SOCK_STREAM;
    Hints.ai_protocol = IPPROTO_TCP;
    int ec = GetAddrInfoA(host, port, &Hints, &result);
    if(ec != 0) return;
    Socket = WSASocket(Hints.ai_family, Hints.ai_socktype, Hints.ai_protocol, nullptr, 0,
      WSA_FLAG_NO_HANDLE_INHERIT | WSA_FLAG_OVERLAPPED);
    if(Socket == INVALID_SOCKET) return;
    for(ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
      ec = ::connect(Socket, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
      if(ec == 0) break;
    }
    FreeAddrInfoA(result);
    if(ptr == nullptr)
      closesocket(Socket);
    else
      Hints = *ptr;
  }
  virtual void send(const Buffer& buf) {
    WSABUF wbuf = to_wbuf(buf.data(), buf.size());
    DWORD num{};
    if(WSASend(Socket, &wbuf, 1, &num, 0, nullptr, nullptr) != 0) throw GetLastError();
  }

  virtual bool recv_able(int_64 timeout) {
    WSAPOLLFD pollfd{Socket, POLLIN};
    auto ret = WSAPoll(&pollfd, 1, timeout);
    if(ret >= 1) {
      if((pollfd.revents & POLLIN) != 0)
        return true;
      else if((pollfd.revents & POLLHUP) != 0)
        return false;
    } else if(ret == 0)
      return false;
    else {
      auto err = GetLastError();
      switch(err) {
        case WSAENETDOWN :
          throw "poll WSAENETDOWN";
        case WSAEFAULT :
          throw "poll WSAEFAULT";
        case WSAEINVAL :
          throw "poll WSAEINVAL";
        case WSAENOBUFS :
          throw "poll WSAENOBUFS";
        default :
          break;
      }
    }
    return false;
  }

  virtual void recv(Buffer& buf) {
    if(buf.full()) buf.reserve(buf.capacity() << 1);
    WSABUF wbuf = to_wbuf(buf.data() + buf.size(), buf.capacity() - buf.size());
    DWORD num{}, flag{};
    if(WSARecv(Socket, &wbuf, 1, &num, &flag, nullptr, nullptr) != 0) throw "error recv";
    buf.resize(buf.size() + num);
  }
  virtual void shutdown() { ::shutdown(Socket, SD_BOTH); }
  virtual void close() {
    ::closesocket(Socket);
    Socket = INVALID_SOCKET;
  }
};
#else
class socket {
protected:
  addrinfo Hints;
  int sfd;  //-1
            // int epfds
            // constexpr WSABUF to_wbuf(const char* buf, size_t len) {
  // return { static_cast<uLong>(len),const_cast<char*>(buf) };
  // }
public:
  socket()
    : Hints()
    , sfd(-1){};
  socket(const char *host, const char *port)
    : socket() {
    connect(host, port);
  }
  virtual ~socket() {
    if(sfd != -1) ::close(sfd);
  }
  virtual int fd() { return sfd; }
  virtual bool invalid() { return sfd == -1; }
  virtual void connect(const char *host, const char *port) {
    addrinfo *result{}, *ptr{};
    MSet(&Hints, sizeof(Hints), '\0');
    Hints.ai_family = AF_UNSPEC;
    Hints.ai_socktype = SOCK_STREAM;
    Hints.ai_protocol = IPPROTO_TCP;
    int ec = getaddrinfo(host, port, &Hints, &result);
    // int ec = GetAddrInfoA(host, port, &Hints, &result);
    if(ec != 0) throw "error";
    // Socket = WSASocket(Hints.ai_family, Hints.ai_socktype, Hints.ai_protocol,
    // nullptr, 0, WSA_FLAG_NO_HANDLE_INHERIT | WSA_FLAG_OVERLAPPED);
    // if (Socket == INVALID_SOCKET)return;
    for(ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
      sfd = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
      if(sfd == -1) continue;
      ec = ::connect(sfd, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
      if(ec == 0) break;
      ::close(sfd);
    }
    freeaddrinfo(result);
    // FreeAddrInfoA(result);
    if(ptr == nullptr) throw "error";
    Hints = *ptr;
  }
  virtual void send(const String& buf) {
    if(write(sfd, buf.data(), buf.size()) != buf.size()) throw "error";
  }
  virtual bool recv_able(int_64 timeout) { return false; }
  virtual void recv(String& r) {
    String buf{tag_alloc{}, 4096};
    buf.resize(buf.capacity() - 1);
    ssize_t count = read(sfd, buf.data(), buf.size());
    if(count == -1) throw "error";
    buf.resize(count);
  }
  virtual void shutdown() { ::shutdown(sfd, SHUT_RDWR); }
  virtual void close() {
    ::close(sfd);
    sfd = -1;
  }
};
#endif
class ssocket : public socket {
  // 	SSL_CTX* Ctx;
  // 	SSL* Ssl;
  // public:
  // 	using base_type = socket;
  // 	ssocket():socket(), Ctx(SSL_CTX_new(TLS_client_method())) {
  // 	};
  // 	ssocket(const char* host, const char* port):ssocket() {
  // 		connect(host, port);
  // 	}
  // 	~ssocket()override {
  // 		SSL_CTX_free(Ctx);
  // 		if (sfd != -1)
  // 			::close(sfd);
  // 	}
  // 	void connect(const char* host, const char* port)override {
  // 		this->base_type::connect(host, port);
  // 		Ssl = SSL_new(Ctx);
  // 		SSL_set_fd(Ssl, static_cast<int>(sfd));
  // 		SSL_connect(Ssl);
  // 	}
  // 	void send(const String& buf) override {
  // 		int count = SSL_write(Ssl, buf.data(), static_cast<int>(buf.size()));
  // 	}
  // 	void recv(String& buf) override {
  // 		buf.resize(buf.capacity() - 1);
  // 		int count = SSL_read(Ssl, buf.data(), buf.size());
  // 		while (count <= 0) {
  // 			int err = SSL_get_error(Ssl, count);
  // 			switch (err) {
  // 			case SSL_ERROR_ZERO_RETURN:
  // 				return;
  // 			case SSL_ERROR_WANT_READ:count = SSL_read(Ssl, buf.data() + buf.size(),
  // static_cast<int>(buf.capacity() - buf.size())); 				break;
  // default: 				throw "error ssl recv";
  // 			}
  // 		}
  // 		buf.resize(count);
  // 	}
  // 	void shutdown()override {
  // 		SSL_shutdown(Ssl);
  // 		::shutdown(sfd, SHUT_RDWR);
  // 	}
  // 	void close()override {
  // 		SSL_free(Ssl);
  // 		::close(sfd);
  // 		sfd = -1;
  // 	}
};
#ifdef XSL_NET_ZLIB_SUPPORT
#define XSL_NET_ZLIB_EXIST(sta) sta
#else
#define XSL_NET_ZLIB_EXIST(sta)
#endif
#define XSL_NET_ACCEPT_ENCODING "" XSL_NET_ZLIB_EXIST("gzip,deflate")

namespace http {
  int connect(const char *host, const char *port) {
    int fd{};
    addrinfo hints{}, *result{}, *ptr{};
    // MSet(&hints, sizeof(hints), '\0');
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    int ec = getaddrinfo(host, port, &hints, &result);
    if(ec != 0) throw "error";
    for(ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
      fd = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
      if(fd == -1) continue;
      ec = ::connect(fd, ptr->ai_addr, ptr->ai_addrlen);
      if(ec == 0) break;
      ::close(fd);
    }
    freeaddrinfo(result);
    if(ptr == nullptr) throw "error";
    return fd;
  }
  void send(int fd, const String& buf) {
    if(::send(fd, buf.data(), buf.size(), 0) != buf.size()) throw "error";
  }
  static size_t once_recv_size = 2048;
  String recv(int fd) {
    String buf{once_recv_size}, ret{};
    int_32 n = ::read(fd, buf.data(), once_recv_size);
    while(n > 0) {
      std::cout << n << '\n';
      ret.append(buf, 0, n);
      n = read(fd, buf.data(), once_recv_size);
    }
    if(n == -1) {
      int a = errno;
      std::cout << errno;
      throw "error";
    }
    return xsl::as_rreference(ret);
  }
  void close(int fd) { ::close(fd); }
  void add_default_http_header(MS2S& header) {
    header.try_emplace("Connection", "close");
    header.try_emplace("Accept", "*/* ");
    header.try_emplace("Accept-Encoding", XSL_NET_ACCEPT_ENCODING);
    header.try_emplace("User-Agent", "xsl");
  }

  namespace detail {

    class IOmultiplexing {
    protected:
      int Fd;
      constexpr IOmultiplexing(int fd)
        : Fd(fd) {}
    public:
    };
    enum IOM_EVENTS {
      IN = 0x001,
      PRI = 0x002,
      OUT = 0x004,
      RDNORM = 0x040,
      RDBAND = 0x080,
      WRNORM = 0x100,
      WRBAND = 0x200,
      MSG = 0x400,
      ERR = 0x008,
      HUP = 0x010,
      RDHUP = 0x2000,
      EXCLUSIVE = 0x1 << 28,
      WAKEUP = 0x1 << 29,
      ONESHOT = 0x1 << 30,
      ET = 0x1 << 31
    };
    constexpr static xsl::tuple<IOM_EVENTS, EPOLL_EVENTS> all_event[15] = {
      {       IOM_EVENTS::IN,        EPOLL_EVENTS::EPOLLIN},
      {      IOM_EVENTS::PRI,       EPOLL_EVENTS::EPOLLPRI},
      {      IOM_EVENTS::OUT,       EPOLL_EVENTS::EPOLLOUT},
      {   IOM_EVENTS::RDNORM,    EPOLL_EVENTS::EPOLLRDNORM},
      {   IOM_EVENTS::RDBAND,    EPOLL_EVENTS::EPOLLRDBAND},
      {   IOM_EVENTS::WRNORM,    EPOLL_EVENTS::EPOLLWRNORM},
      {   IOM_EVENTS::WRBAND,    EPOLL_EVENTS::EPOLLWRBAND},
      {      IOM_EVENTS::MSG,       EPOLL_EVENTS::EPOLLMSG},
      {      IOM_EVENTS::ERR,       EPOLL_EVENTS::EPOLLERR},
      {      IOM_EVENTS::HUP,       EPOLL_EVENTS::EPOLLHUP},
      {    IOM_EVENTS::RDHUP,     EPOLL_EVENTS::EPOLLRDHUP},
      {IOM_EVENTS::EXCLUSIVE, EPOLL_EVENTS::EPOLLEXCLUSIVE},
      {   IOM_EVENTS::WAKEUP,    EPOLL_EVENTS::EPOLLWAKEUP},
      {  IOM_EVENTS::ONESHOT,   EPOLL_EVENTS::EPOLLONESHOT},
      {       IOM_EVENTS::ET,        EPOLL_EVENTS::EPOLLET},
    };
    // 0:IOM_EVENTS 1:EPOLL_EVENTS
    template <uint_8 To>
    constexpr uint_32 event_convert0(uint_32 e) {
      uint_32 te{};
      if((e & IOM_EVENTS::IN) > 0) te |= all_event[0].get<To>();
      if((e & IOM_EVENTS::PRI) > 0) te |= all_event[1].get<To>();
      if((e & IOM_EVENTS::OUT) > 0) te |= all_event[2].get<To>();
      if((e & IOM_EVENTS::RDNORM) > 0) te |= all_event[3].get<To>();
      if((e & IOM_EVENTS::RDBAND) > 0) te |= all_event[4].get<To>();
      if((e & IOM_EVENTS::WRNORM) > 0) te |= all_event[5].get<To>();
      if((e & IOM_EVENTS::WRBAND) > 0) te |= all_event[6].get<To>();
      if((e & IOM_EVENTS::MSG) > 0) te |= all_event[7].get<To>();
      if((e & IOM_EVENTS::ERR) > 0) te |= all_event[8].get<To>();
      if((e & IOM_EVENTS::HUP) > 0) te |= all_event[9].get<To>();
      if((e & IOM_EVENTS::RDHUP) > 0) te |= all_event[10].get<To>();
      if((e & IOM_EVENTS::EXCLUSIVE) > 0) te |= all_event[11].get<To>();
      if((e & IOM_EVENTS::WAKEUP) > 0) te |= all_event[12].get<To>();
      if((e & IOM_EVENTS::ONESHOT) > 0) te |= all_event[13].get<To>();
      if((e & IOM_EVENTS::ET) > 0) te |= all_event[14].get<To>();
      return te;
    }
    template <uint_8 To>
    constexpr uint_32 event_convert1(uint_32 e) {
      uint_32 te{};
      if((e & EPOLL_EVENTS::EPOLLIN) > 0) te |= all_event[0].get<To>();
      if((e & EPOLL_EVENTS::EPOLLPRI) > 0) te |= all_event[1].get<To>();
      if((e & EPOLL_EVENTS::EPOLLOUT) > 0) te |= all_event[2].get<To>();
      if((e & EPOLL_EVENTS::EPOLLRDNORM) > 0) te |= all_event[3].get<To>();
      if((e & EPOLL_EVENTS::EPOLLRDBAND) > 0) te |= all_event[4].get<To>();
      if((e & EPOLL_EVENTS::EPOLLWRNORM) > 0) te |= all_event[5].get<To>();
      if((e & EPOLL_EVENTS::EPOLLWRBAND) > 0) te |= all_event[6].get<To>();
      if((e & EPOLL_EVENTS::EPOLLMSG) > 0) te |= all_event[7].get<To>();
      if((e & EPOLL_EVENTS::EPOLLERR) > 0) te |= all_event[8].get<To>();
      if((e & EPOLL_EVENTS::EPOLLHUP) > 0) te |= all_event[9].get<To>();
      if((e & EPOLL_EVENTS::EPOLLRDHUP) > 0) te |= all_event[10].get<To>();
      if((e & EPOLL_EVENTS::EPOLLEXCLUSIVE) > 0) te |= all_event[11].get<To>();
      if((e & EPOLL_EVENTS::EPOLLWAKEUP) > 0) te |= all_event[12].get<To>();
      if((e & EPOLL_EVENTS::EPOLLONESHOT) > 0) te |= all_event[13].get<To>();
      if((e & EPOLL_EVENTS::EPOLLET) > 0) te |= all_event[14].get<To>();
      return te;
    }
    class epoll : public IOmultiplexing {
    public:
      epoll()
        : IOmultiplexing(epoll_create1(0)) {
        if(Fd == -1) throw "epoll_create1";
      }
      void add(int lfd, uint_32 events) {
        epoll_event ev{};
        ev.events = event_convert0<1>(events) | EPOLL_EVENTS::EPOLLET;
        ev.data.fd = lfd;
        int ec = epoll_ctl(Fd, EPOLL_CTL_ADD, lfd, &ev);
        if(ec == -1) throw "epoll_ctl";
      }
      void refresh(vector<epoll_event>& ready, int timeout) {
        int n = epoll_pwait(Fd, ready.data(), ready.size(), timeout, NULL);
        if(n == -1) throw "epoll_wait";
        ready.resize(n);
      }
    };
  };  // namespace detail
  void assemble_request_line(const char *method, String_View url, String& buf) {
    buf = xsl::as_rreference(xsl::as_rreference(buf) + method + ' ' + url + " HTTP/1.1\r\n");
  }
  void assemble_request_header(const MS2S& header, String& buf) {
    for(auto& kv : header)
      buf += kv.first + ':' + kv.second + "\r\n";
    buf.append("\r\n", 2);
  }
  void assemble_request_body(const String& content, String& buf) { buf.append(content) += "\r\n"; }
  void assemble_request_body(const char *content, size_t count, String& buf) {
    buf.append(content, count).append("\r\n", 2);
  }
  namespace Re {
    const static std::regex shp(R"((?:([a-z]+)://)([^:]+)(?::([0-9]+)){0,1})");
    const static std::regex rpl(R"((HTTP/1\.[01]) ([0-9]{3})(?: (.*?)){0,1}\r\n)");
    const static std::regex url("(?:([a-z]+)://)([^:/]+)(?::([0-9]+)){0,1}(.*)");

  }  // namespace Re
  class response {
    // line-0,header-1,body-2,ok-3
  public:
    String Version, Description, Body;
    uint_16 Status;
    Multimap Header;
    constexpr response()
      : Body(xsl::tag_alloc{}, 1024 * 8) {}
    constexpr response(String&& str)
      : Body(as_rreference(str)) {
      String_View sv = Body;
      size_t tail = analysis_line(sv);
      sv.remove_prefix(tail);
      tail = analysis_header(sv);
      sv.remove_prefix(tail);
      Body = analysis_body(sv);
    }
    constexpr response(response&& ano) = default;
    size_t analysis_line(String_View& buf) {
      std::cmatch cm{};
      if(std::regex_search(buf.data(), cm, Re::rpl)) {
        Version.assign(cm[1].first, cm[1].second);
        Status = std::stoi(cm[2]);
        Description.assign(cm[3].first, cm[3].second);
        return cm.suffix().first - buf.data() + 2;
      }
      return 0;
    }
    size_t analysis_header(String_View& buf) {
      size_t kstart{};  // CRLF
      while(buf[kstart] != '\r') {
        auto str = (const char(*)[10])(const char *)buf.data();
        size_t kend = buf.find(':', kstart + 1);
        size_t vstart = buf.find_first_not_of(' ', kend + 1);
        size_t vend = buf.find("\r\n", vstart);
        Header.emplace(String{buf.data() + kstart, kend - kstart}, buf.data() + vstart, vend - vstart);
        kstart = vend + 2;
      }
      return kstart + 2;
    }
    String decode(const String_View& buf) {
      auto iter = Header.find(String_View{"Content-Encoding"});
      if(iter != Header.end()) {
        if(iter->second == "gzip" || iter->second == "deflate") {
          decompressor de{};
          if(!de.is_valid()) throw "error decompress";
          return as_rreference(de.decompress(buf));
        }
      }
      return buf;
    }
    String analysis_body(const String_View& buf) {
      auto iter = Header.find(String_View{"Transfer-Encoding"});
      if(iter != Header.end() && iter->second == "chunked") {
        String tmp = decode_Transfer_Encoding_chunked(buf);
        return as_rreference(decode(tmp));
      } else {
        iter = Header.find(String_View{"Content-Length"});
        if(iter != Header.end()) {
          return as_rreference(decode(buf.substr(0, (strtol(iter->second.data(), nullptr, 10)))));
        }
      }
      return buf;
    }
  };
  xsl::tuple<String_View, String_View, String_View, String_View> analysis_url(const char *url) {
    std::cmatch cm{};
    std::regex_search(url, cm, Re::url);
    xsl::tuple<String_View, String_View, String_View, String_View> shp{};
    shp.get<0>().assign(cm[1].first, cm[1].length());
    shp.get<1>().assign(cm[2].first, cm[2].length());
    if(cm[3].matched) shp.get<2>().assign(cm[3].first, cm[3].length());
    shp.get<3>().assign(cm[4].first, cm[4].length());
    return shp;
  }
  class scheme {};
  class client : public scheme {
    map<int, response> Ready;
    detail::epoll Iom;
    Vec<socket> Rest;
    // ADDRINFOT
  public:
    client() {}
    // Scheme,host,port

    int req(const char *method, const char *url, MS2S header, const String& body, bool hb = false,
      bool ipe = false /*is percent encoding*/) {
      auto shp = analysis_url(url);
      auto& host = shp.get<1>();
      auto& port = shp.get<2>();
      if(port.size() == 0) {
        if(shp.get<0>().back() == 's')
          port = "443";
        else
          port = "80";
      }
      header.try_emplace("Host",
        shp.get<1>() + ':' + (shp.get<2>().size() == 0 ? (shp.get<0>().back() == 's' ? "443" : "80") : shp.get<2>()));
      String buf{};
      assemble_request_line(method, (ipe ? url : percent_encode(url)), buf);
      add_default_http_header(header);
      assemble_request_header(header, buf);
      if(hb) assemble_request_body(body, buf);
      std::cout.write(buf.data(), buf.size());
      auto fd = connect(host.data(), port.data());
      send(fd, buf);
      Iom.add(fd, detail::IOM_EVENTS::IN);
      return fd;
    }
    decltype(auto) get(const char *url, MS2S header, bool ipe = false /*is percent encoding*/) {
      return req("GET", url, xsl::as_rreference(header), {}, false, ipe);
    }
    decltype(auto) post(const char *url, MS2S header, const String& body, size_t count, bool ipe = false) {
      return req("GET", url, xsl::as_rreference(header), body, true, ipe);
    }
    const response& resp(int id) { return Ready.find(id)->second; }
    size_t refresh(int timeout) {
      vector<epoll_event> buf{xsl::tag_alloc{}, 20};
      buf.resize(10);
      Iom.refresh(buf, timeout);
      size_t n = 0;
      for(auto& e : buf) {
        auto ce = detail::event_convert1<0>(e.events);
        if(ce & detail::IOM_EVENTS::IN) {
          if(ce & detail::IOM_EVENTS::ERR) throw "err";
          auto str = recv(e.data.fd);
          Ready.try_emplace(e.data.fd, as_rreference(str));
          http::close(e.data.fd);
          ++n;
        }
      }
      return n;
    }
    void close() {}
  };
}  // namespace http

XSL_END
#endif  // !XSL_NET
