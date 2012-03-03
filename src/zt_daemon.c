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
#include <limits.h>

#include "zt_internal.h"
#include "zt_log.h"
#include "zt_daemon.h"

/*!
 * Name: zt_daemonize
 *
 * Description: deamonize a process following (for the most part) the
 * rules defined by W. Richard Stevens "Advanced Programming in the UNIX
 * Environment".
 *
 */

int
zt_daemonize( char *root, mode_t mask, int options UNUSED)
{
    pid_t         pid;
    pid_t         pid2;
    rlim_t        i;
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
            if (i > INT_MAX) {
                /* int cast protection */
                i = INT_MAX;
            }
            close((int)i);
        }                                                  /* Close all of the filehandles */

        if ((pid2 = fork()) == (pid_t)(-1)) {
            /* We can't produce an error here as we have already
             * closed all of the file descriptors
             */
            exit(1);
        } else if (pid2 == 0) { /* second child */
            int fd;

            if (root != NULL) {
                if(chdir(root) != 0) {
                    zt_log_printf(zt_log_crit, "Could not chdir %s", root);
                    exit(1);
                }
            }
            umask(mask);
            fd = open("/dev/null", O_RDWR);    /* stdin */

            /* we don't care about the results as we have no open pids at this point */
            (void)dup(fd); /* stdout */
            (void)dup(fd); /* stderr */
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


int
zt_writepid(const char* pidF) {
    int    oVal = -1;
    int    fd   = -1;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    if (!pidF) {
        zt_log_strerror(zt_log_err, errno, "NULL pid file path.");
        return -1;
    }

    fd = creat(pidF, mode);

    if (fd != -1) {
        const int bSize = 64;
        char      buf[bSize];
        pid_t     pid   = getpid();

        int       sz    = snprintf(buf, bSize, "%i\n", (int)pid);
        if (sz > 0 && sz < bSize) {
            ssize_t nBytes = write(fd, buf, sz);
            while (nBytes == -1 && errno == EINTR) {
                nBytes = write(fd, buf, sz);
            }
            if (nBytes == sz) {
                oVal = 0;
            } else {
                zt_log_strerror(zt_log_err, errno, "Cannot write to pid file '%s'.", pidF);
            }
        } else {
            zt_log_strerror(zt_log_err, errno, "Cannot format to pid file '%s'.", pidF);
        }

        int v = close(fd);
        while (v == -1 && errno == EINTR) {
            v = close(fd);
        }

        if (v) {
            oVal = -1;
            zt_log_strerror(zt_log_err, errno, "Cannot close pid fd '%s'.", pidF);
        }
    } else {
        zt_log_strerror(zt_log_err, errno, "Cannot open pid file '%s' for writing.", pidF);
    }

    return oVal;
} /* zt_writepid */
