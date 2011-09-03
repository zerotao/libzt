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
