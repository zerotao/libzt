#include <zt_int.h>
#include "test.h"

int
main(int argc, char *argv[]) 
{
        unsigned int    a = 22;
        unsigned int    b = 44;
        unsigned int    c = 0;

        
        c = uint_add(a, b);

        /* mixed sign addition */
        TEST("int_add", int_add(-128, 0) == -128);
        TEST("int_add", int_add(127, -128) == -1);
        
        
        printf("%d\n", c);
        
}
