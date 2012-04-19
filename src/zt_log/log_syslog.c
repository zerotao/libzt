/*
 * syslog.c        ZeroTao logging to syslog
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: syslog.c,v 1.3 2003/06/10 04:15:55 jshiffer Exp $
 *
 */

/*
 * Description:
 */

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif

#include "log_private.h"
#include "log_syslog.h"

#if HAVE_SYSLOG
static void
destructor(zt_log_ty * log) {
    closelog();
    zt_free(log);
    return;
}

static void
print(zt_log_ty * log, zt_log_level level, const char * file, int line, const char * function, const char * fmt, va_list ap) {
    char       * nfmt         = NULL;
    int          syslog_level = 0;
    unsigned int level_flag   = 0;

    switch (level) {
        case zt_log_emerg:
            syslog_level = LOG_EMERG;
            break;
        case zt_log_alert:
            syslog_level = LOG_ALERT;
            break;
        case zt_log_crit:
            syslog_level = LOG_CRIT;
            break;
        case zt_log_err:
            syslog_level = LOG_ERR;
            break;
        case zt_log_warning:
            syslog_level = LOG_WARNING;
            break;
        case zt_log_notice:
            syslog_level = LOG_NOTICE;
            break;
        case zt_log_info:
            syslog_level = LOG_INFO;
            break;
        case zt_log_debug:
            syslog_level = LOG_DEBUG;
            break;
        default:
            syslog_level = LOG_ERR;
            break;
    } /* switch */

    /* honor level flag if it is set in the logger opts. for the rest of the
     * zt flags, defer to syslog settings */
    level_flag = (log->opts & ZT_LOG_WITH_LEVEL) ? ZT_LOG_WITH_LEVEL : 0;
    nfmt       = zt_log_gen_fmt(log, fmt, file, line, function, level, level_flag);
    vsyslog(syslog_level, nfmt, ap);

    zt_free(nfmt);
} /* print */

/* component data */
static zt_log_vtbl_ty vtbl = {
    sizeof(zt_log_ty),
    destructor,
    print,
};
#endif /* HAVE_SYSLOG */

zt_log_ty *
zt_log_syslog(void) {
    return zt_log_syslog2(ZT_LOG_WITH_PID, LOG_USER);
}

zt_log_ty *
zt_log_syslog2(int opt, int facility) {
#if HAVE_SYSLOG
    char * name    = zt_progname(0, 0);
    int    sysopts = 0;

    if (opt & ZT_LOG_WITH_PID) {
        sysopts |= LOG_PID;
    }
    openlog(name ? name : "Set name with progname call", sysopts, facility);
    return zt_log_new(&vtbl, 0);
#else /* if HAVE_SYSLOG */
    zt_log_ty * logger = zt_log_stderr(ZT_LOG_EMU_SYSLOG);

    zt_log_printf(zt_log_crit, "Syslog not supported on this platform: falling back to zt_log_stderr");
    return logger;
#endif /* HAVE_SYSLOG */
}