#include <libzt/zt_int.h>
#include <libzt/zt_unit.h>

static void
basic_tests(struct zt_unit_test *test, void *data)
{
        unsigned int    a = 22;
        unsigned int    b = 44;
        unsigned int    c = 0;

        
        c = zt_uint_add(a, b);

        /* check addition overflow */
        
        /* CHAR pos common */
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_char_add(CHAR_MAX, 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_char_add(1, CHAR_MAX));
        /* CHAR neg common */
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_char_add(CHAR_MIN, -1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_char_add(-1, CHAR_MIN));
        /* CHAR limits */
        ZT_UNIT_ASSERT(test, zt_char_add(CHAR_MIN, 1) == CHAR_MIN + 1);
        ZT_UNIT_ASSERT(test, zt_char_add(1, CHAR_MIN) == CHAR_MIN + 1);
        ZT_UNIT_ASSERT(test, zt_char_add(CHAR_MAX, -1) == CHAR_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_char_add(-1, CHAR_MAX) == CHAR_MAX - 1);
        /* CHAR middle */
        ZT_UNIT_ASSERT(test, zt_char_add(CHAR_MAX/2, 1) == CHAR_MAX/2 + 1);
        ZT_UNIT_ASSERT(test, zt_char_add(1, CHAR_MAX/2) == CHAR_MAX/2 + 1);
        
        
        /* SHORT pos common */
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_add(SHORT_MAX, 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_add(1, SHORT_MAX));
        /* SHORT neg common */
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_add(SHORT_MIN, -1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_add(-1, SHORT_MIN));
        /* SHORT limits */
        ZT_UNIT_ASSERT(test, zt_short_add(SHORT_MIN, 1) == SHORT_MIN + 1);
        ZT_UNIT_ASSERT(test, zt_short_add(1, SHORT_MIN) == SHORT_MIN + 1);
        ZT_UNIT_ASSERT(test, zt_short_add(SHORT_MAX, -1) == SHORT_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_short_add(-1, SHORT_MAX) == SHORT_MAX - 1);
        /* SHORT middle */
        ZT_UNIT_ASSERT(test, zt_short_add(SHORT_MAX/2, 1) == SHORT_MAX/2 + 1);
        ZT_UNIT_ASSERT(test, zt_short_add(1, SHORT_MAX/2) == SHORT_MAX/2 + 1);

        /* INT pos common */
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_int_add(INT_MAX, 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_int_add(1, INT_MAX));
        /* INT neg common */
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_int_add(INT_MIN, -1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_int_add(-1, INT_MIN));
        /* INT limits */
        ZT_UNIT_ASSERT(test, zt_int_add(INT_MIN, 1) == INT_MIN + 1);
        ZT_UNIT_ASSERT(test, zt_int_add(1, INT_MIN) == INT_MIN + 1);
        ZT_UNIT_ASSERT(test, zt_int_add(INT_MAX, -1) == INT_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_int_add(-1, INT_MAX) == INT_MAX - 1);
        /* INT middle */
        ZT_UNIT_ASSERT(test, zt_int_add(INT_MAX/2, 1) == INT_MAX/2 + 1);
        ZT_UNIT_ASSERT(test, zt_int_add(1, INT_MAX/2) == INT_MAX/2 + 1);

        /* LONG pos common */
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_long_add(LONG_MAX, 1L));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_long_add(1L, LONG_MAX));
        /* LONG neg common */
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_long_add(LONG_MIN, -1L));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_long_add(-1L, LONG_MIN));
        /* LONG limits */
        ZT_UNIT_ASSERT(test, zt_long_add(LONG_MIN, 1L) == LONG_MIN + 1L);
        ZT_UNIT_ASSERT(test, zt_long_add(1L, LONG_MIN) == LONG_MIN + 1L);
        ZT_UNIT_ASSERT(test, zt_long_add(LONG_MAX, -1L) == LONG_MAX - 1L);
        ZT_UNIT_ASSERT(test, zt_long_add(-1L, LONG_MAX) == LONG_MAX - 1L);
        /* LONG middle */
        ZT_UNIT_ASSERT(test, zt_long_add(LONG_MAX/2, 1) == LONG_MAX/2 + 1);
        ZT_UNIT_ASSERT(test, zt_long_add(1, LONG_MAX/2) == LONG_MAX/2 + 1);


        /* unsigned addition overflow */
        
        /* UCHAR pos common */
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uchar_add(UCHAR_MAX, 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uchar_add(1, UCHAR_MAX));
        /* UCHAR neg common */
        ZT_UNIT_ASSERT(test, zt_uchar_add(UCHAR_MIN, -1) == UCHAR_MAX);
        ZT_UNIT_ASSERT(test, zt_uchar_add(-1, UCHAR_MIN) == UCHAR_MAX);
        /* UCHAR limits */
        ZT_UNIT_ASSERT(test, zt_uchar_add(UCHAR_MIN, 1) == UCHAR_MIN + 1);
        ZT_UNIT_ASSERT(test, zt_uchar_add(1, UCHAR_MIN) == UCHAR_MIN + 1);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uchar_add(UCHAR_MAX, -1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uchar_add(-1, UCHAR_MAX));
        /* UCHAR middle */
        ZT_UNIT_ASSERT(test, zt_uchar_add(UCHAR_MAX/2, 1) == UCHAR_MAX/2 + 1);
        ZT_UNIT_ASSERT(test, zt_uchar_add(1, UCHAR_MAX/2) == UCHAR_MAX/2 + 1);
        

        /* USHORT pos common */
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ushort_add(USHORT_MAX, 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ushort_add(1, USHORT_MAX));
        /* USHORT neg common */
        ZT_UNIT_ASSERT(test, zt_ushort_add(USHORT_MIN, -1) == USHORT_MAX);
        ZT_UNIT_ASSERT(test, zt_ushort_add(-1, USHORT_MIN) == USHORT_MAX);
        /* USHORT limits */
        ZT_UNIT_ASSERT(test, zt_ushort_add(USHORT_MIN, 1) == USHORT_MIN + 1);
        ZT_UNIT_ASSERT(test, zt_ushort_add(1, USHORT_MIN) == USHORT_MIN + 1);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ushort_add(USHORT_MAX, -1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ushort_add(-1, USHORT_MAX));
        /* USHORT middle */
        ZT_UNIT_ASSERT(test, zt_ushort_add(USHORT_MAX/2, 1) == USHORT_MAX/2 + 1);
        ZT_UNIT_ASSERT(test, zt_ushort_add(1, USHORT_MAX/2) == USHORT_MAX/2 + 1);
        

        /* UINT pos common */
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uint_add(UINT_MAX, 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uint_add(1, UINT_MAX));
        /* UINT neg common */
        ZT_UNIT_ASSERT(test, zt_uint_add(UINT_MIN, -1) == UINT_MAX);
        ZT_UNIT_ASSERT(test, zt_uint_add(-1, UINT_MIN) == UINT_MAX);
        /* UINT limits */
        ZT_UNIT_ASSERT(test, zt_uint_add(UINT_MIN, 1) == UINT_MIN + 1);
        ZT_UNIT_ASSERT(test, zt_uint_add(1, UINT_MIN) == UINT_MIN + 1);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uint_add(UINT_MAX, -1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uint_add(-1, UINT_MAX));
        /* UINT middle */
        ZT_UNIT_ASSERT(test, zt_uint_add(UINT_MAX/2, 1) == UINT_MAX/2 + 1);
        ZT_UNIT_ASSERT(test, zt_uint_add(1, UINT_MAX/2) == UINT_MAX/2 + 1);


        /* ULONG pos common */
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ulong_add(ULONG_MAX, 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ulong_add(1, ULONG_MAX));
        /* ULONG neg common */
        ZT_UNIT_ASSERT(test, zt_ulong_add(ULONG_MIN, -1) == ULONG_MAX);
        ZT_UNIT_ASSERT(test, zt_ulong_add(-1, ULONG_MIN) == ULONG_MAX);
        /* ULONG limits */
        ZT_UNIT_ASSERT(test, zt_ulong_add(ULONG_MIN, 1) == ULONG_MIN + 1);
        ZT_UNIT_ASSERT(test, zt_ulong_add(1, ULONG_MIN) == ULONG_MIN + 1);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ulong_add(ULONG_MAX, -1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ulong_add(-1, ULONG_MAX));
        /* ULONG middle */
        ZT_UNIT_ASSERT(test, zt_ulong_add(ULONG_MAX/2, 1) == ULONG_MAX/2 + 1);
        ZT_UNIT_ASSERT(test, zt_ulong_add(1, ULONG_MAX/2) == ULONG_MAX/2 + 1);
        

        /* subtraction overflow */

        ZT_UNIT_ASSERT(test, zt_char_sub(CHAR_MAX, 1) == CHAR_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_char_sub(1, CHAR_MAX) == 1 - CHAR_MAX);
        ZT_UNIT_ASSERT(test, zt_char_sub(CHAR_MIN, -1) == CHAR_MIN - -1);
        ZT_UNIT_ASSERT(test, zt_char_sub(-1, CHAR_MIN) == -1 - CHAR_MIN);
        ZT_UNIT_ASSERT(test, zt_char_sub(CHAR_MAX/2, 1) == (CHAR_MAX/2) - 1);
        ZT_UNIT_ASSERT(test, zt_char_sub(1, CHAR_MAX/2) == 1 - (CHAR_MAX/2));

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_char_sub(CHAR_MIN, 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_char_sub(1, CHAR_MIN));
        
        ZT_UNIT_ASSERT(test, zt_short_sub(SHORT_MAX, 1) == SHORT_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_short_sub(1, SHORT_MAX) == 1 - SHORT_MAX);
        ZT_UNIT_ASSERT(test, zt_short_sub(SHORT_MIN, -1) == SHORT_MIN - -1);
        ZT_UNIT_ASSERT(test, zt_short_sub(-1, SHORT_MIN) == -1 - SHORT_MIN);
        ZT_UNIT_ASSERT(test, zt_short_sub(SHORT_MAX/2, 1) == (SHORT_MAX/2) - 1);
        ZT_UNIT_ASSERT(test, zt_short_sub(1, SHORT_MAX/2) == 1 - (SHORT_MAX/2));

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_sub(SHORT_MIN, 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_sub(1, SHORT_MIN));

        ZT_UNIT_ASSERT(test, zt_int_sub(INT_MAX, 1) == INT_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_int_sub(1, INT_MAX) == 1 - INT_MAX);
        ZT_UNIT_ASSERT(test, zt_int_sub(INT_MIN, -1) == INT_MIN - -1);
        ZT_UNIT_ASSERT(test, zt_int_sub(-1, INT_MIN) == -1 - INT_MIN);
        ZT_UNIT_ASSERT(test, zt_int_sub(INT_MAX/2, 1) == (INT_MAX/2) - 1);
        ZT_UNIT_ASSERT(test, zt_int_sub(1, INT_MAX/2) == 1 - (INT_MAX/2));

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_int_sub(INT_MIN, 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_int_sub(1, INT_MIN));

        ZT_UNIT_ASSERT(test, zt_long_sub(LONG_MAX, 1) == LONG_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_long_sub(1, LONG_MAX) == 1 - LONG_MAX);
        ZT_UNIT_ASSERT(test, zt_long_sub(LONG_MIN, -1) == LONG_MIN - -1);
        ZT_UNIT_ASSERT(test, zt_long_sub(-1, LONG_MIN) == -1 - LONG_MIN);
        ZT_UNIT_ASSERT(test, zt_long_sub(LONG_MAX/2, 1) == (LONG_MAX/2) - 1);
        ZT_UNIT_ASSERT(test, zt_long_sub(1, LONG_MAX/2) == 1 - (LONG_MAX/2));

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_long_sub(LONG_MIN, 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_long_sub(1, LONG_MIN));
        
        /* unsigned subtraction */
        ZT_UNIT_ASSERT(test, zt_uchar_sub(UCHAR_MAX, 1) == UCHAR_MAX - 1);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uchar_sub(1, UCHAR_MAX));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uchar_sub(UCHAR_MIN, 1));

        ZT_UNIT_ASSERT(test, zt_uchar_sub(UCHAR_MAX/2, 1) == (UCHAR_MAX/2) - 1);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uchar_sub(1, UCHAR_MAX/2));

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uchar_sub(UCHAR_MIN, 1));
        ZT_UNIT_ASSERT(test, zt_uchar_sub(1, UCHAR_MIN) == 1 - UCHAR_MIN);


        ZT_UNIT_ASSERT(test, zt_ushort_sub(USHORT_MAX, 1) == USHORT_MAX - 1);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ushort_sub(1, USHORT_MAX));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ushort_sub(USHORT_MIN, 1));

        ZT_UNIT_ASSERT(test, zt_ushort_sub(USHORT_MAX/2, 1) == (USHORT_MAX/2) - 1);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ushort_sub(1, USHORT_MAX/2));

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ushort_sub(USHORT_MIN, 1));
        ZT_UNIT_ASSERT(test, zt_ushort_sub(1, USHORT_MIN) == 1 - USHORT_MIN);
        

        ZT_UNIT_ASSERT(test, zt_uint_sub(UINT_MAX, 1) == UINT_MAX - 1);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uint_sub(1, UINT_MAX));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uint_sub(UINT_MIN, 1));

        ZT_UNIT_ASSERT(test, zt_uint_sub(UINT_MAX/2, 1) == (UINT_MAX/2) - 1);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uint_sub(1, UINT_MAX/2));

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uint_sub(UINT_MIN, 1));
        ZT_UNIT_ASSERT(test, zt_uint_sub(1, UINT_MIN) == 1 - UINT_MIN);

        ZT_UNIT_ASSERT(test, zt_ulong_sub(ULONG_MAX, 1) == ULONG_MAX - 1);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ulong_sub(1, ULONG_MAX));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ulong_sub(ULONG_MIN, 1));

        ZT_UNIT_ASSERT(test, zt_ulong_sub(ULONG_MAX/2, 1) == (ULONG_MAX/2) - 1);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ulong_sub(1, ULONG_MAX/2));

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ulong_sub(ULONG_MIN, 1));
        ZT_UNIT_ASSERT(test, zt_ulong_sub(1, ULONG_MIN) == 1 - ULONG_MIN);


        /* multiplication */
        /* char */
        ZT_UNIT_ASSERT(test, zt_char_mul(1, CHAR_MAX) == CHAR_MAX);
        ZT_UNIT_ASSERT(test, zt_char_mul(CHAR_MAX, 1) == CHAR_MAX);

        ZT_UNIT_ASSERT(test, zt_char_mul(-1, CHAR_MAX) == -CHAR_MAX);
        ZT_UNIT_ASSERT(test, zt_char_mul(CHAR_MAX, -1) == -CHAR_MAX);
        
        ZT_UNIT_ASSERT(test, zt_char_mul(1, CHAR_MIN) == CHAR_MIN);
        ZT_UNIT_ASSERT(test, zt_char_mul(CHAR_MIN, 1) == CHAR_MIN);

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_char_mul(-1, CHAR_MIN));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_char_mul(CHAR_MIN, -1));
        
        ZT_UNIT_ASSERT(test, zt_char_mul(2, CHAR_MAX/2) == CHAR_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_char_mul(CHAR_MAX/2, 2) == CHAR_MAX - 1);

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_char_mul(2, CHAR_MAX));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_char_mul(CHAR_MAX, 2));
        
        /* short */
        ZT_UNIT_ASSERT(test, zt_short_mul(1, SHORT_MAX) == SHORT_MAX);
        ZT_UNIT_ASSERT(test, zt_short_mul(SHORT_MAX, 1) == SHORT_MAX);

        ZT_UNIT_ASSERT(test, zt_short_mul(-1, SHORT_MAX) == -SHORT_MAX);
        ZT_UNIT_ASSERT(test, zt_short_mul(SHORT_MAX, -1) == -SHORT_MAX);
        
        ZT_UNIT_ASSERT(test, zt_short_mul(1, SHORT_MIN) == SHORT_MIN);
        ZT_UNIT_ASSERT(test, zt_short_mul(SHORT_MIN, 1) == SHORT_MIN);

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_mul(-1, SHORT_MIN));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_mul(SHORT_MIN, -1));
        
        ZT_UNIT_ASSERT(test, zt_short_mul(2, SHORT_MAX/2) == SHORT_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_short_mul(SHORT_MAX/2, 2) == SHORT_MAX - 1);

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_mul(2, SHORT_MAX));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_mul(SHORT_MAX, 2));

        /* integer */
        ZT_UNIT_ASSERT(test, zt_int_mul(1, INT_MAX) == INT_MAX);
        ZT_UNIT_ASSERT(test, zt_int_mul(INT_MAX, 1) == INT_MAX);

        ZT_UNIT_ASSERT(test, zt_int_mul(-1, INT_MAX) == -INT_MAX);
        ZT_UNIT_ASSERT(test, zt_int_mul(INT_MAX, -1) == -INT_MAX);
        
        ZT_UNIT_ASSERT(test, zt_int_mul(1, INT_MIN) == INT_MIN);
        ZT_UNIT_ASSERT(test, zt_int_mul(INT_MIN, 1) == INT_MIN);
        
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_int_mul(-1, INT_MIN));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_int_mul(INT_MIN, -1));
        
        ZT_UNIT_ASSERT(test, zt_int_mul(2, INT_MAX/2) == INT_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_int_mul(INT_MAX/2, 2) == INT_MAX - 1);
        
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_int_mul(2, INT_MAX));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_int_mul(INT_MAX, 2));

        /* short */
        ZT_UNIT_ASSERT(test, zt_short_mul(1, SHORT_MAX) == SHORT_MAX);
        ZT_UNIT_ASSERT(test, zt_short_mul(SHORT_MAX, 1) == SHORT_MAX);

        ZT_UNIT_ASSERT(test, zt_short_mul(-1, SHORT_MAX) == -SHORT_MAX);
        ZT_UNIT_ASSERT(test, zt_short_mul(SHORT_MAX, -1) == -SHORT_MAX);
        
        ZT_UNIT_ASSERT(test, zt_short_mul(1, SHORT_MIN) == SHORT_MIN);
        ZT_UNIT_ASSERT(test, zt_short_mul(SHORT_MIN, 1) == SHORT_MIN);

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_mul(-1, SHORT_MIN));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_mul(SHORT_MIN, -1));
        
        ZT_UNIT_ASSERT(test, zt_short_mul(2, SHORT_MAX/2) == SHORT_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_short_mul(SHORT_MAX/2, 2) == SHORT_MAX - 1);

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_mul(2, SHORT_MAX));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_mul(SHORT_MAX, 2));

        /* unsigned multiplication */
        /* char */
        ZT_UNIT_ASSERT(test, zt_uchar_mul(1, UCHAR_MAX) == UCHAR_MAX);
        ZT_UNIT_ASSERT(test, zt_uchar_mul(UCHAR_MAX, 1) == UCHAR_MAX);

        ZT_UNIT_ASSERT(test, zt_uchar_mul(1, UCHAR_MIN) == UCHAR_MIN);
        ZT_UNIT_ASSERT(test, zt_uchar_mul(UCHAR_MIN, 1) == UCHAR_MIN);

        ZT_UNIT_ASSERT(test, zt_uchar_mul(2, UCHAR_MAX/2) == UCHAR_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_uchar_mul(UCHAR_MAX/2, 2) == UCHAR_MAX - 1);

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uchar_mul(2, UCHAR_MAX));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uchar_mul(UCHAR_MAX, 2));

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uchar_mul(2, UCHAR_MAX/2 + 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uchar_mul(UCHAR_MAX/2 + 1, 2));

        /* short */
        ZT_UNIT_ASSERT(test, zt_ushort_mul(1, USHORT_MAX) == USHORT_MAX);
        ZT_UNIT_ASSERT(test, zt_ushort_mul(USHORT_MAX, 1) == USHORT_MAX);

        ZT_UNIT_ASSERT(test, zt_ushort_mul(1, USHORT_MIN) == USHORT_MIN);
        ZT_UNIT_ASSERT(test, zt_ushort_mul(USHORT_MIN, 1) == USHORT_MIN);

        ZT_UNIT_ASSERT(test, zt_ushort_mul(2, USHORT_MAX/2) == USHORT_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_ushort_mul(USHORT_MAX/2, 2) == USHORT_MAX - 1);

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ushort_mul(2, USHORT_MAX));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ushort_mul(USHORT_MAX, 2));

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ushort_mul(2, USHORT_MAX/2 + 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ushort_mul(USHORT_MAX/2 + 1, 2));

        /* int */
        ZT_UNIT_ASSERT(test, zt_uint_mul(1, UINT_MAX) == UINT_MAX);
        ZT_UNIT_ASSERT(test, zt_uint_mul(UINT_MAX, 1) == UINT_MAX);

        ZT_UNIT_ASSERT(test, zt_uint_mul(1, UINT_MIN) == UINT_MIN);
        ZT_UNIT_ASSERT(test, zt_uint_mul(UINT_MIN, 1) == UINT_MIN);

        ZT_UNIT_ASSERT(test, zt_uint_mul(2, UINT_MAX/2) == UINT_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_uint_mul(UINT_MAX/2, 2) == UINT_MAX - 1);

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uint_mul(2, UINT_MAX));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uint_mul(UINT_MAX, 2));

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uint_mul(2, UINT_MAX/2 + 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_uint_mul(UINT_MAX/2 + 1, 2));

        /* long */
        ZT_UNIT_ASSERT(test, zt_ulong_mul(1, ULONG_MAX) == ULONG_MAX);
        ZT_UNIT_ASSERT(test, zt_ulong_mul(ULONG_MAX, 1) == ULONG_MAX);

        ZT_UNIT_ASSERT(test, zt_ulong_mul(1, ULONG_MIN) == ULONG_MIN);
        ZT_UNIT_ASSERT(test, zt_ulong_mul(ULONG_MIN, 1) == ULONG_MIN);

        ZT_UNIT_ASSERT(test, zt_ulong_mul(2, ULONG_MAX/2) == ULONG_MAX - 1);
        ZT_UNIT_ASSERT(test, zt_ulong_mul(ULONG_MAX/2, 2) == ULONG_MAX - 1);

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ulong_mul(2, ULONG_MAX));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ulong_mul(ULONG_MAX, 2));

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ulong_mul(2, ULONG_MAX/2 + 1));
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_ulong_mul(ULONG_MAX/2 + 1, 2));

        /* division */
        /* char */
        ZT_UNIT_ASSERT(test, zt_char_div(1, CHAR_MAX) == 0);
        ZT_UNIT_ASSERT(test, zt_char_div(CHAR_MAX, 1) == CHAR_MAX);

        ZT_UNIT_ASSERT(test, zt_char_div(1, CHAR_MIN) == 0);
        ZT_UNIT_ASSERT(test, zt_char_div(CHAR_MIN, 1) == CHAR_MIN);

        ZT_UNIT_ASSERT(test, zt_char_div(0, CHAR_MIN) == 0);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.divide_by_zero, zt_char_div(CHAR_MAX, 0));
        
        ZT_UNIT_ASSERT(test, zt_char_div(CHAR_MAX, 2) == CHAR_MAX / 2);
        ZT_UNIT_ASSERT(test, zt_char_div(2, CHAR_MAX) == 2 / CHAR_MAX);

        ZT_UNIT_ASSERT(test, zt_char_div(CHAR_MIN, 2) == CHAR_MIN / 2);
        ZT_UNIT_ASSERT(test, zt_char_div(2, CHAR_MIN) == 2 / CHAR_MIN);

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_char_div(CHAR_MIN, -1));

        /* short */
        ZT_UNIT_ASSERT(test, zt_short_div(1, SHORT_MAX) == 0);
        ZT_UNIT_ASSERT(test, zt_short_div(SHORT_MAX, 1) == SHORT_MAX);

        ZT_UNIT_ASSERT(test, zt_short_div(1, SHORT_MIN) == 0);
        ZT_UNIT_ASSERT(test, zt_short_div(SHORT_MIN, 1) == SHORT_MIN);

        ZT_UNIT_ASSERT(test, zt_short_div(0, SHORT_MIN) == 0);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.divide_by_zero, zt_short_div(SHORT_MAX, 0));
        
        ZT_UNIT_ASSERT(test, zt_short_div(SHORT_MAX, 2) == SHORT_MAX / 2);
        ZT_UNIT_ASSERT(test, zt_short_div(2, SHORT_MAX) == 2 / SHORT_MAX);

        ZT_UNIT_ASSERT(test, zt_short_div(SHORT_MIN, 2) == SHORT_MIN / 2);
        ZT_UNIT_ASSERT(test, zt_short_div(2, SHORT_MIN) == 2 / SHORT_MIN);

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_short_div(SHORT_MIN, -1));
        
        /* int */
        ZT_UNIT_ASSERT(test, zt_int_div(1, INT_MAX) == 0);
        ZT_UNIT_ASSERT(test, zt_int_div(INT_MAX, 1) == INT_MAX);

        ZT_UNIT_ASSERT(test, zt_int_div(1, INT_MIN) == 0);
        ZT_UNIT_ASSERT(test, zt_int_div(INT_MIN, 1) == INT_MIN);

        ZT_UNIT_ASSERT(test, zt_int_div(0, INT_MIN) == 0);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.divide_by_zero, zt_int_div(INT_MAX, 0));
        
        ZT_UNIT_ASSERT(test, zt_int_div(INT_MAX, 2) == INT_MAX / 2);
        ZT_UNIT_ASSERT(test, zt_int_div(2, INT_MAX) == 2 / INT_MAX);

        ZT_UNIT_ASSERT(test, zt_int_div(INT_MIN, 2) == INT_MIN / 2);
        ZT_UNIT_ASSERT(test, zt_int_div(2, INT_MIN) == 2 / INT_MIN);

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_int_div(INT_MIN, -1));        
        
        /* long */
        ZT_UNIT_ASSERT(test, zt_long_div(1, LONG_MAX) == 0);
        ZT_UNIT_ASSERT(test, zt_long_div(LONG_MAX, 1) == LONG_MAX);

        ZT_UNIT_ASSERT(test, zt_long_div(1, LONG_MIN) == 0);
        ZT_UNIT_ASSERT(test, zt_long_div(LONG_MIN, 1) == LONG_MIN);

        ZT_UNIT_ASSERT(test, zt_long_div(0, LONG_MIN) == 0);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.divide_by_zero, zt_long_div(LONG_MAX, 0));
        
        ZT_UNIT_ASSERT(test, zt_long_div(LONG_MAX, 2) == LONG_MAX / 2);
        ZT_UNIT_ASSERT(test, zt_long_div(2, LONG_MAX) == 2 / LONG_MAX);

        ZT_UNIT_ASSERT(test, zt_long_div(LONG_MIN, 2) == LONG_MIN / 2);
        ZT_UNIT_ASSERT(test, zt_long_div(2, LONG_MIN) == 2 / LONG_MIN);

        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.overflow, zt_long_div(LONG_MIN, -1));

        /* unsigned division */
        /* char */
        ZT_UNIT_ASSERT(test, zt_uchar_div(1, CHAR_MAX) == 0);
        ZT_UNIT_ASSERT(test, zt_uchar_div(CHAR_MAX, 1) == CHAR_MAX);

        ZT_UNIT_ASSERT(test, zt_uchar_div(0, CHAR_MAX) == 0);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.divide_by_zero, zt_uchar_div(CHAR_MAX, 0));

        ZT_UNIT_ASSERT(test, zt_uchar_div(CHAR_MAX, 2) == CHAR_MAX / 2);

        /* char */
        ZT_UNIT_ASSERT(test, zt_ushort_div(1, CHAR_MAX) == 0);
        ZT_UNIT_ASSERT(test, zt_ushort_div(CHAR_MAX, 1) == CHAR_MAX);

        ZT_UNIT_ASSERT(test, zt_ushort_div(0, CHAR_MAX) == 0);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.divide_by_zero, zt_ushort_div(CHAR_MAX, 0));

        ZT_UNIT_ASSERT(test, zt_ushort_div(CHAR_MAX, 2) == CHAR_MAX / 2);

        /* int */
        ZT_UNIT_ASSERT(test, zt_uint_div(1, INT_MAX) == 0);
        ZT_UNIT_ASSERT(test, zt_uint_div(INT_MAX, 1) == INT_MAX);

        ZT_UNIT_ASSERT(test, zt_uint_div(0, INT_MAX) == 0);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.divide_by_zero, zt_uint_div(INT_MAX, 0));

        ZT_UNIT_ASSERT(test, zt_uint_div(INT_MAX, 2) == INT_MAX / 2);
        
        /* long */
        ZT_UNIT_ASSERT(test, zt_ulong_div(1, LONG_MAX) == 0);
        ZT_UNIT_ASSERT(test, zt_ulong_div(LONG_MAX, 1) == LONG_MAX);

        ZT_UNIT_ASSERT(test, zt_ulong_div(0, LONG_MAX) == 0);
        ZT_UNIT_ASSERT_RAISES(test, zt_exception.math.divide_by_zero, zt_ulong_div(LONG_MAX, 0));

        ZT_UNIT_ASSERT(test, zt_ulong_div(LONG_MAX, 2) == LONG_MAX / 2);
        
}

int
register_int_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "int tests", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}
