#ifndef _ZT_WIN32_H_
#define _ZT_WIN32_H_

/* network functions */
#include <WinSock2.h>

/* random functions */
void srandom(unsigned int _Seed);
long int random(void);

/* string functions */
int snprintf(char * s, size_t n, const char * format, ...);
int strcasecmp(const char *s1, const char *s2);
char *index(const char *s, int c);
char *strtok_r(char *str, const char *delim, char **saveptr);

/* memory functions */
void *alloca(size_t size);

/* error function */
int strerror_r(int errnum, char *buf, size_t buflen);

/* process functions */
typedef unsigned long pid_t;
pid_t getpid(void);

/* system functions */
#define _SC_PAGESIZE -1
long sysconf( int name );

#if defined(_WIN64)
  typedef __int64 ssize_t;
#else
  typedef long ssize_t;
#endif

#ifdef HAVE_LONG_LONG
typedef long long ssize_t;
#else
typedef long int ssize_t;
#endif


#endif /* _ZT_WIN32_H_ */
