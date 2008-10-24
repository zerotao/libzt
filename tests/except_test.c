/*
 * Copyright (C) 2000-2006, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id$
 *
 */

/*
 * Description: 
 */

#include <stdio.h>
#define EXCEPT_DEFINE
#include <libzt/zt_except.h>
#include <libzt/zt_unit.h>

char	* Pass = "Pass";
char	* Fail = "Fail";

int test_count = 0;

static char *f1(int i)
{
	TRY({
			if(i == 0)
				THROW(f1);
			TRY_RETURN Pass;
		},
		{
			CATCH(f1, TRY_RETURN Pass;);
		});
	return Fail;
}

int foo(void *e, void* t, char *et, char *f, char *ff, int l){
	test_count++;
	return 0;
}
int bar(void *e, void* t, char *et, char *f, char *ff, int l){
	test_count++;
	return 1;
}
int newbar(void *e, void* t, char *et, char *f, char *ff, int l){
	test_count = 0xDEADBEEF;
	return 0;
}
int fooexit(void *e, void* t, char *et, char *f, char *ff, int l){
	return -1;
}

static void
basic_tests(struct zt_unit_test *test, void *data)
{
	{
		char *do_try = Fail;
		DO_TRY
			THROW(do_try);
		ELSE_TRY
			CATCH(do_try, do_try = Pass;);
		END_TRY

		ZT_UNIT_ASSERT(test, do_try == Pass);

		do_try = Fail;
		DO_TRY
			DO_TRY
			   THROW(do_try);
		    END_TRY
		ELSE_TRY
			CATCH(do_try, do_try = Pass;);
		END_TRY
		ZT_UNIT_ASSERT(test, do_try == Pass);
	}
	
	{
		char *do_try = Fail;
		TRY({
			    TRY(THROW(do_try)); 
		    },{
			    CATCH(do_try, do_try = Pass;);
		    });
		ZT_UNIT_ASSERT(test, do_try == Pass);		
	}

		
	/*Test Nesting*/
	{
		char *nest = Fail;
		TRY({
			    TRY(TRY(TRY(TRY(TRY(THROW(nest))))));
		    },
		    {
			    CATCH(nest, { nest = Pass; });
		    });
		ZT_UNIT_ASSERT(test, nest == Pass);
	}

	/* TRY_RETURN */
	{
		char *foo;
		
		TRY({
			    foo = f1(0);
			    THROW(foo);
		    },{
			    CATCH(foo, ZT_UNIT_ASSERT(test, foo == Pass););
		    });
		

		TRY({
			    foo = f1(1);
			    THROW(foo);
		    },{
			    CATCH(foo, ZT_UNIT_ASSERT(test, foo == Pass););
		    });
		
	}
	
	/*Test Domains*/
	{
		struct {
			struct {
				int i_child;
				char *child;
				int i_child2;
			}subdomain;
			char *domain_child;
		}domain;
    
		
		TRY({  
			    domain.subdomain.child = Fail;
			    TRY({
					TRY({ THROW(domain.subdomain.child); },{});
				},{ 
					CATCH(domain.subdomain,
					      {
						      domain.subdomain.child = Pass;
					      });
					THROW(domain.subdomain.i_child2);
				});
		    },{
			    CATCH(domain, { ZT_UNIT_ASSERT(test, domain.subdomain.child == Pass); });
		    });
	}

	{
		static int	  tmp = 0;
		INSTALL_DEFAULT_HANDLER(domain_default_except_handler);
		
		EXCEPT_DESC(domain2, "domain2",
		  EXCEPT_GROUP(subdomain, "subdomain",
		    EXCEPTION(child, "child")));

		TRY({  
			    TRY({
					TRY({ THROW(domain2.subdomain.child); },{});
				},{ 
					CATCH(domain2.subdomain,
					      {
						      tmp = 1;
					      });
					THROW(domain2.subdomain);
				});
		    },{
				CATCH(domain2, { ZT_UNIT_ASSERT(test, tmp != 0); });
		    });
		
	}

	
	{
		char *rethrow = Fail;
		
		DO_TRY
		{
			DO_TRY
			{
				THROW(rethrow);
			} ELSE_TRY {
				CATCH(rethrow, rethrow = Fail;);
				RETHROW();
			}
			END_TRY;
		}
		ELSE_TRY
		{
			CATCH(printf, rethrow = Fail;);
			CATCH(rethrow, rethrow = Pass;);
		}
		END_TRY;
		ZT_UNIT_ASSERT(test, rethrow == Pass);
	}
	
		
	/*Test Unwind Protect*/
	{
		char *unwind = Fail;
		//char *dummy = Fail;
		
		DO_TRY
			UNWIND_PROTECT({
					       unwind = Fail;
					       THROW(unwind);
				       },{
					       unwind = Pass;
				       });
		ELSE_TRY
			CATCH(unwind, {});
		END_TRY
		ZT_UNIT_ASSERT(test, unwind == Pass);

		DO_TRY {
			UNWIND_PROTECT({
					unwind = Fail;
				},{
					unwind = Pass;
				});
		} ELSE_TRY {
			CATCH(unwind, unwind = Fail;);
		} END_TRY;
		ZT_UNIT_ASSERT(test, unwind == Pass);			
	}

	/*Test CATCH escape*/
	{
		char *catch = Fail;
		
		TRY({
			    TRY({
					THROW(catch);
				},{
					CATCH(catch, catch = Pass;);
				});
		    },{
			    CATCH(catch, catch = Fail;);
		    });
		ZT_UNIT_ASSERT(test, catch == Pass);
	}

	/*Test CATCH in wind*/
	{
		char *domain=Pass;
		
		TRY({ CATCH(domain, domain=Fail;); },{ });
		ZT_UNIT_ASSERT(test, domain == Pass);
	}
	
	{
		char	* domain=Fail;
		TRY({
			    THROW(domain);
		    },{
			    CATCH(except_CatchAll, domain=Pass;);
		    });
		ZT_UNIT_ASSERT(test, domain == Pass);
	}
	
			
/* This will fail with the new unit test framework fix it!!!
 * 	{
 * 		test_count = 0;
 * 		
 * 
 * /\* 		printf("Handlers: \n"); *\/
 * 
 * 		/\* call bar twice 
 * 		 * Here I am using the function as the exception
 * 		 *\/
 * 		INSTALL_EXCEPT_HANDLER(bar, bar);
 * 		INSTALL_EXCEPT_HANDLER(bar, bar);
 * 		test_count = 0;
 * 		
 * 		TRY({
 * 			    THROW(bar);
 * 		    },{});
 * 		
 * 		ZT_UNIT_ASSERT(test, test_count == 2);
 * 		
 * 		REMOVE_EXCEPT_HANDLER(bar, bar);
 * 		REMOVE_EXCEPT_HANDLER(bar, bar);
 * 		
 * 		INSTALL_EXCEPT_HANDLER(bar, newbar);
 * 		TRY({
 * 			    THROW(bar);
 * 		    },{});
 * 		REMOVE_EXCEPT_HANDLER(bar, newbar);
 * 		ZT_UNIT_ASSERT(test, test_count == 0xDEADBEEF);
 * 		test_count = 0;
 * 		
 * 		/\* install foo 4 times however foo exits with 0 so only call once
 * 		 * Same as above with the exception being a function
 * 		 *\/
 * 		INSTALL_EXCEPT_HANDLER(foo, foo);
 * 		INSTALL_EXCEPT_HANDLER(foo, foo);
 * 		INSTALL_EXCEPT_HANDLER(foo, foo);
 * 		INSTALL_EXCEPT_HANDLER(foo, foo);
 * 
 * 		TRY({
 * 			    THROW(foo);
 * 		    },{});
 * 		ZT_UNIT_ASSERT(test, test_count == 1);
 * 		test_count = 0;
 * 		
 * 		fflush(stdout);
 *     
 * 		REMOVE_EXCEPT_HANDLER(foo, foo);
 * 		REMOVE_EXCEPT_HANDLER(foo, foo);
 * 		REMOVE_EXCEPT_HANDLER(foo, foo);
 * 		REMOVE_EXCEPT_HANDLER(foo, foo);
 * 		//REMOVE_EXCEPT_HANDLER(foo, foo);
 * 
 * 		test_count = 0;
 * 		INSTALL_DEFAULT_HANDLER(foo);
 * 		TRY_THROW(bar);
 * 
 * 		ZT_UNIT_ASSERT(test, test_count > 0);
 * 		
 * 		/\* FIXME: figure out how to test this ccorrectly *\/
 * /\* 		fprintf(stderr, "This should exit: "); *\/
 * /\* 		INSTALL_EXCEPT_HANDLER(fooexit, fooexit); *\/
 * /\* 		TRY({  *\/
 * /\* 			    THROW(fooexit); *\/
 * /\* 		    },{}); *\/
 * /\* 		fprintf(stderr, "Fail\n"); *\/
 * 		
 * 			 }
 */
}


int
register_except_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "except tests", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}
