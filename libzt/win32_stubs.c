

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

#include "win32_stubs.h"

void srandom(unsigned int _Seed)
{
    srand(_Seed);
}

long int random(void)
{
    return rand();
}

int snprintf(char * s, size_t n, const char * format, ...)
{
    int oVal = -1;
    va_list args;
    va_start(args, format);
    oVal = vsprintf_s(s, n, format, args);
    va_end(args);
    return oVal;
}

int strcasecmp(const char *s1, const char *s2)
{
    return _stricmp(s1, s2);
}

char *index(const char *s, int c)
{
    return strchr(s,c);
}

char *strtok_r(char *str, const char *delim, char **saveptr)
{
    return strtok_s(str, delim, saveptr);
}

void *alloca(size_t size)
{
    return _alloca(size);
}


int strerror_r(int errnum, char *buf, size_t buflen)
{
    return strerror_s(buf, buflen, errnum);
}

unsigned long getpid(void)
{
    return GetCurrentProcessId();
}

long sysconf(int name)
{
    long oVal = -1;

    if (name == _SC_PAGESIZE)
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        oVal = si.dwPageSize;
    }

    return oVal;
}
