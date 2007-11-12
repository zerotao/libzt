#include <libzt/zt.h>
#include <libzt/adt/zt_stack.h>
#include <libzt/adt/zt_queue.h>
#include <libzt/zt_unit.h>

typedef struct stack_elt
{
	zt_stack	  member;
	int		  n;
}stack_elt;

typedef struct queue_elt
{
	zt_queue	  member;
	int		  n;
}queue_elt;

static int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
#define VALUES_MAX sizeof_array(values)

static void
basic_tests(struct zt_unit_test *test, void *data)
{
	stack_elt	* se;
	queue_elt	* qe;

	zt_stack	* tse;
	zt_queue	* tqe;
	
	int		  i;
	
	zt_stack(stk);
	zt_queue(que);

	
	ZT_UNIT_ASSERT(test, stk.prev == &stk);
	ZT_UNIT_ASSERT(test, stk.next == &stk);

	ZT_UNIT_ASSERT(test, zt_stack_empty(&stk));
	ZT_UNIT_ASSERT(test, zt_queue_empty(&que));
	

	for(i = 0; i < VALUES_MAX; i++) {
		se = XCALLOC(stack_elt, 1);
		qe = XCALLOC(queue_elt, 1);
		
		se->n = values[i];
		qe->n = values[VALUES_MAX - i];

		zt_stack_push(&stk, &se->member);
		zt_queue_enqueue(&que, &qe->member);
	}


	for(i = 0; i < sizeof_array(values); i++) {
		tse = zt_stack_pop(&stk);
		tqe = zt_queue_dequeue(&que);
			
		se = zt_stack_data(tse, stack_elt, member);
		qe = zt_queue_data(tse, queue_elt, member);
		if(i == 0){			
			ZT_UNIT_ASSERT(test, se->n == qe->n);
			ZT_UNIT_ASSERT(test, se->n == qe->n);
		}
	}
	
	ZT_UNIT_ASSERT(test, zt_stack_empty(&stk));
	ZT_UNIT_ASSERT(test, zt_queue_empty(&que));
}


int
register_stack_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "stack tests", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}
