#include <libzt/zt.h>
#include <libzt/zt_chash.h>

#include "test.h"

/* used for timings */
void driver1()
{
	unsigned char buf[256];
	unsigned long long i;
	unsigned long long h=0;

	for (i=0; i<256; ++i) 
	{
		h = zt_chash64(buf,i,h);
	}
}

/* check that every input bit changes every output bit half the time */
#define HASHSTATE 1
#define HASHLEN   1
#define MAXPAIR 80
#define MAXLEN 5
void driver2()
{
	unsigned char qa[MAXLEN+1], qb[MAXLEN+2], *a = &qa[0], *b = &qb[1];
	unsigned long long c[HASHSTATE], d[HASHSTATE], i, j=0, k, l, m=0, z;
	unsigned long long e[HASHSTATE],f[HASHSTATE],g[HASHSTATE],h[HASHSTATE];
	unsigned long long x[HASHSTATE],y[HASHSTATE];
	unsigned long long hlen;

	printf("No more than %d trials should ever be needed \n",MAXPAIR/2);
	for (hlen=0; hlen < MAXLEN; ++hlen)
	{
		z=0;
		for (i=0; i<hlen; ++i)  /*----------------------- for each byte, */
		{
			for (j=0; j<8; ++j)   /*------------------------ for each bit, */
			{
				for (m=0; m<8; ++m) /*-------- for serveral possible levels, */
				{
					for (l=0; l<HASHSTATE; ++l) e[l]=f[l]=g[l]=h[l]=x[l]=y[l]=~((unsigned long long)0);

					/*---- check that every input bit affects every output bit */
					for (k=0; k<MAXPAIR; k+=2)
					{ 
						unsigned long long finished=1;
						/* keys have one bit different */
						for (l=0; l<hlen+1; ++l) {a[l] = b[l] = (unsigned char)0;}
						/* have a and b be two keys differing in only one bit */
						a[i] ^= (k<<j);
						a[i] ^= (k>>(8-j));
						c[0] = zt_chash64(a, hlen, m);
						b[i] ^= ((k+1)<<j);
						b[i] ^= ((k+1)>>(8-j));
						d[0] = zt_chash64(b, hlen, m);
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
						       (unsigned long)e[0],(unsigned long)f[0],(unsigned long)g[0],(unsigned long)h[0],(unsigned long)x[0],(unsigned long)y[0]);
						printf("i %ld j %ld m %ld len %ld\n",
						       (unsigned long)i,(unsigned long)j,(unsigned long)m,(unsigned long)hlen);
					}
					if (z==MAXPAIR) goto done;
				}
			}
		}
	done:
		if (z < MAXPAIR)
		{
			printf("Mix success  %2ld bytes  %2ld levels  ",(unsigned long)i,(unsigned long)m);
			printf("required  %ld  trials\n",(unsigned long)(z/2));
		}
	}
	printf("\n");
}

/* Check for reading beyond the end of the buffer and alignment problems */
void driver3()
{
	unsigned char buf[MAXLEN+20], *b;
	unsigned long long len;
	unsigned char q[] = "This is the time for all good men to come to the aid of their country";
	unsigned char qq[] = "xThis is the time for all good men to come to the aid of their country";
	unsigned char qqq[] = "xxThis is the time for all good men to come to the aid of their country";
	unsigned char qqqq[] = "xxxThis is the time for all good men to come to the aid of their country";
	unsigned char o[] = "xxxxThis is the time for all good men to come to the aid of their country";
	unsigned char oo[] = "xxxxxThis is the time for all good men to come to the aid of their country";
	unsigned char ooo[] = "xxxxxxThis is the time for all good men to come to the aid of their country";
	unsigned char oooo[] = "xxxxxxxThis is the time for all good men to come to the aid of their country";
	unsigned long long h,i,j,ref,x,y;

	printf("Endianness.  These should all be the same:\n");
	h = zt_chash64(q+0, (unsigned long long)(sizeof(q)-1), (unsigned long long)0);
	printf("%.8lx%.8lx\n", (unsigned long)h, (unsigned long)(h>>32));
	h = zt_chash64(qq+1, (unsigned long long)(sizeof(q)-1), (unsigned long long)0);
	printf("%.8lx%.8lx\n", (unsigned long)h, (unsigned long)(h>>32));
	h = zt_chash64(qqq+2, (unsigned long long)(sizeof(q)-1), (unsigned long long)0);
	printf("%.8lx%.8lx\n", (unsigned long)h, (unsigned long)(h>>32));
	h = zt_chash64(qqqq+3, (unsigned long long)(sizeof(q)-1), (unsigned long long)0);
	printf("%.8lx%.8lx\n", (unsigned long)h, (unsigned long)(h>>32));
	h = zt_chash64(o+4, (unsigned long long)(sizeof(q)-1), (unsigned long long)0);
	printf("%.8lx%.8lx\n", (unsigned long)h, (unsigned long)(h>>32));
	h = zt_chash64(oo+5, (unsigned long long)(sizeof(q)-1), (unsigned long long)0);
	printf("%.8lx%.8lx\n", (unsigned long)h, (unsigned long)(h>>32));
	h = zt_chash64(ooo+6, (unsigned long long)(sizeof(q)-1), (unsigned long long)0);
	printf("%.8lx%.8lx\n", (unsigned long)h, (unsigned long)(h>>32));
	h = zt_chash64(oooo+7, (unsigned long long)(sizeof(q)-1), (unsigned long long)0);
	printf("%.8lx%.8lx\n", (unsigned long)h, (unsigned long)(h>>32));
	printf("\n");
	for (h=0, b=buf+1; h<8; ++h, ++b)
	{
		for (i=0; i<MAXLEN; ++i)
		{
			len = i;
			for (j=0; j<i; ++j) *(b+j)=0;

			/* these should all be equal */
			ref = zt_chash64(b, len, (unsigned long long)1);
			*(b+i)=(unsigned char)~0;
			*(b-1)=(unsigned char)~0;
			x = zt_chash64(b, len, (unsigned long long)1);
			y = zt_chash64(b, len, (unsigned long long)1);
			if ((ref != x) || (ref != y)) 
			{
				printf("alignment error: %.8lx %.8lx %.8lx %ld %ld\n",
				       (unsigned long)ref,(unsigned long)x,(unsigned long)y,(unsigned long)h,(unsigned long)i);
			}
		}
	}
}

/* check for problems with nulls */
void driver4()
{
	unsigned char buf[1];
	unsigned long long h,i,state[HASHSTATE];


	buf[0] = ~0;
	for (i=0; i<HASHSTATE; ++i) state[i] = 1;
	printf("These should all be different\n");
	for (i=0, h=0; i<8; ++i)
	{
		h = zt_chash64(buf, (unsigned long long)0, h);
		printf("%2ld  0-byte strings, hash is  %.8lx%.8lx\n", (unsigned long)i,
		       (unsigned long)h,(unsigned long)(h>>32));
	}
}


int main()
{
	driver1();   /* test that the key is hashed: used for timings */
	driver2();   /* test that whole key is hashed thoroughly */
	driver3();   /* test that nothing but the key is hashed */
	driver4();   /* test hashing multiple buffers (all buffers are null) */
	return 1;
}
