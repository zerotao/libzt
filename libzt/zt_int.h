#ifndef __zt_int_h__
#define __zt_int_h__

#include <libzt/zt.h>
#include <libzt/zt_except.h>

BEGIN_C_DECLS

#include <limits.h>
#include <stdint.h>


EXCEPT_DESC(Arithmatic, "Foo Exception Group",
	    EXCEPT_GROUP(Overflow, "Bar sub-group",
			 EXCEPTION(Integer, "Baz Exception")));

extern char    * zt_int_arithmetic_overflow;
extern char    * zt_int_arithmetic_divide_by_zero;

#define ZT_INT_IS_SIGNED(u) ((u)-1 < 0)
#define ZT_INT_SAME_SIGN(u,v) (!(((u) ^ (v)) < 0))

#define ZT_INT_UNSIGNED_ADD(ltype, sltype, sutype)                      \
    static INLINE ltype CONC3(zt_,sltype,_add)(ltype lhs,               \
                                          ltype rhs)                    \
    {                                                               	\
        if(lhs > CONC(sutype,_MAX) - rhs) {                             \
            THROW(zt_int_arithmetic_overflow);                          \
        }                                                               \
        return lhs + rhs;                                           	\
    }

ZT_INT_UNSIGNED_ADD(unsigned char, uchar, UCHAR)
ZT_INT_UNSIGNED_ADD(unsigned short, ushrt, USHRT)
ZT_INT_UNSIGNED_ADD(unsigned int, uint, UINT)
ZT_INT_UNSIGNED_ADD(unsigned long, ulong, ULONG)

#define ZT_INT_SIGNED_ADD(ltype, sltype, sutype)                    \
	static INLINE ltype CONC3(zt_,sltype,_add)(ltype lhs,           \
                                          ltype rhs)                \
	{                                                               \
		if(ZT_INT_SAME_SIGN(lhs, rhs)) {                            \
			if(rhs < 0)                                             \
			{                                                       \
				/* 2 negatives */                                   \
				if(lhs < CONC(sutype,_MIN) - rhs) {                 \
					THROW(zt_int_arithmetic_overflow);              \
				}                                                   \
			} else {                                                \
                /* 2 positives */                                   \
				if(CONC(sutype,_MAX) - lhs < rhs)                   \
				{                                                   \
					THROW(zt_int_arithmetic_overflow);              \
				}                                                   \
			}                                                       \
		}                                                           \
		return lhs + rhs;                                           \
	}

ZT_INT_SIGNED_ADD(signed char, char, CHAR)
ZT_INT_SIGNED_ADD(signed short, shrt, SHRT)
ZT_INT_SIGNED_ADD(signed int, int, INT)
ZT_INT_SIGNED_ADD(signed long, long, LONG)


/**************************************************************/

#define ZT_INT_UNSIGNED_SUB(ltype, sltype, sutype)                  \
	INLINE static ltype CONC3(zt_,sltype,_sub)(ltype lhs,           \
                                          ltype rhs)				\
	{                                                               \
		if(lhs < rhs) {                                             \
			THROW(zt_int_arithmetic_overflow);                      \
		}                                                           \
		return lhs - rhs;                                           \
	}

ZT_INT_UNSIGNED_SUB(unsigned char, uchar, UCHAR)
ZT_INT_UNSIGNED_SUB(unsigned short, ushrt, USHRT)
ZT_INT_UNSIGNED_SUB(unsigned int, uint, UINT)
ZT_INT_UNSIGNED_SUB(unsigned long, ulong, ULONG)

#define ZT_INT_SIGNED_SUB(ltype, sltype, sutype)                    \
	INLINE static ltype CONC3(zt_,sltype,_sub)(ltype lhs,            \
                                          ltype rhs) 				\
	{                                                               \
		if(!ZT_INT_SAME_SIGN(lhs, rhs))                             \
		{                                                           \
			if(lhs >= 0)                                            \
			{                                                       \
				if(lhs > CONC(sutype,_MAX) + rhs)                   \
				{                                                   \
					THROW(zt_int_arithmetic_overflow);              \
				}                                                   \
			}                                                       \
			else                                                    \
			{                                                       \
				if(lhs < CONC(sutype,_MIN + rhs))                   \
				{                                                   \
					THROW(zt_int_arithmetic_overflow);              \
				}                                                   \
			}                                                       \
		}                                                           \
		return (lhs - rhs);                                         \
	}

