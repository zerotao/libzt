#include <libzt/zt.h>
#include <libzt/adt/zt_stack.h>
#include <libzt/adt/zt_queue.h>

#include "test.h"

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

int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
#define VALUES_MAX sizeof_array(values)

int
main(int argc, char *argv[]) 
{
	stack_elt	* se;
	queue_elt	* qe;

	zt_stack	* tse;
	zt_queue	* tqe;
	
	int	  	  test_n = 0;
	int		  i;
	
	zt_stack(stk);
	zt_queue(que);

	
	TEST_N("zt_stack", test_n, stk.prev == &stk);
	TEST_N("zt_stack", test_n, stk.next == &stk);

	test_n = 0;
	
	TEST_N("zt_stack_head", test_n, zt_stack_empty(&stk));
	TEST_N("zt_queue_empty", test_n, zt_queue_empty(&que));
	

	for(i = 0; i < VALUES_MAX; i++) {
		se = XCALLOC(stack_elt, 1);
		qe = XCALLOC(queue_elt, 1);
		
		se->n = values[i];
		qe->n = values[VALUES_MAX - i];

		zt_stack_push(&stk, &se->member);
		zt_queue_enqueue(&que, &qe->member);
	}


	test_n = 0;
	for(i = 0; i < sizeof_array(values); i++) {
		tse = zt_stack_pop(&stk);
		tqe = zt_queue_dequeue(&que);
			
		se = zt_stack_data(tse, stack_elt, member);
		qe = zt_queue_data(tse, queue_elt, member);
		if(test_n == 0){			
			TEST_N("zt_stack_pop & zt_stack_data ", test_n, se->n == qe->n);
			test_n = 0;
			TEST_N("zt_queue_dequeue & zt_queue_data ", test_n, se->n == qe->n);
		}
	}
	
	test_n = 0;
	
	TEST_N("zt_stack_empty", test_n, zt_stack_empty(&stk));
	TEST_N("zt_queue_empty", test_n, zt_queue_empty(&que));
	return 0;
}
