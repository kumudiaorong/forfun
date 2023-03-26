#pragma once
#ifndef LOOP_H
#define LOOP_H
#include"xdef.h"
#include"totp.h"
#define TYPE totype::
namespace toloop {
	template<_ll_ Max, _ll_ Min = 0>
	class num_loop_t
	{
		using num_t = TYPE num_type<Max, Min>;
		num_t num;
		inline num_t format(_ll_ n) {
			_ll_ d = Max - Min;
			return Min + ((n < 0 ? d - (0 - n) % d : n % d) - Min) % d;
		}
	public:
		num_loop_t(num_t start = 0) :num(format(start)) {}
		inline num_loop_t& operator++() {
			if (++num == Max)num = Min;
			return *this;
		}
		inline num_loop_t operator++(int) {
			num_loop_t tnuml(num);
			if (++num == Max)num = Min;
			return tnuml;
		}
		inline num_loop_t& operator--() {
			if (--num == Min)num = Max - 1;
			return *this;
		}
		inline num_loop_t operator--(int) {
			num_loop_t tnuml(num);
			if (--num == Min)num = Max - 1;
			return tnuml;
		}
		inline num_loop_t add(_ll_ n) { return num_loop_t(format(num + n)); }
		inline num_loop_t reduce(_ll_ n) { return num_loop_t(format(num - n)); }
		operator _ll_() { return _ll_(num); }
	};
	template<typename num_t = _ll_>
	class num_loop_v
	{
		num_t Max, Min, num;
		inline num_t format(_ll_ n) {
			num_t d = Max - Min;
			return Min + ((n < 0 ? d - (0 - n) % d : n % d) - Min) % d;
		}
	public:
		num_loop_v(num_t mx, num_t mn, num_t start) :Max(mx), Min(mn), num(format(start)) {}
		num_loop_v(num_t lp, num_t start = 0) :Max(start + lp), Min(start), num(start) {}
		inline void reset(num_t mx, num_t start = 0, num_t mn = 0) {
			Max = mx;
			Min = mn;
			num = start;
		}
		num_loop_v(num_loop_v&&) = default;
		num_loop_v(const num_loop_v&) = default;
		inline num_t loop() { return Max - Min; }
		inline num_loop_v& operator++() {
			if (++num == Max)num = Min;
			return *this;
		}
		inline num_loop_v operator++(int) {
			num_loop_v t_numl(Max, Min, num);
			if (++num == Max)num = Min;
			return t_numl;
		}
		inline num_loop_v& operator--() {
			if (num-- == Min)num = Max - 1;
			return *this;
		}
		inline num_loop_v operator--(int) {
			num_loop_v t_numl(Max, Min, num);
			if (num-- == Min)num = Max - 1;
			return t_numl;
		}
		inline num_loop_v add(_ll_ n) { return num_loop_v(Max, Min, format(num + n)); }
		inline num_loop_v reduce(_ll_ n) { return num_loop_v(Max, Min, format(num - n)); }
		operator _ll_() { return _ll_(num); }
	};
}
#undef TYPE
#endif // !LOOP_H
