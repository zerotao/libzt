#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <zt_internal.h>
#include <zt_assert.h>

#ifdef HAVE_SYSLOG_H
#  include <syslog.h>
#endif

#ifdef HAVE_STRING_H
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif

#include <stdarg.h>
#include <errno.h>
#include <limits.h>

#ifndef HAS_SYS_ERRLIST
extern char *sys_errlist[];
extern int   sys_nerr;
#endif

static char*
percent_m(char    *obuf, int len, char *ibuf)
{
    char * bp = obuf;
    char * cp = ibuf;

    while ((bp - obuf < len) && (*bp = *cp)) {
        if (*cp == '%' && cp[1] == 'm') {
            if (errno < sys_nerr && errno > 0) {
                strcpy(bp, sys_errlist[errno]);
            } else {
                sprintf(bp, "Unknown Error %d", errno);
            }
            bp += strlen(bp);
            cp += 2;
        } else {
            bp++;
            cp++;
        }
    }
    return obuf;
}

void
vsyslog(int severity, char *format, va_list ap)
{
    char * fbuf;
    char * obuf;
    char * ftmp;
    int    len;
    int    flen;
    int    olen;

    zt_assert(format);

    len = strlen(format);
    flen = len * 2;
    olen = flen * 3;

    fbuf = XCALLOC(char, flen + 1);
    obuf = XCALLOC(char, olen + 1);

    if ((ftmp = percent_m(fbuf, flen, format)) == NULL) {
        return;
    }

    vsprintf(obuf, ftmp, ap);
    syslog(severity, "%s", obuf);

    free(fbuf);
    free(obuf);
}
