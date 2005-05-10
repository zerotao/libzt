/*
 * zt_progname.c        ZeroTao progname functions
 *
 * Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: progname.c,v 1.3 2003/06/09 16:55:09 jshiffer Exp $
 *
 */


/*
 * Description: 
 */


#include <config.h>
#include <limits.h>

#include <string.h>

#include "zt.h"
#include "zt_cstr.h"
#include "zt_progname.h"

static char _progname[PATH_MAX];

char *
progname ( name, opts )
     char *name;
     int opts;
{
	if(name){
		memset(_progname, '\0', PATH_MAX);
		if(opts == STRIP_DIR){
			cstr_basename(_progname, PATH_MAX, name, NULL);
			//char *nname = basename(name);
			//memcpy(_progname, nname, strlen(name));
			//basename(_progname, PATH_MAX, name, NULL);
		} else {
			memcpy(_progname, name, MIN(PATH_MAX, strlen(name)));
		}
	}
	return _progname;
}
