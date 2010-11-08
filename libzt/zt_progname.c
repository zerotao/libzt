/*
 * zt_progname.c        ZeroTao progname functions
 *
 * Copyright (C) 2000-2006, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id: progname.c,v 1.3 2003/06/09 16:55:09 jshiffer Exp $
 *
 */


/*
 * Description:
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>

#include "zt_internal.h"
#include "zt_cstr.h"
#include "zt_progname.h"
#include "zt_log.h"

#define UNKNOWN "*UNKNOWN*"
static char _progname[PATH_MAX+1] = UNKNOWN;
static char _progpath[PATH_MAX+1] = UNKNOWN;

char *
zt_progname(char *name, int opts)
{
    if (name && *name != '\0') {
        memset(_progname, '\0', PATH_MAX);
        if (opts == STRIP_DIR) {
            zt_cstr_basename(_progname, PATH_MAX, name, NULL);
        } else {
            memcpy(_progname, name, MIN(PATH_MAX, strlen(name)));
        }
    }
    return _progname;
}

char *
zt_progpath(char *prog) {

    if(prog && *prog != '\0') {
        memset(_progpath, '\0', PATH_MAX);
        zt_cstr_dirname(_progpath, PATH_MAX, prog);

        if(_progpath[0] == '\0') {
            /* the passed in path did not include a path */
            char        * path;
            size_t        offt = 0;
            ssize_t       base = 0;
            size_t        len;
            char        * tpath;
            struct stat   sbuf;


            path = getenv("PATH");
            if (!path) {
                /* no way to determine the path */
                return _progpath;
            }

            len = strlen(path);
            while(offt < len) {
                ssize_t   sofft;

                if ((sofft = zt_cstr_any(path, offt, -1, ENV_SEPERATOR)) < 0) {
                    break;
                }
                offt = (size_t)sofft;
                tpath = zt_cstr_catv(path, base, offt-1,
                                     PATH_SEPERATOR, 0, -1,
                                     prog, 0, -1, NULL);

                errno = 0;
                if((stat(tpath, &sbuf) == 0)) {
                    zt_cstr_copy(tpath, 0, -1, _progpath, PATH_MAX);
                    XFREE(tpath);
                    return _progpath;
                }
                XFREE(tpath);
                base = offt+1;
                offt++;
            }
        } else {
            /* the passed in path did include a path
             * determine if it is relative
             */
            char      cwd[PATH_MAX+1];

            getcwd(cwd, PATH_MAX);
            zt_cstr_copy(cwd, 0, -1, _progpath, PATH_MAX);
        }
    }
    return _progpath;
}
