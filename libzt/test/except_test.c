/*
 * Copyright (C) 2000-2005, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
 * See file COPYING for details.
 *
 * $Id$
 *
 */

/*
 * Description: 
 */

#include <stdio.h>
#include <libzt/zt_except.h>
#include "test.h"

char	* Pass = "Pass";
char	* Fail = "Fail";
int main(int argc, char *argv[]){
	{
		char *do_try = Fail;
		DO_TRY
			THROW(do_try);
		ELSE_TRY
			CATCH(do_try, do_try = Pass;);
		END_TRY
			
		TEST("Alternate Syntax:", do_try == Pass);

		do_try = Fail;
		DO_TRY
			DO_TRY
			   THROW(do_try);
		        END_TRY
		
		ELSE_TRY
			CATCH(do_try, do_try = Pass;);
                END_TRY
			
			
                TEST("END_TRY w/o ELSE_TRY: ", do_try == Pass);
	}
	
	{
		char *do_try = Fail;
		TRY({
			    TRY(THROW(do_try)); 
		    },{
			    CATCH(do_try, do_try = Pass;);
		    });
		TEST("Empty Unwind: ", do_try == Pass);		
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
		TEST("Nesting: ", nest == Pass);
	}

	/* TRY_RETURN */
	{
		char *foo;
		char *f(int i)
		{
			TRY({
				    if(i == 0)
					    THROW(f);
				    TRY_RETURN Pass;
			    },
			    {
				    CATCH(f, TRY_RETURN Pass;);
			    });
			return Fail;
		}
		
		TRY({
			    foo = f(0);
			    THROW(foo);
		    },{
			    CATCH(foo, TEST("TRY_RETURN from CATCH: ", foo == Pass););
		    });
		

		TRY({
			    foo = f(1);
			    THROW(foo);
		    },{
			    CATCH(foo, TEST("TRY_RETURN from WIND: ", foo == Pass););
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
			    CATCH(domain, { TEST("Domains:", domain.subdomain.child == Pass); });
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
		TEST("Rethrow: ", rethrow == Pass);
	}
	
		
	/*Test Unwind Protect*/
	{
		char *unwind = Fail;
		
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
		TEST("Unwind_protect: ", unwind == Pass);
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
		TEST("CATCH escape:", catch == Pass);
	}

	/*Test CATCH in wind*/
	{
		char *domain=Pass;
		
		TRY({ CATCH(domain, domain=Fail;); },{ });
		TEST("Catch in Try Block:", domain == Pass);
	}
	
	{
		char	* domain=Fail;
		TRY({
			    THROW(domain);
		    },{
			    CATCH(except_CatchAll, domain=Pass;);
		    });
		TEST("except_All:", domain == Pass);
	}
	
			
	{
		int test_count = 0;
		
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

/* 		printf("Handlers: \n"); */

		/* call bar twice 
		 * Here I am using the function as the exception
		 */
		INSTALL_EXCEPT_HANDLER(bar, bar);
		INSTALL_EXCEPT_HANDLER(bar, bar);
		test_count = 0;
		
		TRY({
			    THROW(bar);
		    },{});
		TEST("Multiple Handler Calls:", test_count == 2);

		
		REMOVE_EXCEPT_HANDLER(bar, bar);
		REMOVE_EXCEPT_HANDLER(bar, bar);
		INSTALL_EXCEPT_HANDLER(bar, newbar);
		TRY({
			    THROW(bar);
		    },{});
		REMOVE_EXCEPT_HANDLER(bar, newbar);
		TEST("Removing handlers:", test_count == 0xDEADBEEF);
		test_count = 0;
		
		/* install foo 4 times however foo exits with 0 so only call once
		 * Same as above with the exception being a function
		 */
		INSTALL_EXCEPT_HANDLER(foo, foo);
		INSTALL_EXCEPT_HANDLER(foo, foo);
		INSTALL_EXCEPT_HANDLER(foo, foo);
		INSTALL_EXCEPT_HANDLER(foo, foo);

		TRY({
			    THROW(foo);
		    },{});
		TEST("Multiple installed/Call once:", test_count == 1);
		test_count = 0;
		
		fflush(stdout);
    
		REMOVE_EXCEPT_HANDLER(foo, foo);
		REMOVE_EXCEPT_HANDLER(foo, foo);
		REMOVE_EXCEPT_HANDLER(foo, foo);
		REMOVE_EXCEPT_HANDLER(foo, foo);
		//REMOVE_EXCEPT_HANDLER(foo, foo);

		test_count = 0;
		INSTALL_DEFAULT_HANDLER(foo);
		TRY_THROW(bar);

		TEST("INSTALL_DEFAULT_HANDLER: ", test_count > 0);
		
		/* FIXME: figure out how to test this ccorrectly */
/* 		fprintf(stderr, "This should exit: "); */
/* 		INSTALL_EXCEPT_HANDLER(fooexit, fooexit); */
/* 		TRY({  */
/* 			    THROW(fooexit); */
/* 		    },{}); */
/* 		fprintf(stderr, "Fail\n"); */
	}
	return 0;
}
