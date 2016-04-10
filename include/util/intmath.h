/*
	intmath.h - integer math convenience functions
*/

#ifndef INTMATH_H
#define INTMATH_H

#include <limits.h>

// Old functions - made into macros
#define u_sublim(a, b, c) sub_sat_u(a, b, c)
#define u_addlim(a, b, c) add_sat_u(a, b, c)
#define ul_sublim(a, b, c) sub_sat_ul(a, b, c)
#define ul_addlim(a, b, c) add_sat_ul(a, b, c)

#ifdef __cplusplus
extern "C" {
#endif

// Signed arithmetic

static inline
int add_i (int augend, int addend) {
	if ((addend > 0) && ((INT_MAX - addend) < augend)) return INT_MAX;
	if ((addend < 0) && ((INT_MIN - addend) > augend)) return INT_MIN;
	return (addend + augend);
}

static inline
int sub_i (int minuend, int subtrahend) {
	if ((subtrahend > 0) && ((INT_MIN + subtrahend) > minuend)) return INT_MIN;
	if ((subtrahend < 0) && ((INT_MAX + subtrahend) < minuend)) return INT_MAX;
	return (minuend - subtrahend);
}


//~ static inline
//~ int add_sat_i (int augend, int addend, int lower, int upper) {
	//~ if 
	//~ if ((addend > 0) && ((INT_MAX - addend) < augend)) return upper;
	//~ if ((addend < 0) && ((INT_MIN - addend) > augend)) return lower;
	//~ return (addend + augend);
//~ }

// Unsigned arithmetic

inline
unsigned add_u (unsigned augend, unsigned addend) {
	if ((UINT_MAX - addend) < augend) return UINT_MAX;
	return (addend + augend);
}

inline
unsigned sub_u (unsigned minuend, unsigned subtrahend) {
	if (subtrahend > minuend) return 0;
	return (minuend - subtrahend);
}

inline
unsigned add_ul (unsigned long augend, long unsigned addend) {
	if ((UINT_MAX - addend) < augend) return UINT_MAX;
	return (addend + augend);
}

inline
unsigned sub_ul (unsigned long minuend, unsigned long subtrahend) {
	if (subtrahend > minuend) return 0;
	return (minuend - subtrahend);
}

// Unsigned saturation arithmetic

inline
unsigned add_sat_u (unsigned augend, unsigned addend, unsigned limit) {
	if ((augend > limit) || ((limit - augend) < addend)) return limit;
	return (addend + augend);
}

inline
unsigned sub_sat_u (unsigned minuend, unsigned subtrahend, unsigned limit) {
	if ((minuend < limit) || ((minuend - limit) < subtrahend)) return limit;
	return (minuend - subtrahend);
}

inline
unsigned add_sat_ul (unsigned long augend, unsigned long addend, unsigned long limit) {
	if ((augend > limit) || ((limit - augend) < addend)) return limit;
	return (addend + augend);
}

inline
unsigned sub_sat_ul (unsigned long minuend, unsigned long subtrahend, unsigned long limit) {
	if ((minuend < limit) || ((minuend - limit) < subtrahend)) return limit;
	return (minuend - subtrahend);
}

#ifdef __cplusplus
}
#endif

#endif // INTMATH_H