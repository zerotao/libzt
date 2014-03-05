#ifndef _ZT_RANDOM_H_
#define _ZT_RANDOM_H_

#include <zt.h>
BEGIN_C_DECLS

#define ZT_RANDOM_RECURRENCE 624

typedef struct zt_random_state_t {
    uint32_t      mt[ZT_RANDOM_RECURRENCE];
    uint32_t      mti;
    uint32_t      initialized;
} zt_random_state;

void zt_random_initstate(zt_random_state * state, uint32_t seed[], size_t len);
void zt_random_srandom(zt_random_state * state, uint32_t seed);

/* generates a random number on [0,0xffffffff]-interval */
uint32_t zt_random_uint32(zt_random_state * state);

/* generates a random number on [0,1]-real-interval */
#define zt_random_float(state) (zt_random_uint32(state)*(1.0/4294967295.0))

/* generates a random number on (0,1)-real-interval */
#define zt_random_float00(state) ((zt_random_uint32(state) + 0.5)*(1.0/4294967296.0))

/* generates a random number on [0,1)-real-interval */
#define zt_random_float10(state) (zt_random_uint32(state)*(1.0/4294967296.0))


END_C_DECLS
#endif /* _ZT_RANDOM_H_ */
