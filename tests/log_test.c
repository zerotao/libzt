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

static void
basic_tests(struct zt_unit_test *test, void *data UNUSED)
{
    char          position1[255];
    char          position2[255];
    zt_log_ty   * logger;
    /* zt_log_ty *lstderr = zt_log_stderr((ZT_LOG_WITH_PROGNAME | ZT_LOG_WITH_LEVEL));  */
    zt_log_ty   * lfile = zt_log_file( "log.err",
                                      ZT_LOG_FILE_OVERWRITE,
                                      (ZT_LOG_EMU_SYSLOG | ZT_LOG_WITH_LEVEL));

    /*    zt_log_ty *lsyslog = zt_log_syslog(LOG_PID, LOG_DAEMON, ZT_LOG_RAW); */

    zt_progname("."PATH_SEPERATOR"unit_test", STRIP_DIR);

    /* capture the standard logger and set to the new logger */
    logger = zt_log_logger(lfile);

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
    sprintf(position2, "(%s:%d)", __FILE__, __LINE__); ZT_LOG_DEBUG_INFO(), zt_log_lprintf(lfile, zt_log_debug, "lprintf with debugging");

    /* reset to the default logger */
    zt_log_logger(logger);

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
#ifdef DEBUG
        items = fscanf(file, "LOG_DEBUG: in basic_tests at %s\n", msg);
        ZT_UNIT_ASSERT(test, ((items == 1) &&
                              (!strcmp(msg, position1))));
#endif

#if 0
        /* issues with paths with spaces in them */
        items = fscanf(file, "lprintf with debugging: in basic_tests at %s\n", msg);
        ZT_UNIT_ASSERT(test, ((items == 1) &&
                              (!strcmp(msg, position2))));
#endif
        fclose(file);
        /* unlink("log.err"); */
    }
} /* basic_tests */

#ifdef HAVE_PTHREADS
#include <pthread.h>
static int _count1 = 0;
static int _count2 = 0;

void *
log_1(void * p) {
    const char * file1;
    const char * func1;
    int          line1;
    const char * file2;
    const char * func2;
    int          line2;

    ZT_LOG_DEBUG_INFO(), file1 = __FILE__, line1 = __LINE__, func1 = __FUNCTION__;
    zt_log_get_debug_info( &file2, &line2, &func2);
    zt_log_printf(zt_log_err, "test1");

    if ((strcmp(file1, file2) == 0) &&
        (strcmp(func1, func2) == 0) &&
        line1 == line2) {
        _count1++;
    }

    return NULL;
}

void *
log_2(void * p) {
    pthread_t  * pt = p;
    const char * file1;
    const char * func1;
    int          line1;
    const char * file2;
    const char * func2;
    int          line2;

    ZT_LOG_DEBUG_INFO(), file1 = __FILE__, line1 = __LINE__, func1 = __FUNCTION__;
    pthread_join(*pt, NULL);
    zt_log_get_debug_info(&file2, &line2, &func2);
    zt_log_printf(zt_log_err, "test2");

    if ((strcmp(file1, file2) == 0) &&
        (strcmp(func1, func2) == 0) &&
        line1 == line2) {
        _count2++;
    }

    return NULL;
}

void
threaded_tests(struct zt_unit_test *test, void *data UNUSED) {
    pthread_t   t1;
    pthread_t   t2;
    int         i;
    zt_log_ty   * lfile;
    zt_log_ty   * ologger;

    lfile = zt_log_file( "test.log", ZT_LOG_FILE_APPEND, 0);
    ologger = zt_log_logger(lfile);

    for(i = 0; i < 100; i++) {
        pthread_create(&t1, NULL, log_1, NULL);
        pthread_create(&t2, NULL, log_2, &t1);

        pthread_join(t2, NULL);
    }

    ZT_UNIT_ASSERT(test, _count1 == 100);
    ZT_UNIT_ASSERT(test, _count2 == 100);

    zt_log_logger(ologger);
    zt_log_close(lfile);

    unlink("test.log");
}
#endif /* HAVE_PTHREADS */

int
register_log_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "logging tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
#ifdef HAVE_PTHREADS
    zt_unit_register_test(suite, "threaded", threaded_tests);
#endif /* HAVE_PTHREADS */

    return 0;
}
