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

#include <libzt/zt_log.h>
#include <libzt/zt_progname.h>

#include "test.h"

int
main( argc, argv )
     int argc;
     char *argv[];
{
  char position1[255];
  char position2[255];
  //  log_ty *lstderr = log_stderr((LOG_WITH_PROGNAME | LOG_WITH_LEVEL)); 
  log_ty *lfile = log_file( "log.err",  
  			    LOG_FILE_OVERWRITE,  
  			    (LOG_EMU_SYSLOG | LOG_WITH_LEVEL)); 
/*    log_ty *lsyslog = log_syslog(LOG_PID, LOG_DAEMON, LOG_RAW); */

  progname(argv[0], STRIP_DIR); 

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
  sprintf(position1, "(%s:%d)", __FUNCTION__, __LINE__); LOG_NDEBUG( "LOG_DEBUG" );
  sprintf(position2, "(%s:%d)", __FUNCTION__, __LINE__); LOG_NDEBUG_INFO(lfile), log_lprintf(lfile, log_debug, "lprintf with debugging");

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
    TEST("log[0]: ", (file));
    items = fscanf(file, "%s\n", msg);
    TEST("log[1]: ", ((items == 1) && 
		      (!strcmp(msg, "LOG_RAW"))));
    items = fscanf(file, "%s %d %s : %s\n", month, &date, time, msg);
    TEST("log[2]: ", ((items == 4) &&
		      (!strcmp(msg, "LOG_WITH_DATE"))));
    items = fscanf(file, "%s %s\n", level, msg);
    TEST("log[3]: ", ((items == 2) &&
		      (!strcmp(msg, "LOG_WITH_LEVEL"))));

    items = fscanf(file, "%s %s\n", sysname, msg);
    TEST("log[4]: ", ((items == 2) &&
		      (!strcmp(msg, "LOG_WITH_SYSNAME"))));
    items = fscanf(file, "%s %s %[A-Z_| ]\n", sysname, progname, msg);
    TEST("log[5]: ", ((items == 3) &&
		      (!strcmp(msg, "LOG_WITH_SYSNAME | LOG_WITH_PROGNAME"))));

    items = fscanf(file, "%s %s\n", progname, msg);
    TEST("log[6]: ", ((items == 2) &&
		      (!strcmp(msg, "LOG_WITH_PROGNAME"))));
    items = fscanf(file, "[%d]: %s\n", &pid, msg);
    TEST("log[7]: ", ((items == 2) &&
		      (!strcmp(msg, "LOG_WITH_PID"))));
    items = fscanf(file, "%s %d %s %s %[a-zA-Z_-][%d]: %s\n", month, &date, time, sysname, progname, &pid, msg);
    TEST("log[8]: ", ((items == 7) &&
		      (!strcmp(msg, "LOG_EMU_SYSLOG"))));
    items = fscanf(file, "LOG_DEBUG: in log_test.c at %s\n", msg);
    TEST("log[9]: ", ((items == 1) &&
		      (!strcmp(msg, position1))));
    items = fscanf(file, "lprintf with debugging: in log_test.c at %s\n", msg);
    TEST("log[10]: ", ((items == 1) &&
		      (!strcmp(msg, position2))));
    fclose(file);
  }
  return 0;
}
