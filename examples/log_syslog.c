/*
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: zt_log_test.c,v 1.7 2003/11/26 15:45:10 jshiffer Exp $
 *
 */

/*
 * Description:
 */

#include <string.h>
#define ZT_WITH_UNIT
#include <zt.h>

int
main(int argc, char **argv) {
    char          position1[255];
    char          position2[255];
    zt_log_ty   * logger;
    zt_log_ty   * lsyslog;

    /* set the progname before calling syslog */
    zt_progname(argv[0], STRIP_DIR);

    lsyslog = zt_log_syslog();

    logger = zt_log_logger(lsyslog);
    zt_log_debug_logger(lsyslog);
    zt_log_set_level(lsyslog, zt_log_debug);

    zt_log_printf( zt_log_emerg, "*emergency* message" );
    zt_log_printf( zt_log_alert, "*alert* message" );
    zt_log_printf( zt_log_crit, "*critical* message" );
    zt_log_printf( zt_log_err, "*error* message" );
    zt_log_printf( zt_log_warning, "*warning* message" );
    zt_log_printf( zt_log_notice, "*notice* message" );
    zt_log_printf( zt_log_info, "*info* message" );
    zt_log_printf( zt_log_debug, "*debug* message" );

    zt_log_set_opts(lsyslog, (ZT_LOG_RAW));

    /* these need to be on the same line for the test to work */
    sprintf(position1, "(%s:%d)", __FILE__, __LINE__); ZT_LOG_XDEBUG( "LOG_DEBUG" );
    sprintf(position2, "(%s:%d)", __FILE__, __LINE__); ZT_LOG_DEBUG_INFO(lsyslog), zt_log_lprintf(lsyslog, zt_log_debug, "lprintf with debugging");

    zt_log_close(lsyslog);
    zt_log_logger(logger);

    return 0;
} /* basic_tests */