ZT_INT_SIGNED_SUB(signed char, char, CHAR)
ZT_INT_SIGNED_SUB(signed short, shrt, SHRT)
ZT_INT_SIGNED_SUB(signed int, int, INT)
ZT_INT_SIGNED_SUB(signed long, long, LONG)


/**************************************************************/

/* A > MAX / B */
#define ZT_INT_UNSIGNED_MUL(ltype, sltype, sutype, cvtype)          \
	INLINE static ltype CONC3(zt_,sltype,_mul)(ltype lhs,           \
                                          ltype rhs)				\
	{                                                               \
		cvtype _tmp = (cvtype)lhs * (cvtype)rhs;                    \
		if(((_tmp) >> (sizeof(ltype) * 8)) > 0) {                   \
			THROW(zt_int_arithmetic_overflow);                      \
		}                                                           \
		return _tmp;                                                \
	}
	
ZT_INT_UNSIGNED_MUL(unsigned char, uchar, UCHAR, unsigned int)
ZT_INT_UNSIGNED_MUL(unsigned short, ushrt, USHRT, unsigned int)

#ifdef HAVE_LONG_LONG
ZT_INT_UNSIGNED_MUL(unsigned int, uint, UINT, unsigned long long)
ZT_INT_UNSIGNED_MUL(unsigned long, ulong, ULONG, unsigned long long)
#endif

#ifdef HAVE_LONG_LONG
#define ZT_INT_CHECK_LONG_LONG                                          \
			case 4:                                                     \
                if(ZT_INT_IS_SIGNED(rhs) ||                             \
                   ZT_INT_IS_SIGNED(lhs))                               \
                {                                                       \
                    if(((_tmp & 0xffffffff80000000LL) != 0) &&          \
                       ((_tmp & 0xffffffff80000000LL)                   \
                        != 0xffffffff80000000LL)) {                     \
                            THROW(zt_int_arithmetic_overflow)           \
                        }                                               \
				} else {                                                \
					if((_tmp & 0xffffffff00000000ULL)) {                \
						THROW(zt_int_arithmetic_overflow);              \
					}                                                   \
				}                                                       \
				break;
#else
#define ZT_INT_CHECK_LONG_LONG
#endif

#define ZT_INT_SIGNED_MUL(ltype, sltype, sutype, cvtype)                \
	INLINE static ltype CONC3(zt_,sltype,_mul)(ltype lhs,               \
                                          ltype rhs)                    \
	{                                                                   \
		cvtype _tmp = 0;                                                \
		if((rhs == 0) || (lhs == 0)) {                                  \
			return 0;                                                   \
		}                                                               \
		_tmp = (cvtype)lhs * (cvtype)rhs;                               \
		switch(sizeof(cvtype)) {                                        \
			case 2:                                                     \
				if(ZT_INT_IS_SIGNED(rhs) ||                             \
				   ZT_INT_IS_SIGNED(lhs)) {                             \
					if(((_tmp & 0xffff8000) != 0) &&                    \
					   ((_tmp & 0xffff8000) != 0xffff8000)) {           \
						THROW(zt_int_arithmetic_overflow);              \
					}                                                   \
				} else {                                                \
					if ((_tmp & 0xffff0000)) {                          \
						THROW(zt_int_arithmetic_overflow);              \
					}                                                   \
				}                                                       \
				break;                                                  \
			case 1:                                                     \
				if(ZT_INT_IS_SIGNED(rhs) ||                             \
				   ZT_INT_IS_SIGNED(lhs))                               \
				{                                                       \
					if(((_tmp & 0xff80) != 0) &&                        \
					   ((_tmp & 0xff80) != 0xff80)) {                   \
						THROW(zt_int_arithmetic_overflow);              \
					}                                                   \
				} else {                                                \
					if(_tmp & 0xff00) {                                 \
						THROW(zt_int_arithmetic_overflow);              \
					}                                                   \
				}                                                       \
				break;                                                  \
			default:                                                    \
				if((rhs ^ lhs) < 0)                                     \
				{                                                       \
					if(lhs > 0)                                         \
					{                                                   \
						if(CONC(sutype,_MAX)/lhs < rhs)                 \
						{                                               \
							THROW(zt_int_arithmetic_overflow);          \
						}                                               \
					}                                                   \
					else                                                \
					{                                                   \
						if((lhs == CONC(sutype,_MIN)) ||                \
						   (rhs == CONC(sutype,_MIN))) {                \
							THROW(zt_int_arithmetic_overflow);          \
						}                                               \
						if(CONC(sutype,_MAX/-lhs < -rhs))               \
						{                                               \
							THROW(zt_int_arithmetic_overflow);          \
						}                                               \
					}                                                   \
				} else {                                                \
					/* mixed signs */                                   \
					if(lhs < 0) {                                       \
						if(lhs < CONC(sutype,_MIN)/rhs) {               \
							THROW(zt_int_arithmetic_overflow);          \
						}                                               \
					} else {                                            \
						if(rhs < CONC(sutype,_MIN)/lhs) {               \
							THROW(zt_int_arithmetic_overflow);          \
						}                                               \
					}                                                   \
				}                                                       \
				break;                                                  \
		}                                                               \
		return (lhs - rhs);                                             \
	}
	

