#ifdef HAVE_STRING_H
#include <string.h>
#endif /* HAVE_STRING_H */

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif /* HAVE_ERRNO_H */

#include "zt.h"

int
zt_mkdir(char * path, mode_t mode, zt_mkdir_flags flags) {
    ssize_t     len;
    char        cpath[PATH_MAX + 1];
    ssize_t     offt;
    struct stat sbuf;
    ssize_t     pslen;

    errno = 0;

AGAIN:
    if (mkdir(path, mode) != 0) {

        if (errno == EEXIST) {
            return 0;
        }

        if (!ZT_BIT_ISSET(flags, zt_mkdir_create_parent)) {
            /* could not create dir and create parents not set */
            zt_log_strerror(zt_log_err, errno, "could not create dir: %s", path);
            return -1;
        }

        pslen = strlen(PATH_SEPERATOR);
        len   = strlen(path);
        memset(cpath, 0, sizeof(cpath));

        offt  = 0;
        while (len > offt) {
            offt = zt_cstr_find(path, offt, -1, PATH_SEPERATOR);
            if (offt == -1) {
                /* end of path */
                break;
            }

            memcpy(cpath, path, MIN(sizeof(cpath), (size_t)offt));
            offt += pslen;

            if (offt == pslen || stat(cpath, &sbuf) == 0) {
                /* path exists or we are looking at the root */
                continue;
            }

            mkdir(cpath, mode);
        }

        goto AGAIN;
    }

    return 0;
} /* zt_mkdir */

/* simple utility to remove the need for the local function to
 * allocate a stat struct when existance is the only thing that matters
 */
int
zt_path_exists(const char * path) {
    struct stat sbuf;

    errno = 0;
    if (stat(path, &sbuf) == 0) {
        return 0;
    }
    return -1;
}

/* find the longest matching directory containing a specific file or directory.
 * eg. the path /home/jshiffer/test/env and the trigger .ssh
 * would return /home/jshiffer unless there was a .ssh in a higher directory (env or test).
 */
char *zt_find_basedir(const char * path, const char * trigger) {
    struct stat sbuf;
    size_t      len;
    ssize_t     offt;
    char      * result = NULL;
    char        cpath[PATH_MAX + 1];
    char        lpath[PATH_MAX + 1];

    len = strlen(path);
    memset(lpath, 0, sizeof(lpath));
    memcpy(lpath, path, MIN(len, sizeof(cpath)));

    do {
        memset(cpath, 0, sizeof(cpath));
        snprintf(cpath, sizeof(cpath), "%s%s%s", lpath, PATH_SEPERATOR, trigger);
        errno = 0;
        if (stat(cpath, &sbuf) == 0) {
            /* found it */
            len    = strlen(lpath) + strlen(PATH_SEPERATOR) + 1;
            result = zt_callocs(sizeof(char), len);
            snprintf(result, len, "%s%s", lpath, PATH_SEPERATOR);
            break;
        }

        if ((strlen(lpath) == 0) ||
            (offt = zt_cstr_rfind(lpath, 0, -1, PATH_SEPERATOR)) == -1) {
            break;
        }
        lpath[offt] = '\0';
    } while (1);

    return result;
} /* zt_find_basedir */


