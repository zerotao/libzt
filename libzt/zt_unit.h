#ifndef _ZT_UNIT_H_
#define _ZT_UNIT_H_

#include <zt_internal.h>
#include <zt_assert.h>
#include <zt_macros.h>
#include <adt/zt_list.h>

BEGIN_C_DECLS

extern char * zt_unit_exception;

struct zt_unit;
struct zt_unit_suite;
struct zt_unit_test;

typedef void (*zt_unit_setup_fn)(void *data);
typedef void (*zt_unit_teardown_fn)(void *data);
typedef void (*zt_unit_test_fn)(struct zt_unit_test *test, void *data);

struct zt_unit {
    zt_elist_t   suites;
    unsigned int successes;
    unsigned int failures;
};

struct zt_unit_suite {
    zt_elist_t          suite;
    zt_elist_t          tests;
    char              * name;
    zt_unit_setup_fn    setup_fn;
    zt_unit_teardown_fn teardown_fn;
    void              * data;
    int                 succeeded;
    int                 failed;
};

struct zt_unit_test {
    zt_elist_t      test;
    char          * name;
    zt_unit_test_fn test_fn;
    int             success;
    char          * error;
    long            assertions;
};

#define ZT_UNIT_ASSERT(test, expr)                          \
    do {                                                    \
        zt_assert((expr));                                  \
        zt_unit_test_add_assertion(test);                   \
    } while(0)

#define ZT_UNIT_ASSERT_EQUAL(test, expr1, expr2) \
    ZT_UNIT_ASSERT(test, (expr1 == expr2))

#define ZT_UNIT_ASSERT_NOT_EQUAL(test, expr1, expr2) \
    ZT_UNIT_ASSERT(test, (expr1 != expr2))


/* #define ZT_UNIT_ASSERT_RAISES(test, excpt, expr) */


struct zt_unit *
zt_unit_init(void);

void
zt_unit_release(struct zt_unit **unit);

struct zt_unit_suite *
zt_unit_register_suite(struct zt_unit    * unit,
                       char     * name,
                       zt_unit_setup_fn setup_fn,
                       zt_unit_teardown_fn teardown_fn,
                       void     * data );

void
zt_unit_release_suite(struct zt_unit_suite **suite);

struct zt_unit_test *
zt_unit_register_test(struct zt_unit_suite    * suite,
                      char     * name,
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
zt_unit_test_add_assertion(struct zt_unit_test *test);

END_C_DECLS
#endif    /* _ZT_UNIT_H_ */
