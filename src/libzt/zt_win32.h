#ifndef _ZT_WIN32_H_
#define _ZT_WIN32_H_

/* network functions */
#include <WinSock2.h>
#include <errno.h>
#include <fcntl.h>
#include <direct.h>
#include <tchar.h>
#include <io.h>
#include <process.h>

#include <libzt/zt_config.h>

#ifndef __cplusplus
  typedef int bool;
  #define true 1
  #define false 0
#endif /* __cplusplus */

/* 4996 = The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _*. See online help for details. */
#pragma warning( disable : 4996 )

/* random functions */
void srandom(unsigned int _Seed);
long int random(void);

/* string functions */
int snprintf(char * s, size_t n, const char * format, ...);
//#define snprintf _snprintf
int strcasecmp(const char *s1, const char *s2);
//#define strcasecmp _stricmp

char *strtok_r(char *str, const char *delim, char **saveptr);

/* error function */
int strerror_r(int errnum, char *buf, size_t buflen);

/* process functions */

typedef unsigned long pid_t;

/* system functions */
#define _SC_PAGESIZE -1
long sysconf( int name );

#if defined(_WIN64)
  typedef __int64 ssize_t;
#else
  typedef long ssize_t;
#endif

char * strsep (char **stringp, const char *delim);

#endif /* _ZT_WIN32_H_ */
