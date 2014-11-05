/*
 * =====================================================================================
 *
 *       Filename:  zt_random.c
 *
 *    Description:  Mersenne Twister based on http://www.math.sci.hiroshima-u.ac.jp/~%20m-mat/MT/MT2002/CODES/mt19937ar.c
 *
 *        Version:  1.0
 *        Created:  03/05/2014 13:21:01
 *
 *         Author:  Jason L. Shiffer (jshiffer@zerotao.org)
 *
 * =====================================================================================
 */
#include "zt_random.h"

#define RECURRENCE 624
#define MIDDLE_WORD 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

#define INITIALIZED 0xD15EA5E


void
zt_random_initstate(zt_random_state * state, uint32_t seed[], size_t len) {
    /* initialize by an array with array-length
     * seed is the array for initializing keys
     * len is it the seed array length
     * slight change for C++, 2004/2/26
     */

    size_t   i = 1;
    size_t   j = 0;
    size_t   k = (RECURRENCE > len ? RECURRENCE : len);

    register uint32_t * mt = state->mt;

    zt_random_srandom(state, 19650218UL);

    for(; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL)) + seed[j] + (uint32_t)j; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */

        i++;
        j++;

        if (i>=RECURRENCE) {
            mt[0] = mt[RECURRENCE-1];
            i=1;
        }

        if (j>=len) j=0;
    }

    for (k=RECURRENCE-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL)) - (uint32_t)i; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */

        i++;

        if (i>=RECURRENCE) {
            mt[0] = mt[RECURRENCE-1];
            i=1;
        }
    }

    mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
}

void
zt_random_srandom(zt_random_state * state, uint32_t seed) {
    register uint32_t     mti = state->mti;

    state->mt[0] = seed & 0xffffffffUL;
    state->initialized = INITIALIZED;

    for (mti=1; mti < RECURRENCE; mti++) {
        state->mt[mti] = (1812433253UL * (state->mt[mti-1] ^ (state->mt[mti-1] >> 30)) + mti);
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier.
         * In the previous versions, MSBs of the seed affect
         * only MSBs of the array mt[].
         * 2002/01/09 modified by Makoto Matsumoto
         */

        state->mt[mti] &= 0xffffffffUL; /* for >32 bit machines */
    }

    state->mti = mti;
}

uint32_t
zt_random_uint32(zt_random_state * state) {
    register uint32_t       * mt = state->mt;
    uint32_t                  y;
    static uint32_t           mag01[2] = {0x0UL, MATRIX_A}; /* mag01[x] = x * MATRIX_A for x=0,1 */

    if (state->initialized != INITIALIZED) {
        zt_log_printf(zt_log_alert, "zt_random_uint32 called without initialization, seeding with time(NULL)");
        zt_random_srandom(state, (uint32_t)time(NULL));
    }

    if (state->mti >= RECURRENCE) { /* generate RECURRENCE words at one time */
        int   kk;

        for (kk=0; kk < RECURRENCE - MIDDLE_WORD; kk++) {
            y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
            mt[kk] = mt[kk + MIDDLE_WORD] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }

        for (; kk < RECURRENCE-1; kk++) {
            y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
            mt[kk] = mt[kk + (MIDDLE_WORD - RECURRENCE)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }

        y = (mt[RECURRENCE - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
        mt[RECURRENCE - 1] = mt[MIDDLE_WORD - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        state->mti = 0;
    }

    y = mt[state->mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}
