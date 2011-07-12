#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <zt.h>

#ifdef __cplusplus
extern "C" {
#pragma }
#endif
#define STMT_DEPTH 60
#define TEST_STEP 5

#define TEST(n, t)                                            \
    do {                                                      \
        int offset = 0;                                       \
        offset = printf(n);                                   \
        if (t) {                                              \
            printf(BLANK "=> success\n",                      \
                   INDENT_TO(STMT_DEPTH, TEST_STEP, offset)); \
        } else {                                              \
            printf(BLANK "=> failure @%s:%d\n",               \
                   INDENT_TO(STMT_DEPTH, TEST_STEP, offset),  \
                   __FILE__, __LINE__);                       \
        }                                                     \
    } while (0)


#define TEST_N(n, i, t)                                       \
    do {                                                      \
        int offset = 0;                                       \
        offset = printf("%s[%d]", n, i);                      \
        i += 1;                                               \
        if (t) {                                              \
            printf(BLANK "=> success\n",                      \
                   INDENT_TO(STMT_DEPTH, TEST_STEP, offset)); \
        } else {                                              \
            printf(BLANK "=> failure @%s:%d\n",               \
                   INDENT_TO(STMT_DEPTH, TEST_STEP, offset),  \
                   __FILE__, __LINE__);                       \
        }                                                     \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /*_TEST_H_*/
