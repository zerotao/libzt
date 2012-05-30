/*
 * private.c        ZeroTao private definitions for logging
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: zt_log_private.c,v 1.4 2003/11/26 17:47:29 jshiffer Exp $
 *
 */

/*
 * Description:
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <time.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "../zt_internal.h"
#include "../zt_progname.h"
#include "log_private.h"

zt_log_level_desc_ty zt_log_level_desc[] = {
    { zt_log_emerg,   "Emergency" },
    { zt_log_alert,   "Alert"     },
    { zt_log_crit,    "Critical"  },
    { zt_log_err,     "Error"     },
    { zt_log_warning, "Warning"   },
    { zt_log_notice,  "Notice"    },
    { zt_log_info,    "Info"      },
    { zt_log_debug,   "Debug"     },
    { -1,             NULL        },
};

zt_log_ty *
zt_log_new(zt_log_vtbl_ty * vptr, unsigned int opts) {
    zt_log_ty * result;

    result        = zt_callocs(vptr->size, 1);
    if (!result) {
        fprintf(stderr, "Unable to calloc memory for log vtable\n");
        exit(1);
    }
    result->vtbl  = vptr;
    result->opts  = opts;
    result->level = zt_log_max;

    return result;
}

char*
zt_log_gen_fmt(zt_log_ty * log, const char * fmt, const char * file, int line, const char * function, zt_log_level level, unsigned int opts) {
    size_t len  = 0;
    char * buff = NULL;

    len  = strlen(fmt) + 4;   /* format + seperator + "\n" + null */
    buff = (char*)zt_calloc(char, len);  /* mem_calloc(len, sizeof(char)); */

    if (opts & ZT_LOG_WITH_DATE) {
        char   sbuf[255];
        time_t tt = time(NULL);

        len += strftime(sbuf, 254, "%b %d %H:%M:%S ", localtime(&tt));

        buff = zt_realloc(char, buff, len);
        sprintf(buff, "%s", sbuf);
    }

    if (opts & ZT_LOG_WITH_SYSNAME) {
        char   sbuf[255];
        char * t = NULL;

        if (gethostname(sbuf, 254) == -1) {
            sprintf(sbuf, "*hostname*");
        }
        t    = strchr(sbuf, '.');
        if (t) {
            *t = '\0';
        }
        len += strlen(sbuf) + 1;    /* sbuf + space */
        buff = zt_realloc(char, buff, len);
        strcat(buff, sbuf);
        if ((opts > ZT_LOG_WITH_SYSNAME)) {
            strcat(buff, " ");
        }
    }

    if (opts & ZT_LOG_WITH_PROGNAME) {
        len += strlen(zt_progname(NULL, 0)); /* progname */
        buff = zt_realloc(char, buff, len);
        strcat(buff, zt_progname(NULL, 0));
    }

    if (opts & ZT_LOG_WITH_PID) {
        char  sbuf[10 + 3];       /* pid + [] + null */
        pid_t pid = getpid();

        sprintf(sbuf, "[%u]", pid);
        len += strlen(sbuf);
        buff = zt_realloc(char, buff, len);
        strcat(buff, sbuf);
    }

    if (opts & ZT_LOG_WITH_LEVEL) {
        if ((level < zt_log_max) && ((int)level >= zt_log_emerg)) {
            len += strlen(zt_log_level_desc[level].desc) + 2; /* ': ' + level desc */
            buff = zt_realloc(char, buff, len);
            if (opts != ZT_LOG_WITH_LEVEL) {
                strcat(buff, ": ");
            }
            strcat(buff, zt_log_level_desc[level].desc);
        }
    }
    if (opts != ZT_LOG_RAW) {
        strcat(buff, ": ");
    }
    strcat(buff, fmt);
    if (((file) && (line > -1) && (function))) {
        char * nbuff = NULL;
        size_t nlen  = 13;    /* ': in  at (:)' + null */

        nlen += strlen(file);
        nlen += strlen(function);
        nlen += 10;    /* length of a int to str */
        nbuff = zt_calloc(char, nlen); /* mem_calloc(nlen, sizeof(char)); */
        sprintf(nbuff, ": in %s at (%s:%d)", function, file, line);
        len  += nlen;
        buff  = zt_realloc(char, buff, len);
        strcat(buff, nbuff);
        zt_free(nbuff);
    }
    strcat(buff, "\n");
    return buff;
} /* zt_log_gen_fmt */