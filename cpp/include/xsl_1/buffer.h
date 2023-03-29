#pragma once
#ifndef XSL_BUFFER
#define XSL_BUFFER
#include"bits/xsl_def.h"
#include"bits/xsl_allocator.h"
#include"bits/pf/memory.h"
#include"string.h"
XSL_BEGIN

template<class _Val, class _Alloc = default_allocator<_Val>>
class buffer {
	//
public:
	using val_type = _Val;
	using alloc_type = _Alloc;
	using size_type = alloc_type::size_type;
	//
public:
	constexpr buffer(size_type count)
		: Alc(), Head(Alc.allocate((count + 1) * sizeof(val_type))), Size(), Cap(count + 1) {};
	//
	constexpr buffer(const val_type* first, size_type count)
		: buffer(count) {
		write(first, count);
	};
	//
	constexpr buffer(const basic_string<val_type>& str)
		: buffer(str.data(), str.size()) {};
	//
	constexpr buffer(buffer&& ano)
		: Head(ano.Head), Size(ano.Size), Cap(ano.Cap) {
		ano.Head = nullptr;
		ano.Size = 0;
		ano.Cap = 0;
	};
	//
	constexpr ~buffer() {
		if (!this->invalid())
			Alc.deallocate(Head);
	}
	//
	constexpr buffer& operator=(buffer&& ano) {
		if (!this->invalid())
			Alc.deallocate(Head);
		Alc = as_rreference(ano.Alc);
		Head = ano.Head;
		Cap = ano.Cap;
		Size = ano.Size;
		ano.Head = nullptr;
		ano.Cap = 0;
		ano.Size = 0;
		return *this;
	};
	//
	constexpr val_type* data()noexcept {
		return Head;
	}
	//
	constexpr const val_type* data() const noexcept {
		return Head;
	}
	//
	constexpr size_t find(val_type val, size_t index = 0)const noexcept {
		size_t ret = strchr(Head + index, Size - index, val);
		return ret == npos ? npos : ret + index;
	}
	//
	constexpr size_t find(const val_type* first, size_t index = 0)const noexcept {
		size_t ret = strstr(Head + index, Size - index, first);
		return  ret == npos ? npos : ret + index;
	}
	//
	constexpr size_t find_first_not_of(val_type val, size_t index = 0)const noexcept {
		for (; index < Size; ++index)
			if (Head[index] != val)
				break;
		return index == Size ? npos : index;
	}
	//
	constexpr val_type& back()noexcept {
		return Head[Size - 1];
	}
	//
	constexpr val_type& operator[](size_t index)noexcept {
		return Head[index];
	}
	//
	constexpr const val_type& operator[](size_t index)const noexcept {
		return Head[index];
	}
	//
	constexpr bool read(val_type* first, size_t count)noexcept {
		if (count > Size)
			return false;
		MCopy(first, Head, count * sizeof(val_type));
		Size -= count;
		MCopy(Head, Head + count, Size * sizeof(val_type));
		return true;
	}
	//
	constexpr bool read(val_type* first, val_type* end)noexcept {
		return read(first, strstr(Head, Size, end));
	}
	//
	constexpr basic_string<val_type> read(size_t count)noexcept {
		basic_string<val_type> str{};
		if (count <= Size) {
			str.append(Head, count);
			Size -= count;
			MCopy(Head, Head + count, Size * sizeof(val_type));
		}
		return as_rreference(str);
	}
	//
	constexpr bool flush(size_t count) noexcept {
		if (Size < count)
			return false;
		Size -= count;
		MCopy(Head, Head + count, Size * sizeof(val_type));
		return true;
	}
	//
	constexpr buffer& write(const val_type* first, size_t count) {
		if (Cap - Size < count) {
			if (Cap == 0)Cap = count;
			while (Cap - Size < count)Cap <<= 1;
			val_type* NewHead = Alc.allocate(Cap * sizeof(val_type));
			MCopy(NewHead, Head, Size * sizeof(val_type));
			Alc.deallocate(Head);
			Head = NewHead;
		}
		MCopy(Head + Size, first, count * sizeof(val_type));
		Size += count;
		return *this;
	}
	//
	constexpr buffer& write(const val_type* first) {
		return write(first, batch::length(first));
	}
	//
	template<class Alloc>
	constexpr buffer& operator<<(const basic_string<val_type, Alloc>& str) {
		return write(str.data(), str.size());
	}
	//
	constexpr buffer& operator<<(const val_type* first) {
		return write(first, batch::length(first));
	}
	//
	constexpr buffer& operator<<(const val_type& val) {
		return write(&val, 1);
	}
	//
	constexpr bool invalid() const noexcept {
		return Head == nullptr;
	}
	//
	constexpr bool empty() const noexcept {
		return Cap == 0;
	}
	//
	constexpr bool full() const noexcept {
		return Size == Cap;
	}
	//
	constexpr void reserve(size_type NewCap) {
		if (Cap < NewCap) {
			if (Cap == 0)Cap = NewCap;
			while (Cap < NewCap)Cap <<= 1;
			val_type* NewHead = Alc.allocate(Cap * sizeof(val_type));
			MCopy(NewHead, Head, Size * sizeof(val_type));
			Alc.deallocate(Head);
			Head = NewHead;
		}
	}
	//
	constexpr void erase(size_t index = 0, size_t count = npos) {
		count = min(Size - index, count);
		Size -= count;
		MCopy(Head + index, Head + index + count, (Size - index) * sizeof(val_type));
	}
	//
	constexpr void clear() {
		Size = 0;
	}
	//
	constexpr void resize(size_t newsize) {
		Size = newsize;
	}
	//
	constexpr size_type size()const noexcept {
		return Size;
	}
	//
	constexpr size_type capacity()const noexcept {
		return Cap;
	}
	alloc_type Alc;
	val_type* Head;
	size_type Size, Cap;
};
XSL_END
#endif // !XSL_BASIC_STRING
