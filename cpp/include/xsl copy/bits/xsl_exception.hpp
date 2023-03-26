#pragma once
#ifndef XSL_BASE_EXCEPTION
#define XSL_BASE_EXCEPTION
#include"xsl_def.hpp"
XSL_BEGIN
class exception
{
public:
	virtual const char* what() {
		return "exception";
	}
	virtual uint_64  err() {
		return 0;
	}
};
class bad_alloc :public exception {
public:
	virtual const char* what() {
		return "bad_alloc";
	}
	virtual uint_64  err() {
		return 1;
	}
};
class out_of_range :public exception {
public:
	virtual const char* what() {
		return "out_of_range";
	}
	virtual uint_64 err() {
		return 2;
	}
};
class empty_object :public exception {
public:
	virtual const char* what() {
		return "empty_object";
	}
	virtual uint_64 err() {
		return 3;
	}
};
class wrong_key :public exception {
public:
	virtual const char* what() {
		return "wrong_key";
	}
	virtual uint_64 err() {
		return 4;
	}
};
#define XSL_EMPTY_CHECK(sign) if (bool(sign))throw empty_object{};
#define XSL_ALLOC_CHECK(sign) if (bool(sign))throw bad_alloc{};
#define XSL_RANGE_CHECK(sign) if (bool(sign))throw out_of_range{};
//statement STMT exception exc
#define XSL_TRY try{

#define XSL_EXC_DESTRUCT(CLASS) }catch (const xsl::exception& Exc) { this->~CLASS(); throw Exc; }

XSL_END
#endif // !XSL_BASE_EXCEPTION
