#include <stdio.h>

#include <libzt/zt.h>
#include <libzt/zt_hash.h>

#include "test.h"

/* used for timings */
void driver1()
{
	unsigned char buf[256];
	unsigned long i;
	unsigned long h=0;

	for (i=0; i<256; ++i) 
	{

		h = zt_hash(buf,i,h);
	}
}

/* check that every input bit changes every output bit half the time */
#define HASHSTATE 1
#define HASHLEN   1
#define MAXPAIR 80
#define MAXLEN 70
void driver2()
{
	unsigned char qa[MAXLEN+1], qb[MAXLEN+2], *a = &qa[0], *b = &qb[1];

	unsigned long c[HASHSTATE], d[HASHSTATE], i, j=0, k, l, m=0, z;
	unsigned long e[HASHSTATE],f[HASHSTATE],g[HASHSTATE],h[HASHSTATE];
	unsigned long x[HASHSTATE],y[HASHSTATE];
	unsigned long hlen;

	printf("No more than %d trials should ever be needed \n",MAXPAIR/2);
	for (hlen=0; hlen < MAXLEN; ++hlen)
	{
		z=0;
		for (i=0; i<hlen; ++i)  /*----------------------- for each input byte, */
		{
			for (j=0; j<8; ++j)   /*------------------------ for each input bit, */
			{
				for (m=1; m<8; ++m) /*------------ for serveral possible initvals, */
				{
					for (l=0; l<HASHSTATE; ++l) e[l]=f[l]=g[l]=h[l]=x[l]=y[l]=~((unsigned long)0);

					/*---- check that every output bit is affected by that input bit */
					for (k=0; k<MAXPAIR; k+=2)
					{ 
						unsigned long finished=1;
						/* keys have one bit different */
						for (l=0; l<hlen+1; ++l) {a[l] = b[l] = (unsigned char)0;}
						/* have a and b be two keys differing in only one bit */
						a[i] ^= (k<<j);
						a[i] ^= (k>>(8-j));
						c[0] = zt_hash(a, hlen, m);
						b[i] ^= ((k+1)<<j);
						b[i] ^= ((k+1)>>(8-j));
						d[0] = zt_hash(b, hlen, m);
						/* check every bit is 1, 0, set, and not set at least once */
						for (l=0; l<HASHSTATE; ++l)
						{
							e[l] &= (c[l]^d[l]);
							f[l] &= ~(c[l]^d[l]);
							g[l] &= c[l];
							h[l] &= ~c[l];
							x[l] &= d[l];
							y[l] &= ~d[l];
							if (e[l]|f[l]|g[l]|h[l]|x[l]|y[l]) finished=0;
						}
						if (finished) break;
					}
					if (k>z) z=k;
					if (k==MAXPAIR) 
					{
						printf("Some bit didn't change: ");
						printf("%.8lx %.8lx %.8lx %.8lx %.8lx %.8lx  ",
						       (long unsigned int)e[0],(long unsigned int)f[0],(long unsigned int)g[0],(long unsigned int)h[0],(long unsigned int)x[0],(long unsigned int)y[0]);
						printf("i %ld j %ld m %ld len %ld\n",
						       (long unsigned int)i,(long unsigned int)j,(long unsigned int)m,(long unsigned int)hlen);
					}
					if (z==MAXPAIR) goto done;
				}
			}
		}
	done:
		if (z < MAXPAIR)
		{
			printf("Mix success  %2ld bytes  %2ld initvals  ",(long int)i, (long int)(m));
			printf("required  %ld  trials\n",(long int)z/2);
		}
	}
	printf("\n");
}

/* Check for reading beyond the end of the buffer and alignment problems */
void driver3()
{
	unsigned char buf[MAXLEN+20], *b;
	unsigned long len;
	unsigned char q[] = "This is the time for all good men to come to the aid of their country";
	unsigned char qq[] = "xThis is the time for all good men to come to the aid of their country";
	unsigned char qqq[] = "xxThis is the time for all good men to come to the aid of their country";
	unsigned char qqqq[] = "xxxThis is the time for all good men to come to the aid of their country";
	unsigned long h,i,j,ref,x,y;

	printf("Endianness.  These should all be the same:\n");
	printf("%.8lx\n", (unsigned long int)zt_hash(q, sizeof(q)-1, (unsigned long)0));
	printf("%.8lx\n", (unsigned long int)zt_hash(qq+1, sizeof(q)-1, (unsigned long)0));
	printf("%.8lx\n", (unsigned long int)zt_hash(qqq+2, sizeof(q)-1, (unsigned long)0));
	printf("%.8lx\n", (unsigned long int)zt_hash(qqqq+3, sizeof(q)-1, (unsigned long)0));
	printf("\n");
	for (h=0, b=buf+1; h<8; ++h, ++b)
	{
		for (i=0; i<MAXLEN; ++i)
		{
			len = i;
			for (j=0; j<i; ++j) *(b+j)=0;

			/* these should all be equal */
			ref = zt_hash(b, len, (unsigned long)1);
			*(b+i)=(unsigned char)~0;
			*(b-1)=(unsigned char)~0;
			x = zt_hash(b, len, (unsigned long)1);
			y = zt_hash(b, len, (unsigned long)1);
			if ((ref != x) || (ref != y)) 
			{
				printf("alignment error: %.8lx %.8lx %.8lx %ld %ld\n",
				       (long int)ref, (unsigned long int)x, (unsigned long int)y, \
				       (unsigned long int)h, (unsigned long int)i);
			}
		}
	}
}

/* check for problems with nulls */
void driver4()
{
	unsigned char buf[1];
	unsigned long h,i,state[HASHSTATE];


	buf[0] = ~0;
	for (i=0; i<HASHSTATE; ++i) state[i] = 1;
	printf("These should all be different\n");
	for (i=0, h=0; i<8; ++i)
	{
		h = zt_hash(buf, (unsigned long)0, h);

		printf("%2ld  0-byte strings, hash is  %.8lx\n", (unsigned long) i, (unsigned long)h);
	}
}


int
main(int argc, char *argv[])
{
	driver1();   /* test that the key is hashed: used for timings */
	driver2();   /* test that whole key is hashed thoroughly */
	driver3();   /* test that nothing but the key is hashed */
	driver4();   /* test hashing multiple buffers (all buffers are null) */
	return 1;
}
	
