#include <string.h>
#include <libzt/adt/zt_llist.h>
#include <libzt/zt_unit.h>

static void
basic_tests(struct zt_unit_test *test, void *data)
{
    char    * m = "c";
    zt_pair * x, * y;

    x = zt_llist_cons("a", NULL);
    x = zt_llist_cons("b", x);

    y = zt_llist_reverse(x);

    m = (char *)zt_llist_nth(y, 1);
    ZT_UNIT_ASSERT(test, strcmp(m, "a"));
}

int
register_llist_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "linked list tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
