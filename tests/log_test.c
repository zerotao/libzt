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
#define DEBUG
#include <libzt/zt_log.h>
#include <libzt/zt_progname.h>
#include <libzt/zt_unit.h>

static void
basic_tests(struct zt_unit_test *test, void *data)
{
    char       position1[255];
    char       position2[255];
    /* zt_log_ty *lstderr = zt_log_stderr((ZT_LOG_WITH_PROGNAME | ZT_LOG_WITH_LEVEL));  */
    zt_log_ty *lfile = zt_log_file( "log.err",
                                    ZT_LOG_FILE_OVERWRITE,
                                    (ZT_LOG_EMU_SYSLOG | ZT_LOG_WITH_LEVEL));

    /*    zt_log_ty *lsyslog = zt_log_syslog(LOG_PID, LOG_DAEMON, ZT_LOG_RAW); */

    zt_progname("./unit_test", STRIP_DIR);

    zt_log_logger(lfile);
    zt_log_debug_logger(lfile);

    zt_log_set_opts(lfile, (ZT_LOG_RAW));
    zt_log_printf( zt_log_debug, "ZT_LOG_RAW" );

    zt_log_set_opts(lfile, (ZT_LOG_WITH_DATE));
    zt_log_printf( zt_log_debug, "ZT_LOG_WITH_DATE" );

    zt_log_set_opts(lfile, (ZT_LOG_WITH_LEVEL));
    zt_log_printf( zt_log_debug, "ZT_LOG_WITH_LEVEL" );

    zt_log_set_opts(lfile, (ZT_LOG_WITH_SYSNAME));
    zt_log_printf( zt_log_debug, "ZT_LOG_WITH_SYSNAME" );

    zt_log_set_opts(lfile, (ZT_LOG_WITH_SYSNAME | ZT_LOG_WITH_PROGNAME));
    zt_log_printf( zt_log_debug, "ZT_LOG_WITH_SYSNAME | ZT_LOG_WITH_PROGNAME" );

    zt_log_set_opts(lfile, (ZT_LOG_WITH_PROGNAME));
    zt_log_printf( zt_log_debug, "ZT_LOG_WITH_PROGNAME" );

    zt_log_set_opts(lfile, (ZT_LOG_WITH_PID));
    zt_log_printf( zt_log_debug, "ZT_LOG_WITH_PID" );

    zt_log_set_opts(lfile, (ZT_LOG_EMU_SYSLOG));
    zt_log_printf( zt_log_debug, "ZT_LOG_EMU_SYSLOG" );

    zt_log_set_opts(lfile, (ZT_LOG_RAW));

    /* these need to be on the same line for the test to work */
    sprintf(position1, "(%s:%d)", __FILE__, __LINE__); ZT_LOG_XDEBUG( "LOG_DEBUG" );
    sprintf(position2, "(%s:%d)", __FILE__, __LINE__); ZT_LOG_DEBUG_INFO(lfile), zt_log_lprintf(lfile, zt_log_debug, "lprintf with debugging");

    zt_log_close(lfile);

    {
        FILE *file = fopen("log.err", "r");
        int   items = 0;
        char  msg[255];
        char  level[255];
        char  sysname[255];
        char  progname[255];
        int   pid = 0;
        char  month[255];
        int   date = 0;
        char  time[255];
        ZT_UNIT_ASSERT(test, (file));
        items = fscanf(file, "%s\n", msg);
        ZT_UNIT_ASSERT(test, ((items == 1) &&
                              (!strcmp(msg, "ZT_LOG_RAW"))));
        items = fscanf(file, "%s %d %s : %s\n", month, &date, time, msg);
        ZT_UNIT_ASSERT(test, ((items == 4) &&
                              (!strcmp(msg, "ZT_LOG_WITH_DATE"))));
        items = fscanf(file, "%s %s\n", level, msg);
        ZT_UNIT_ASSERT(test, ((items == 2) &&
                              (!strcmp(msg, "ZT_LOG_WITH_LEVEL"))));

        items = fscanf(file, "%s %s\n", sysname, msg);
        ZT_UNIT_ASSERT(test, ((items == 2) &&
                              (!strcmp(msg, "ZT_LOG_WITH_SYSNAME"))));
        items = fscanf(file, "%s %s %[A-Z_| ]\n", sysname, progname, msg);
        ZT_UNIT_ASSERT(test, ((items == 3) &&
                              (!strcmp(msg, "ZT_LOG_WITH_SYSNAME | ZT_LOG_WITH_PROGNAME"))));

        items = fscanf(file, "%s %s\n", progname, msg);
        ZT_UNIT_ASSERT(test, ((items == 2) &&
                              (!strcmp(msg, "ZT_LOG_WITH_PROGNAME"))));
        items = fscanf(file, "[%d]: %s\n", &pid, msg);
        ZT_UNIT_ASSERT(test, ((items == 2) &&
                              (!strcmp(msg, "ZT_LOG_WITH_PID"))));
        items = fscanf(file, "%s %d %s %s %[a-zA-Z_-][%d]: %s\n", month, &date, time, sysname, progname, &pid, msg);
        ZT_UNIT_ASSERT(test, ((items == 7) &&
                              (!strcmp(msg, "ZT_LOG_EMU_SYSLOG"))));
        items = fscanf(file, "LOG_DEBUG: in basic_tests at %s\n", msg);
        ZT_UNIT_ASSERT(test, ((items == 1) &&
                              (!strcmp(msg, position1))));
        items = fscanf(file, "lprintf with debugging: in basic_tests at %s\n", msg);
        ZT_UNIT_ASSERT(test, ((items == 1) &&
                              (!strcmp(msg, position2))));
        fclose(file);
    }
} /* basic_tests */

int
register_log_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "logging tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return(0);
}
