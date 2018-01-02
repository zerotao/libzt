#ifndef _ZT_UNIT_H_
#define _ZT_UNIT_H_

#include <setjmp.h>
#include <zt.h>
#include <libzt/zt_list.h>

BEGIN_C_DECLS

extern char * zt_unit_exception;

struct zt_unit;
struct zt_unit_suite;
struct zt_unit_test;

typedef void (*zt_unit_setup_fn)(void *data);
typedef int  (*zt_unit_try_fn)(struct zt_unit_suite * suite, struct zt_unit_test * test);
typedef void (*zt_unit_teardown_fn)(void *data);
typedef void (*zt_unit_test_fn)(struct zt_unit_test *test, void *data);

struct zt_unit {
    zt_elist_t   suites;

    unsigned int suite_count;
    unsigned int tests;
    unsigned int successes;
    unsigned int failures;
    unsigned int assertions;
    unsigned int exceptions;
    unsigned int empty;
};

struct zt_unit_suite {
    zt_elist_t          suite;
    zt_elist_t          tests;
    char              * name;
    zt_unit_setup_fn    setup_fn;
    zt_unit_try_fn      try_fn;
    zt_unit_teardown_fn teardown_fn;
    void              * data;
    int                 succeeded;
    int                 failed;
    int                 exceptions;
    int                 assertions;
    int                 empty;
};

struct zt_unit_test {
    zt_elist_t      test;
    char          * name;
    zt_unit_test_fn test_fn;
    int             success;
    char          * error;
    long            assertions;
    long            exceptions;
    jmp_buf         env;
};

int zt_unit_printf(char **strp, const char *fmt, ...);

#ifdef __cplusplus
class zt_unit_exceptT {};
# define ZT_UNIT_EXIT(test) \
    throw zt_unit_exceptT()
#else
# define ZT_UNIT_EXIT(test) \
    longjmp(test->env, 1)
#endif /* __cplusplus */

#define ZT_UNIT_ASSERT(test, expr)                          \
    do {                                                    \
        if((expr)) {                                        \
            zt_unit_test_add_assertion(test);               \
        } else {                                            \
            if(zt_unit_printf(&test->error, "%s %s:%d", #expr, __FILE__, __LINE__) == 0)  { \
                test->error = NULL;                         \
            }                                               \
            ZT_UNIT_EXIT(test);                             \
        }                                                   \
    } while(0)

#define ZT_UNIT_ASSERT_EQUAL(test, expr1, expr2) \
    ZT_UNIT_ASSERT(test, (expr1 == expr2))

#define ZT_UNIT_ASSERT_NOT_EQUAL(test, expr1, expr2) \
    ZT_UNIT_ASSERT(test, (expr1 != expr2))

/* #define ZT_UNIT_ASSERT_RAISES(test, excpt, expr) */

#define ZT_UNIT_SET_ERROR(test, fmt, ...) \
    zt_unit_printf(&test->error, fmt, __VA_ARGS__)

#define ZT_UNIT_FAIL(test, fmt, ...) \
    ZT_UNIT_SET_ERROR(test, fmt, __VA_ARGS__), ZT_UNIT_EXIT(test)


struct zt_unit *
zt_unit_init(void);

int zt_unit_try_cpp(struct zt_unit_suite * suite, struct zt_unit_test * test);
int zt_unit_try_c(struct zt_unit_suite * suite, struct zt_unit_test * test);

void
zt_unit_release(struct zt_unit **unit);

struct zt_unit_suite *
zt_unit_register_suite_(struct zt_unit    * unit,
                       const char * name,
                       zt_unit_setup_fn setup_fn,
                       zt_unit_teardown_fn teardown_fn,
                       void     * data,
                       zt_unit_try_fn try_fn
                       );

#ifdef __cplusplus
# define zt_unit_register_suite(unit, name, setup_fn, teardown_fn, data) \
    zt_unit_register_suite_(unit, name, setup_fn, teardown_fn, data, zt_unit_try_cpp);
#else
# define zt_unit_register_suite(unit, name, setup_fn, teardown_fn, data) \
    zt_unit_register_suite_(unit, name, setup_fn, teardown_fn, data, zt_unit_try_c);
#endif

void
zt_unit_release_suite(struct zt_unit_suite **suite);

struct zt_unit_test *
zt_unit_register_test(struct zt_unit_suite    * suite,
                      const char * name,
                      zt_unit_test_fn test_fn);

void
zt_unit_release_test(struct zt_unit_test **test);

int
zt_unit_run(struct zt_unit    * unit);

int
zt_unit_run_suite(struct zt_unit    * unit,
                  struct zt_unit_suite     * suite);

int
zt_unit_run_test(struct zt_unit    * unit,
                 struct zt_unit_suite     * suite,
                 struct zt_unit_test     * test);

int
zt_unit_run_by_name(struct zt_unit    * unit,
                    char    * name);

int
zt_unit_main(struct zt_unit    * unit,
             int argc,
             char    * argv[]);

void
zt_unit_test_add_exception(struct zt_unit_test *test);

void
zt_unit_test_add_assertion(struct zt_unit_test *test);

void
zt_unit_test_global_stats(struct zt_unit *unit);

END_C_DECLS
#endif    /* _ZT_UNIT_H_ */
