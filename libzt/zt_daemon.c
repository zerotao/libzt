/*!
 * Filename: zt_daemon.c
 * Description: deamon tools
 *
 * Author: Jason L. Shiffer <jshiffer@zerotao.org>
 * Copyright:
 *        Copyright (C) 2000-2010, Jason L. Shiffer.
 *        See file COPYING for details
 *
 * Notes:
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "zt.h"
#include "zt_log.h"

/*!
 * Name: zt_daemonize
 *
 * Description: deamonize a process following (for the most part) the
 * rules defined by W. Richard Stevens "Advanced Programming in the UNIX
 * Environment".
 *
 */

int
zt_daemonize( char *root, mode_t mask, int options)
{
    pid_t         pid;
    pid_t         pid2;
    int           i;
    int           status;
    struct rlimit rlimits;

    if ((pid = fork()) == (pid_t)(-1)) { /* first error */
        zt_log_printf(zt_log_emerg, "failed to fork(): %s", strerror(errno));
        exit(1);
    } else if (pid == 0) {  /* first child */
        /* FIXME: Not everyone has setsid.  Add support for them. */
        if (setsid() == -1) {
            zt_log_printf(zt_log_emerg, "faild to setsid(): %s", strerror(errno));
            exit(1);
        }

        /* get the max number of open fd and close them */
        memset(&rlimits, 0, sizeof(struct rlimit));
        if (getrlimit(RLIMIT_NOFILE, &rlimits) == -1) {
            zt_log_printf(zt_log_emerg, "faild to getlimit(): %s", strerror(errno));
            exit(1);
        }
        for (i = 0; i < rlimits.rlim_cur; i++) {
            close(i);
        }                                                  /* Close all of the filehandles */

        if ((pid2 = fork()) == (pid_t)(-1)) {
            /* We can't produce an error here as we have already
             * closed all of the file descriptors
             */
            exit(1);
        } else if (pid2 == 0) { /* second child */
            int fd;
            if (root != NULL) {
                chdir(root);
            }
            umask(mask);
            fd = open("/dev/null", O_RDWR);    /* stdin */
            dup(fd);    /* stdout */
            dup(fd); /* stderr */
        } else {                                           /* second parent */
            sleep(1);
            exit(0);
        }
    } else {    /* first parent */
        /* wait here for the first child to exit */
        wait(&status);
        exit(0);
    }

    /* At this point we are in the second child properly forked and closed off */
    return 0;
} /* zt_daemonize */
