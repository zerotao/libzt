

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

#include "zt_win32.h"

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



/*-
 * Copyright (c) 1990, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *  This product includes software developed by the University of
 *  California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

char *
strsep (char **stringp,
     const char *delim)
{
    register char *s;
    register const char *spanp;
    register int c, sc;
    char *tok;

    if ((s = *stringp) == NULL)
        return (NULL);
    for (tok = s;;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } while (sc != 0);
    }
    /* NOTREACHED */
}
