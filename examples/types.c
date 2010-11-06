#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STRING_H
# include <string.h>
#endif /* HAVE_STRING_H */

#ifdef HAVE_STDINT_H
# include <stdint.h>
#endif

#include <zt_internal.h>
#include "test.h"


int
main(int argc UNUSED, char *argv[] UNUSED) {
    uint8_t           c8 = 0xFF;
    unsigned char      uc8 = 0xFF;

    uint16_t          c16 = 0xFFFF;
    unsigned short     s16 = 0xFFFF;

    uint32_t          c32 = 0xFFFFFFFFL;
    unsigned long      l32 = 0xFFFFFFFFL;

    uint64_t          c64 = 0xFFFFFFFFFFFFFFFFLL;
    unsigned long long ll64 = 0xFFFFFFFFFFFFFFFFLL;

    printf("uint8_t   0x%hX\n", c8);
    printf("uc8  0x%hX\n", uc8);

    printf("c16  0x%hX\n", c16);
    printf("s16  0x%hX\n", s16);

    printf("c32  0x%X\n", c32);
    printf("l32  0x%lX\n", l32);

    printf("c64  0x%llX\n", c64);
    printf("ll64 0x%llX\n", ll64);


    return 0;
}
