#define ZT_WITH_UNIT
#include <zt.h>

typedef struct stack_elt {
    zt_stack_t member;
    int      n;
}stack_elt;

typedef struct queue_elt {
    zt_queue_t member;
    int      n;
}queue_elt;

static int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
#define VALUES_MAX (int)sizeof_array(values)

static void
basic_tests(struct zt_unit_test *test, void *data UNUSED)
{
    stack_elt * se;
    queue_elt * qe;

    zt_stack_t  * tse;
    zt_queue_t  * tqe;

    int         i;

    zt_stack(stk);
    zt_queue(que);


    ZT_UNIT_ASSERT(test, stk.prev == &stk);
    ZT_UNIT_ASSERT(test, stk.next == &stk);

    ZT_UNIT_ASSERT(test, zt_stack_empty(&stk));
    ZT_UNIT_ASSERT(test, zt_queue_empty(&que));


    for (i = 0; i < VALUES_MAX; i++) {
        se = zt_calloc(stack_elt, 1);
        qe = zt_calloc(queue_elt, 1);

        se->n = values[(VALUES_MAX-1) - i];
        qe->n = values[i];

        zt_stack_push(&stk, &se->member);
        zt_queue_enqueue(&que, &qe->member);
    }

    tse = zt_stack_peek(&stk);
    tqe = zt_queue_peek(&que);

    se = zt_stack_data(tse, stack_elt, member);
    qe = zt_queue_data(tqe, queue_elt, member);

    ZT_UNIT_ASSERT(test, se->n == values[0]);
    ZT_UNIT_ASSERT(test, qe->n == values[0]);

    for (i = 0; i < VALUES_MAX; i++) {
        tse = zt_stack_pop(&stk);
        tqe = zt_queue_dequeue(&que);

        se = zt_stack_data(tse, stack_elt, member);
        qe = zt_queue_data(tqe, queue_elt, member);

        ZT_UNIT_ASSERT(test, se->n == values[i]);
        ZT_UNIT_ASSERT(test, qe->n == values[i]);

        zt_free(se);
        zt_free(qe);
    }

    ZT_UNIT_ASSERT(test, zt_stack_empty(&stk));
    ZT_UNIT_ASSERT(test, zt_queue_empty(&que));

} /* basic_tests */


int
register_stack_suite(struct zt_unit *unit)
{
    struct zt_unit_suite * suite;

    suite = zt_unit_register_suite(unit, "stack tests", NULL, NULL, NULL);
    zt_unit_register_test(suite, "basic", basic_tests);
    return 0;
}