ZT_INT_SIGNED_MUL(signed char, char, CHAR, signed int)
ZT_INT_SIGNED_MUL(signed short, shrt, SHRT, signed int)

#ifdef HAVE_LONG_LONG
ZT_INT_SIGNED_MUL(signed int, int, INT, signed long long)
ZT_INT_SIGNED_MUL(signed long, long, LONG, signed long long)
#endif

/**************************************************************/


#define ZT_INT_UNSIGNED_DIV(ltype, sltype, sutype, cvtype)      \
	INLINE static ltype CONC3(zt_,sltype,_div)(ltype lhs,       \
                                          ltype rhs)			\
    {                                                           \
        if(rhs == 0) {                                          \
            THROW(zt_int_arithmetic_divide_by_zero);            \
        }                                                       \
        return lhs / rhs;                                       \
    }


ZT_INT_UNSIGNED_DIV(unsigned char, uchar, UCHAR, unsigned int)
ZT_INT_UNSIGNED_DIV(unsigned short, ushrt, USHRT, unsigned int)

#ifdef HAVE_LONG_LONG
ZT_INT_UNSIGNED_DIV(unsigned int, uint, UINT, unsigned long long)
ZT_INT_UNSIGNED_DIV(unsigned long, ulong, ULONG, unsigned long long)
#endif

#define ZT_INT_SIGNED_DIV(ltype, sltype, sutype, cvtype)        \
	INLINE static ltype CONC3(zt_,sltype,_div)(ltype lhs,       \
                                          ltype rhs) 			\
    {                                                           \
        cvtype	  _tmp;                                         \
        if(rhs == 0) {                                          \
            THROW(zt_int_arithmetic_divide_by_zero);            \
        }                                                       \
        if(ZT_INT_IS_SIGNED(lhs) && ZT_INT_IS_SIGNED(rhs)) {    \
            if(lhs == CONC(sutype,_MIN) && rhs == -1) {         \
                THROW(zt_int_arithmetic_overflow);              \
            }                                                   \
        }                                                       \
        if(ZT_INT_SAME_SIGN(lhs, rhs)) {                        \
            return lhs / rhs;                                   \
        }                                                       \
        _tmp = (cvtype)lhs/(cvtype)rhs;                         \
        if(_tmp < CONC(sutype,_MIN)) {                          \
            THROW(zt_int_arithmetic_overflow);                  \
        }                                                       \
        return _tmp;                                            \
    }

ZT_INT_SIGNED_DIV(signed char, char, CHAR, signed int)
ZT_INT_SIGNED_DIV(signed short, shrt, SHRT, signed int)

#ifdef HAVE_LONG_LONG
ZT_INT_SIGNED_DIV(signed int, int, INT, signed long long)
ZT_INT_SIGNED_DIV(signed long, long, LONG, signed long long)
#endif

END_C_DECLS
#endif /* __zt_int_h__ */
