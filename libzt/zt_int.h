#ifndef __zt_int_h__
#define __zt_int_h__

#include <libzt/zt.h>
#include <libzt/zt_except.h>

BEGIN_C_DECLS

#include <limits.h>
#include <stdint.h>

#define UNSIGNED_ADD(ltype, sltype, sutype)                              \
        char    * CONC(sltype,_overflow) = STR(CONC(sltype,_overflow)); \
        INLINE ltype CONC(sltype,_add)(ltype a,                         \
                                       ltype b)                         \
        {                                                               \
                if(a > CONC(sutype,_MAX) - b) {                         \
                        THROW(CONC(sltype,_overflow));                  \
                }                                                       \
                return a + b;                                           \
        }                                                               \


UNSIGNED_ADD(unsigned char, uchar, UCHAR);
UNSIGNED_ADD(unsigned short, ushrt, USHRT);
UNSIGNED_ADD(unsigned int, uint, UINT);
UNSIGNED_ADD(unsigned long, ulong, ULONG);

char    * arithmetic_overflow = "";

int_add(int a,
        int b) 
{
        if(!((a ^ b) < 0)) {
                if(b < 0)
                {
                        /* 2 negatives */
                        if(a < INT_MIN - b) {
                                THROW(arithmetic_overflow);
                        }
                } else {
                        /* 2 positives */
                        if(INT_MAX - a < b)
                        {
                                THROW(arithmetic_overflow);
                        }
                }
        }
        return a + b;
}

END_C_DECLS
#endif /* __zt_int_h__ */

