/*
 * private.c        ZeroTao private definitions for logging
 *
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: log_private.c,v 1.4 2003/11/26 17:47:29 jshiffer Exp $
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

#include <unistd.h>

#include "../zt.h"
#include "../zt_progname.h"
#include "log_private.h"

log_level_desc_ty log_level_desc[] = {
  { log_emerg,   "Emergency" },
  { log_alert,   "Alert"     },
  { log_crit,    "Critical"  },
  { log_err,     "Error"     },
  { log_warning, "Warning"   },
  { log_notice,  "Notice"    },
  { log_info,    "Info"      },
  { log_debug,   "Debug"     },
  { -1, NULL },
};

log_ty*
log_new (log_vtbl_ty *vptr, unsigned int opts)
{
  log_ty *result;

  result = calloc(1, vptr->size);
  if(!result){
    fprintf(stderr, "Unable to calloc memory for log vtable\n");
    exit(1);
  }
  result->vtbl = vptr;
  result->file = NULL;
  result->function = NULL;
  result->line = 0;
  result->opts = opts;
  result->level = log_max;
  return(result);
}

char*
log_gen_fmt (log_ty *log, char *fmt, log_level level, unsigned int  opts)
{
  int len = 0;
  char *buff = NULL;

  len = strlen(fmt) + 4;	/* format + seperator + "\n" + null */
  buff = (char *)XCALLOC(char, len); /* mem_calloc(len, sizeof(char)); */

  if(opts & LOG_WITH_DATE){
    char sbuf[255];
    time_t tt = time(NULL);
    len += strftime(sbuf, 254, "%b %e %H:%M:%S ", localtime(&tt));
    buff = XREALLOC(char, buff, len);
    sprintf(buff, "%s", sbuf);
  }

  if(opts & LOG_WITH_SYSNAME){
    char sbuf[255];
    char *t = NULL;
    gethostname(sbuf, 254);
    t = index(sbuf, '.');
    if(t)
      *t = '\0';
    len += strlen(sbuf) + 1;	/* sbuf + space */
    buff = XREALLOC(char, buff, len);
    strcat(buff, sbuf);
    if((opts > LOG_WITH_SYSNAME))
      strcat(buff, " ");
  }

  if(opts & LOG_WITH_PROGNAME){
    len += strlen(progname(NULL, 0)); /* progname */
    buff = XREALLOC(char, buff, len);
    strcat(buff, progname(NULL, 0));
  }

  if(opts & LOG_WITH_PID){
    char sbuf[10 + 3];		/* pid + [] + null */
    pid_t pid = getpid();
    sprintf(sbuf, "[%u]", pid);
    len += strlen(sbuf);
    buff = XREALLOC(char, buff, len);
    strcat(buff, sbuf);
  }

  if(opts & LOG_WITH_LEVEL){
    if((level < log_max) || (level >= 0)){
      len += strlen(log_level_desc[level].desc) + 2; /* ': ' + level desc */
      buff = XREALLOC(char, buff, len);
      if (opts != LOG_WITH_LEVEL)
	strcat(buff, ": ");
      strcat(buff, log_level_desc[level].desc);
    }
  }
  if(opts != LOG_RAW){
    strcat(buff, ": ");
  }
  strcat(buff, fmt);
  if(((log->file) && (log->line > -1) && (log->function))){
      char *nbuff = NULL;
      int nlen = 13;		/* ': in  at (:)' + null */
      nlen += strlen(log->file);
      nlen += strlen(log->function);
      nlen += 10;			/* length of a int to str */
      nbuff = XCALLOC(char, nlen); /* mem_calloc(nlen, sizeof(char)); */
      sprintf(nbuff, ": in %s at (%s:%d)", log->function, log->file, log->line);
      len += nlen;
      buff = XREALLOC(char, buff, len);
      strcat (buff, nbuff);
      XFREE(nbuff);
  }
  strcat(buff, "\n");
  return buff;
}
