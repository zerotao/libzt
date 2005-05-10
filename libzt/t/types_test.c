#include <libzt/zt.h>
#include "test.h"


int
main(int argc, char *argv[]) 
{
	u_int8_t	  c8 = 0xFF;
	unsigned  char uc8 = 0xFF;
		
	u_int16_t	  c16 = 0xFFFF;
	unsigned short s16 = 0xFFFF;
	
	u_int32_t	  c32 = 0xFFFFFFFFL;
	unsigned long l32 = 0xFFFFFFFFL;
	
	u_int64_t	  c64 = 0xFFFFFFFFFFFFFFFFLL;
	unsigned long long ll64 = 0xFFFFFFFFFFFFFFFFLL;

	printf("u_int8_t   0x%hhX\n", c8);
	printf("uc8  0x%hhX\n", uc8);
	
	printf("c16  0x%hX\n", c16);
	printf("s16  0x%hX\n", s16);
	
	printf("c32  0x%X\n", c32);
	printf("l32  0x%lX\n", l32);
	
	printf("c64  0x%llX\n", c64);
	printf("ll64 0x%llX\n", ll64);
	

	return 0;
}
