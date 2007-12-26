#ifndef _ZT_UNIT_H_
#define _ZT_UNIT_H_

#include <libzt/zt.h>
#include <libzt/zt_macros.h>
#include <libzt/zt_except.h>
BEGIN_C_DECLS

extern char * zt_unit_exception;

struct zt_unit;
struct zt_unit_suite;
struct zt_unit_test;

typedef void (*zt_unit_setup_fn)(void *data);
typedef void (*zt_unit_teardown_fn)(void *data);
typedef void (*zt_unit_test_fn)(struct zt_unit_test *test, void *data);


#define ZT_UNIT_ASSERT(test, expr)									\
	if (!(expr)) {													\
		zt_unit_exception = "Assertion Failed: " STR(expr);			\
		TRY_THROW(zt_unit_exception);								\
	}else{                                                          \
		zt_unit_test_add_assertion(test);							\
    }

#define ZT_UNIT_ASSERT_EQUAL(test, expr1, expr2)						\
	ZT_UNIT_ASSERT(test, (expr1 == expr2))
	/* 
     * if ((expr1) != (expr2)) {											\
	 * 	zt_unit_exception = "Assertion Failed: " STR(expr1) " != " STR(expr2); \
	 * 	TRY_THROW(zt_unit_exception);									\
	 * } else {                                                            \
	 * 	zt_unit_test_add_assertion(test);								\
     * }
     */

#define ZT_UNIT_ASSERT_NOT_EQUAL(test, expr1, expr2)					\
	ZT_UNIT_ASSERT(test, (expr1 != expr2))
	/* 
     * if ((expr1) == (expr2)) {											\
	 * 	zt_unit_exception = "Assertion Failed: " STR(expr1) " != " STR(expr2); \
	 * 	TRY_THROW(zt_unit_exception);									\
	 * } else {                                                            \
	 * 	zt_unit_test_add_assertion(test);								\
     * }
     */


	
#define ZT_UNIT_ASSERT_RAISES(test,excpt, expr)							\
	{																	\
		int	  success = 0;												\
		TRY({															\
				{expr;}													\
			},{															\
				CATCH(excpt, success = 1;);								\
			});															\
		if (success != 1) {												\
			zt_unit_exception = "Assertion Failed: " STR(expr) " did not raise " STR(excpt); \
			TRY_THROW(zt_unit_exception);								\
		}																\
		zt_unit_test_add_assertion(test);								\
	}



struct zt_unit *
zt_unit_init(void);

void
zt_unit_release(struct zt_unit **unit);

struct zt_unit_suite *
zt_unit_register_suite(struct zt_unit		* unit,
					   char 				* name,
					   zt_unit_setup_fn 	  setup_fn,
					   zt_unit_teardown_fn 	  teardown_fn,
					   void 				* data );

void
zt_unit_release_suite(struct zt_unit_suite **suite);

struct zt_unit_test *
zt_unit_register_test(struct zt_unit_suite	* suite,
					  char 					* name,
					  zt_unit_test_fn		  test_fn);

void
zt_unit_release_test(struct zt_unit_test **test);

int
zt_unit_run(struct zt_unit	* unit);

int
zt_unit_run_suite(struct zt_unit		* unit,
				  struct zt_unit_suite 	* suite);

int
zt_unit_run_test(struct zt_unit			* unit,
				 struct zt_unit_suite 	* suite,
				 struct zt_unit_test 	* test);
			
int
zt_unit_run_by_name(struct zt_unit		* unit,
					char				* name);
	
int
zt_unit_main(struct zt_unit				* unit,
			 int						  argc,
			 char						* argv[]);

void
zt_unit_test_add_assertion(struct zt_unit_test *test);

END_C_DECLS
#endif	/* _ZT_UNIT_H_ */
