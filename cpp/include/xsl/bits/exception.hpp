#pragma once
#ifndef XSL_BASE_EXCEPTION
#define XSL_BASE_EXCEPTION
#include <xsl/bits/def.hpp>
namespace xsl::ecp {
  class exception {
  public:
    virtual const char *what() {
      return "exception";
    }
    virtual uint_64 err() {
      return 0;
    }
  };
  class bad_alloc : public exception {
  public:
    virtual const char *what() {
      return "bad_alloc";
    }
    virtual uint_64 err() {
      return 1;
    }
  };
  class out_of_range : public exception {
  public:
    virtual const char *what() {
      return "out_of_range";
    }
    virtual uint_64 err() {
      return 2;
    }
  };
  class empty_object : public exception {
  public:
    virtual const char *what() {
      return "empty_object";
    }
    virtual uint_64 err() {
      return 3;
    }
  };
  class wrong_key : public exception {
  public:
    virtual const char *what() {
      return "wrong_key";
    }
    virtual uint_64 err() {
      return 4;
    }
  };
  constexpr void check_empty_object(bool sign) {
    if(sign)
      throw empty_object{};
  }
  constexpr void check_bad_alloc(bool sign) {
    if(sign)
      throw bad_alloc{};
  }
  constexpr void check_out_of_range(bool sign) {
    if(sign)
      throw out_of_range{};
  }
  // statement STMT exception exc
}  // namespace xsl::exp
#endif  // !XSL_BASE_EXCEPTION
