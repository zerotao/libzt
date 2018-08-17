/*
 * daemon_test.c        test daemon
 *
 * Copyright (C) 2008, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * See file COPYING for details.
 *
 */

/*
 * Description:
 */
#ifdef HAVE_CONFIG_H
# include <libzt/zt_config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STRING_H
# include <string.h>
#endif /* HAVE_STRING_H */

#define ZT_WITH_UNIT
#include <zt.h>

#include <sys/stat.h>

static void
makepid_tests(struct zt_unit_test *test, void* data UNUSED)
{
#ifndef WIN32
    const char* pidF = "/tmp/zt_unit_makepid.pid";

    int v = zt_writepid(pidF);
    ZT_UNIT_ASSERT(test, v == 0);

    const size_t kSz = 256;
    char buf[kSz];

    FILE* fh = fopen(pidF, "r");
    ZT_UNIT_ASSERT(test, fh != NULL);
    size_t nRead = fread(buf, 1, kSz, fh);
    ZT_UNIT_ASSERT(test, nRead > 0 && nRead < kSz);
    ZT_UNIT_ASSERT(test, fclose(fh) == 0);

    // check that current pid was written and that permissions are current
    // (NOTE: Not win32 compliant, but neither is the makepid code)
    pid_t pid = getpid();

    long tst = strtol(buf, NULL, 10);
    ZT_UNIT_ASSERT(test, tst == pid);

    // check permissions
    struct stat s;
    memset(&s, 0, sizeof(s));
    ZT_UNIT_ASSERT(test, stat(pidF, &s) == 0);

    mode_t tMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    ZT_UNIT_ASSERT(test, (s.st_mode & ~S_IFMT) == tMode);
#endif /* WIN32 */
}

int
register_daemon_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "daemon tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "zt_makepid", makepid_tests);
    return 0;
}
