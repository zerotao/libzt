/*
 * Copyright (C) 2000-2004, Jason L. Shiffer <jshiffer@zerotao.com>.  All Rights Reserved.
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

int main(int argc, char *argv[]){
	
	{
		char *do_try = "Fail";
		printf("Alternate Syntax: ");
		
		DO_TRY
			THROW(do_try);
		ELSE_TRY
			CATCH(do_try, do_try = "Pass";);
		END_TRY;
		printf("%s\n", do_try);

		do_try = "Fail";
		printf("END_TRY w/o ELSE_TRY: ");
		DO_TRY
			DO_TRY
			   THROW(do_try);
		        END_TRY
		ELSE_TRY
			CATCH(do_try, do_try = "Pass";);
		END_TRY
		printf("%s\n", do_try);
	}
	
	{
		char *do_try = "failed";
		printf("Empty Unwind: ");
		TRY(
		    {
			    TRY(THROW(do_try)); 
		    },{
			    CATCH(do_try, do_try = "Pass";);
		    });
		printf("%s\n", do_try);
	}

		
	/*Test Nesting*/
	{
		char *nest = "Fail";
		printf("Nesting: ");
		TRY({
			    TRY(TRY(TRY(TRY(TRY(THROW(nest))))));
		    },
		    {
			    CATCH(nest, { nest = "Pass"; });
		    });
		printf("%s\n", nest);
	}

	/* TRY_RETURN */
	{
		char *foo;
		char *f(int i)
		{
			TRY({
				    if(i == 0)
					    THROW(f);
				    TRY_RETURN "Pass";
			    },
			    {
				    CATCH(f, TRY_RETURN "Pass";);
			    });
			return "Fail";
		}
		printf("TRY_RETURN from CATCH: ");
		TRY({
			    foo = f(0);
			    THROW(foo);
		    },
		    {
			    CATCH(foo, printf("%s\n", foo); );
		    });
		
		printf("TRY_RETURN from WIND: ");
		TRY({
			    foo = f(1);
			    THROW(foo);
		    },
		    {
			    CATCH(foo, printf("%s\n", foo); );
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
    
		printf("Domains: ");
		TRY({  
			    domain.subdomain.child = "Fail";
			    TRY({
					TRY({ THROW(domain.subdomain.child); },{});
				},{ 
					CATCH(domain.subdomain,
					      {
						      domain.subdomain.child = "Pass";
					      });
					THROW(domain.subdomain.i_child2);
				});
		    },{
			    CATCH(domain, {	printf("%s\n", domain.subdomain.child); });
		    });
	}
	{
		char *rethrow = "Fail";
		printf("Rethrow: ");
		DO_TRY
		{
			DO_TRY
			{
				THROW(rethrow);
			} ELSE_TRY {
				CATCH(rethrow, rethrow = "Fail";);
				RETHROW();
			}
			END_TRY;
		}
		ELSE_TRY
		{
			CATCH(printf, rethrow = "Fail";);
			CATCH(rethrow, rethrow = "Pass";);
		}
		END_TRY;
		printf("%s\n", rethrow);
	}
	
		
	/*Test Unwind Protect*/
	{
		char *unwind = "Fail";
		printf("Unwind_protect: ");
		DO_TRY
			UNWIND_PROTECT({
					       unwind = "Fail";
					       THROW(unwind);
				       },{
					       unwind = "Pass";
				       });
		ELSE_TRY
			CATCH(unwind, {});
		END_TRY
		printf("%s\n", unwind);
	}

	/*Test CATCH escape*/
	{
		char *catch = "Fail";
		printf("CATCH escape: ");
		TRY({
			    TRY({
					THROW(catch);
				},{
					CATCH(catch, catch = "Pass";);
				});
		    },{
			    CATCH(catch, catch="Fail";);
		    });
		printf("%s\n", catch);
	}

	/*Test CATCH in wind*/
	{
		char *domain="Pass";
		printf("Catch in Try Block: ");
		TRY({ CATCH(domain, domain="Fail";); },{ });
		printf("%s\n", domain);
	}

	{
		int foo(void *e, void* t, char *et, char *f, char *ff, int l){
			static int times = 1;
			printf("%d ", times++);
			return 0;
		}
		int bar(void *e, void* t, char *et, char *f, char *ff, int l){
			static int times = 1;
			printf("%d ", times++);
			return 1;
		}
		int newbar(void *e, void* t, char *et, char *f, char *ff, int l){
			printf("Pass\n");
			return 0;
		}
		int fooexit(void *e, void* t, char *et, char *f, char *ff, int l){
			return -1;
		}

		printf("Handlers: \n");

		/* call bar twice 
		 * Here I am using the function as the exception
		 */
		INSTALL_EXCEPT_HANDLER(bar, bar);
		INSTALL_EXCEPT_HANDLER(bar, bar);

		printf("  Multiple calls (should be 1 2): ");
    
		TRY({
			    THROW(bar);
		    },{});
		printf("\n");

		printf("  Remove Handlers: ");
		REMOVE_EXCEPT_HANDLER(bar, bar);
		REMOVE_EXCEPT_HANDLER(bar, bar);
		INSTALL_EXCEPT_HANDLER(bar, newbar);
		TRY({
			    THROW(bar);
		    },{});
		REMOVE_EXCEPT_HANDLER(bar, newbar);

		/* install foo 4 times however foo exits with 0 so only call once
		 * Same as above with the exception being a function
		 */
		INSTALL_EXCEPT_HANDLER(foo, foo);
		INSTALL_EXCEPT_HANDLER(foo, foo);
		INSTALL_EXCEPT_HANDLER(foo, foo);
		INSTALL_EXCEPT_HANDLER(foo, foo);

		printf("  Multiple installed but call only once (should be 1): ");
		TRY({
			    THROW(foo);
		    },{});

		printf("\n");
		fflush(stdout);
    
		REMOVE_EXCEPT_HANDLER(foo, foo);
		REMOVE_EXCEPT_HANDLER(foo, foo);
		REMOVE_EXCEPT_HANDLER(foo, foo);
		REMOVE_EXCEPT_HANDLER(foo, foo);
		//REMOVE_EXCEPT_HANDLER(foo, foo);
    
		fprintf(stderr, "  Exit from Handler: ");
		INSTALL_EXCEPT_HANDLER(fooexit, fooexit);
		TRY({ 
			    THROW(fooexit);
		    },{});
		fprintf(stderr, "Fail\n");
	}
	return 0;
}
