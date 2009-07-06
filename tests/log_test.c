/*
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: log_test.c,v 1.7 2003/11/26 15:45:10 jshiffer Exp $
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
	char position1[255];
	char position2[255];
	/* log_ty *lstderr = log_stderr((LOG_WITH_PROGNAME | LOG_WITH_LEVEL));  */
	log_ty *lfile = log_file( "log.err",  
							  LOG_FILE_OVERWRITE,  
							  (LOG_EMU_SYSLOG | LOG_WITH_LEVEL)); 
	/*    log_ty *lsyslog = log_syslog(LOG_PID, LOG_DAEMON, LOG_RAW); */

	zt_progname("./unit_test", STRIP_DIR); 

	log_logger(lfile);
	log_debug_logger(lfile);

	log_set_opts(lfile, (LOG_RAW));
	log_printf( log_debug, "LOG_RAW" );

	log_set_opts(lfile, (LOG_WITH_DATE));
	log_printf( log_debug, "LOG_WITH_DATE" );

	log_set_opts(lfile, (LOG_WITH_LEVEL));
	log_printf( log_debug, "LOG_WITH_LEVEL" );

	log_set_opts(lfile, (LOG_WITH_SYSNAME));
	log_printf( log_debug, "LOG_WITH_SYSNAME" );

	log_set_opts(lfile, (LOG_WITH_SYSNAME | LOG_WITH_PROGNAME));
	log_printf( log_debug, "LOG_WITH_SYSNAME | LOG_WITH_PROGNAME" );

	log_set_opts(lfile, (LOG_WITH_PROGNAME));
	log_printf( log_debug, "LOG_WITH_PROGNAME" );

	log_set_opts(lfile, (LOG_WITH_PID));
	log_printf( log_debug, "LOG_WITH_PID" );

	log_set_opts(lfile, (LOG_EMU_SYSLOG));
	log_printf( log_debug, "LOG_EMU_SYSLOG" );

	log_set_opts(lfile, (LOG_RAW));

	/* these need to be on the same line for the test to work */
	sprintf(position1, "(%s:%d)", __FILE__, __LINE__); LOG_XDEBUG( "LOG_DEBUG" );
	sprintf(position2, "(%s:%d)", __FILE__, __LINE__); LOG_DEBUG_INFO(lfile), log_lprintf(lfile, log_debug, "lprintf with debugging");

	log_close(lfile);

	{
		FILE *file = fopen("log.err", "r");
		int items = 0;
		char msg[255];
		char level[255];
		char sysname[255];
		char progname[255];
		int pid = 0;
		char month[255];
		int date = 0;
		char time[255];
		ZT_UNIT_ASSERT(test, (file));
		items = fscanf(file, "%s\n", msg);
		ZT_UNIT_ASSERT(test, ((items == 1) && 
							  (!strcmp(msg, "LOG_RAW"))));
		items = fscanf(file, "%s %d %s : %s\n", month, &date, time, msg);
		ZT_UNIT_ASSERT(test, ((items == 4) &&
							  (!strcmp(msg, "LOG_WITH_DATE"))));
		items = fscanf(file, "%s %s\n", level, msg);
		ZT_UNIT_ASSERT(test, ((items == 2) &&
							  (!strcmp(msg, "LOG_WITH_LEVEL"))));

		items = fscanf(file, "%s %s\n", sysname, msg);
		ZT_UNIT_ASSERT(test, ((items == 2) &&
							  (!strcmp(msg, "LOG_WITH_SYSNAME"))));
		items = fscanf(file, "%s %s %[A-Z_| ]\n", sysname, progname, msg);
		ZT_UNIT_ASSERT(test, ((items == 3) &&
							  (!strcmp(msg, "LOG_WITH_SYSNAME | LOG_WITH_PROGNAME"))));

		items = fscanf(file, "%s %s\n", progname, msg);
		ZT_UNIT_ASSERT(test, ((items == 2) &&
							  (!strcmp(msg, "LOG_WITH_PROGNAME"))));
		items = fscanf(file, "[%d]: %s\n", &pid, msg);
		ZT_UNIT_ASSERT(test, ((items == 2) &&
							  (!strcmp(msg, "LOG_WITH_PID"))));
		items = fscanf(file, "%s %d %s %s %[a-zA-Z_-][%d]: %s\n", month, &date, time, sysname, progname, &pid, msg);
		ZT_UNIT_ASSERT(test, ((items == 7) &&
							  (!strcmp(msg, "LOG_EMU_SYSLOG"))));
		items = fscanf(file, "LOG_DEBUG: in basic_tests at %s\n", msg);
		ZT_UNIT_ASSERT(test, ((items == 1) &&
							  (!strcmp(msg, position1))));
		items = fscanf(file, "lprintf with debugging: in basic_tests at %s\n", msg);
		ZT_UNIT_ASSERT(test, ((items == 1) &&
							  (!strcmp(msg, position2))));
		fclose(file);
	}
}

int
register_log_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "logging tests", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}
