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

#include "zt.h"
#include "zt_internal.h"
/* #include "zt_cstr.h" */
/* #include "zt_progname.h" */
/* #include "zt_log.h" */

#if defined(__APPLE__)
# include <mach-o/dyld.h>
#endif /* APPLE */
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

static ssize_t
_proc_get_path(char * result, size_t size) {
    pid_t     pid;
    char      path[PATH_MAX + 1];

    pid = getpid();

    /* FIXME: switch to "supported" definitions ie if defined(PROC_PID_PATH) ... */
#if defined(linux)
    /* check proc */
    snprintf(path, PATH_MAX, "/proc/%d/exe", pid);
#elif defined(sun) && defined(svr4)
    snprintf(path, PATH_MAX, "/proc/%d/path/a.out", pid);
#elif defined(BSD)
    snprintf(path, PATH_MAX, "/proc/%d/file", pid);
#else
    return -1;
#endif /* defined linux */

    return readlink(path, result, size);
}

char *
zt_os_progpath() {
    char    * result = NULL;

    if ((result = calloc(sizeof(char), PATH_MAX+1)) == NULL) {
        return NULL;
    }

    /*
     * OSX: _NSGetExecutablePath()
     * Solaris: getexecname() or readlink /proc/<pid>/path/a.out
     * Linux: readlink /proc/<pid>/exe
     * FreeBSD:  sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME -1
     * BSD w/ Procfs: readlink /proc/curproc/file
     * Windows:  GetModuleFileName() with hModule = NULL
     */

    if(_proc_get_path(result, PATH_MAX) == -1) {
#if defined(__APPLE__)
        uint32_t     size = PATH_MAX;
        _NSGetExecutablePath(result, &size);
#endif /* __APPLE__ */
    }

    return result;
}

char *
zt_progpath(char *prog) {

    /* If prog is !NULL then try to calculate the correct path.
     * Otherwise just return the current path setting.
     */
    if(prog && *prog != '\0') {
        /* if the passed in path is not NULL */
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
                /* try to use system specific methods to get the path */
                char    * pp;

                if ((pp = zt_os_progpath()) == NULL) {
                    return _progpath;
                }

                zt_cstr_dirname(_progpath, PATH_MAX, pp);
                zt_free(pp);
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
                    zt_free(tpath);
                    return _progpath;
                }
                zt_free(tpath);
                base = offt+1;
                offt++;
            }
        } else {
            /* the passed in path did include a path
             * determine if it is relative
             */
            char      cwd[PATH_MAX+1];

            if(getcwd(cwd, PATH_MAX) != NULL) {
                zt_cstr_copy(cwd, 0, -1, _progpath, PATH_MAX);
            }
        }
    }
    return _progpath;
}
