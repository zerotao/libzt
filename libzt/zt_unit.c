#include <string.h>

#include "zt_unit.h"
#include "zt_assert.h"
#include "zt_except.h"
#include "zt_log.h"
#include "adt/zt_list.h"
#include "zt_opts.h"

#define ZT_UNIT_EXCEPTION_DEFAULT "Unknown Error"
char * zt_unit_exception = ZT_UNIT_EXCEPTION_DEFAULT;

struct zt_unit {
	zt_elist				  suites;
};

struct zt_unit_suite {
	zt_elist				  suite;
	zt_elist				  tests;
	char					* name;
	zt_unit_setup_fn		  setup_fn;
	zt_unit_teardown_fn		  teardown_fn;
	void					* data;
	int						  succeeded;
	int						  failed;
};

struct zt_unit_test {
	zt_elist				  test;
	char					* name;
	zt_unit_test_fn			  test_fn;
	int						  success;
	char					* error;
};

struct zt_unit *
zt_unit_init(void)
{
	struct zt_unit	* unit = XCALLOC(struct zt_unit, 1);
	log_ty			* log;
	
	zt_elist_reset(&unit->suites);
	return unit;
}

void
zt_unit_release(struct zt_unit **unit)
{
	zt_elist	* ignore;
	zt_elist	* tmp;

	assert(unit);
	assert(*unit);

	zt_elist_for_each_safe(&(*unit)->suites, tmp, ignore) {
		struct zt_unit_suite	* tsuite = zt_elist_data(tmp, struct zt_unit_suite, suite);
		zt_unit_release_suite(&tsuite);
	}
	
	XFREE(*unit);
	*unit = NULL;
}

struct zt_unit_suite *
zt_unit_register_suite(struct zt_unit 		* unit,
					   char 				* name,
					   zt_unit_setup_fn 	  setup_fn,
					   zt_unit_teardown_fn 	  teardown_fn,
					   void 				* data ) 
{
	int						  len;
	struct zt_unit_suite	* suite = XCALLOC(struct zt_unit_suite, 1);
	
	assert(name != NULL);
	suite->name = XCALLOC(char, len+1);
	strncpy(suite->name, name, len);
	suite->setup_fn = setup_fn;
	suite->teardown_fn = teardown_fn;
	suite->data = data;
	suite->succeeded = 0;
	suite->failed = 0;
	
	zt_elist_reset(&suite->tests);
	zt_elist_add(&unit->suites, &suite->suite);
	return suite;
}

void
zt_unit_release_suite(struct zt_unit_suite **suite)
{
	zt_elist	* ignore;
	zt_elist	* tmp;
	
	assert(suite);
	assert(*suite);
	assert((*suite)->name);

	zt_elist_for_each_safe(&(*suite)->tests, tmp, ignore) {
		struct zt_unit_test	* ttest = zt_elist_data(tmp, struct zt_unit_test, test);
		zt_unit_release_test(&ttest);
	}
	
	zt_elist_remove(&(*suite)->suite);

	XFREE((*suite)->name);
	XFREE(*suite);
	*suite = NULL;
}


struct zt_unit_test *
zt_unit_register_test(struct zt_unit_suite	* suite,
					  char 					* name,
					  zt_unit_test_fn		  test_fn) 
{
	struct zt_unit_test	* test = XCALLOC(struct zt_unit_test, 1);
	int					  len;
	
	assert(suite);
	assert(name);
	
	len = strlen(name);

	test->name = XCALLOC(char, len+1);
	strncpy(test->name, name, len);
	test->success = -1;
	test->test_fn = test_fn;
	
	zt_elist_add(&suite->tests, &test->test);
	return test;
}

void
zt_unit_release_test(struct zt_unit_test **test)
{
	assert (test);
	assert(*test);
	assert((*test)->name);

	zt_elist_remove(&(*test)->test);
	
	XFREE((*test)->name);
	
	if ((*test)->error) {
		XFREE((*test)->error);
	}
	
	XFREE(*test);
}


int
zt_unit_run(struct zt_unit	* unit)
{
	zt_elist				* tmp;
	struct zt_unit_suite	* unit_suite;
	int						  result = 0;

	zt_elist_for_each(&unit->suites, tmp) {
		unit_suite = zt_elist_data(tmp, struct zt_unit_suite, suite);
		result += zt_unit_run_suite(unit, unit_suite);
	}
	return result;
}

char * minuses_60 = "------------------------------------------------------------";

static int
printf_underline(int len) 
{
	char	  fmt[7+1];  // "%%.??s\n"
	
	snprintf(fmt, 7, "%%.%ds\n", len <= 60 ? len : 60);
	return printf(fmt, minuses_60);
}

int
zt_unit_run_suite(struct zt_unit		* unit,
				  struct zt_unit_suite 	* suite)
{
	zt_elist			* tmp;
	struct zt_unit_test	* unit_test;
	int					  result;
	int					  len;
	
	suite->failed = 0;
	suite->succeeded = 0;
	printf("---\n%s:\n", suite->name);
	printf(BLANK "Tests:\n", INDENT_TO(2, 2, 0));
	
	zt_elist_for_each(&suite->tests, tmp) {
		unit_test = zt_elist_data(tmp, struct zt_unit_test, test);
		result = zt_unit_run_test(unit, suite, unit_test);
		if (result != TRUE) {
			suite->failed += 1;
		} else {
			suite->succeeded += 1;
		}
	}

	if (suite->failed != 0) {
		len = printf(BLANK "Errors:\n", INDENT_TO(2, 2, 0));
		
		zt_elist_for_each(&suite->tests, tmp) {
			unit_test = zt_elist_data(tmp, struct zt_unit_test, test);
			if (unit_test->success != TRUE) {
				len = printf(BLANK "%s", INDENT_TO(4, 4, 0), unit_test->name);
				len = printf(BLANK ": '%s'\n", INDENT_TO(30, 5, len), unit_test->error);			
			}
		}
	}
	
	return suite->failed + suite->succeeded;
}


static void
test_passed(struct zt_unit_test *test)
{
	assert(test);
	test->success = TRUE;
}

static void
test_failed(struct zt_unit_test	* test,
			char				* error,
			char				* efile,
			char				* efunc,
			int				  	  eline)
{
	int	  len;
#   define MAX_STR_INT	43
	assert(test);
	assert(error);

	test->success = FALSE;
	
	len = strlen(error);
	len += strlen(efile);
	len += strlen(efunc);
	len += MAX_STR_INT + 3 + 3;
	
	test->error = XCALLOC(char, len + 1);
	snprintf(test->error, len, "%s @ %s[%s:%d]", error, efile, efunc, eline);
}

int
zt_unit_run_test(struct zt_unit			* unit,
				 struct zt_unit_suite 	* suite,
				 struct zt_unit_test 	* test) 
{
	int	  offt;
	
	test->success = FALSE;
	assert(test);
	
	offt = printf(BLANK "%s", INDENT_TO(4, 4, 0), test->name);
	
	TRY({
			UNWIND_PROTECT({
					if(suite->setup_fn) 
						suite->setup_fn(suite->data);
					test->test_fn(suite->data);
					test_passed(test);
				},{
					if(suite->teardown_fn)
						suite->teardown_fn(suite->data);
				});
		},{
			CATCH(zt_unit_exception,
				  {
					  test_failed(test, zt_unit_exception, _except_Stack->efile, _except_Stack->efunc, _except_Stack->eline);
					  zt_unit_exception = ZT_UNIT_EXCEPTION_DEFAULT;
				  });
		});

	printf(BLANK ": %s\n", INDENT_TO(30, 5, offt),
		   test->success == TRUE ? "success" : "failure");
	
	/* 
     * if (test->success == FALSE) {
	 * 	printf(" -- %s", test->error);
	 * }
	 * printf("\n");
     */
	
	return test->success;
}

#include <string.h>
char **str_split(char *str, char * delim, int *elts)
{
	char	* p1;
	char	**ap;
	char	**argv;
	int		  argc;
	char	* tmp;
	size_t	  memsize;
	
	if (str == NULL | delim == NULL) {
		return (NULL);
	}
	
	argc=1;
	p1 = str;
	while((p1=strpbrk(p1, delim)) != NULL) {
		++p1;
		argc += 1;
		if(p1 == '\0')
			break;
	}

	/* calculate the size of the char *vector then add the string to
	 * the end of that (includeing null space for the empty vector
	 * spot and the NUL char
	 */
	memsize = (sizeof(char *) * (argc + 1)) + (sizeof(char) * (strlen(str) + 1));
	
	argv = (char **)xmalloc(memsize);
	memset(argv, '\0', memsize);
	
	tmp = (char *)argv + (sizeof(char *) * (argc + 1));
	strncpy(tmp, str, strlen(str));
		
	*elts = argc;
	
	for(ap = argv; (*ap = strsep(&tmp, delim)) != NULL;) {
		if (**ap != '\0') {
			++ap;
			if (--argc <=0)
				break;
		}
	}

	return argv;
}

void str_split_free(char ***argv) 
{
	XFREE(*argv);
}

int
zt_unit_run_by_name(struct zt_unit		* unit,
					char				* name)
{
	int	  					  i;
	int						  count;
	char					**targetv;
	struct zt_unit_suite	* unit_suite = NULL;
	struct zt_unit_test		* unit_test = NULL;
	zt_elist				* tmp;
	int						  result = 0;
	int						  len;
	
	targetv = str_split(name, ".", &count);

	if(count <= 0 || count > 2) {
		result = -1;
		goto done;
	}

	len = strlen(targetv[0]);
	zt_elist_for_each(&unit->suites, tmp) {
		unit_suite = zt_elist_data(tmp, struct zt_unit_suite, suite);
		if (len == strlen(unit_suite->name)) {
			if (strncmp(unit_suite->name, targetv[0], len) == 0) {
				break;
			}
		}
		unit_suite = NULL;
	}

	if (unit_suite == NULL) {
		result = -1;
		goto done;
	}

	if (count == 1) {
		zt_unit_run_suite(unit, unit_suite);
		goto done;
	}

	len = strlen(targetv[1]);
	zt_elist_for_each(&unit_suite->tests, tmp) {
		unit_test = zt_elist_data(tmp, struct zt_unit_test, test);
		if (len == strlen(unit_test->name)) {
			if (strncmp(unit_test->name, targetv[1], len) == 0) {
				break;
			}
		}
		unit_test = NULL;
	}
	
	if (unit_test == NULL) {
		result = -1;
		goto done;
	}
	
	zt_unit_run_test(unit, unit_suite, unit_test);
 done:
	str_split_free(&targetv);
	return result;
}

void
zt_unit_list(struct zt_unit *unit)
{
	zt_elist				* tmp;
	zt_elist				* tmp2;
	struct zt_unit_suite	* unit_suite;
	struct zt_unit_test		* unit_test;
	int						  offt;
	
	offt = printf("Test Suites:\n");
	
	zt_elist_for_each(&unit->suites, tmp){
		unit_suite = zt_elist_data(tmp, struct zt_unit_suite, suite);
		printf(BLANK "%s:\n", INDENT_TO(2, 2, 0),
			   unit_suite->name);
		zt_elist_for_each(&unit_suite->tests, tmp2) {
			unit_test = zt_elist_data(tmp2, struct zt_unit_test, test);
			printf(BLANK "- %s\n", INDENT_TO(4, 4, 0),
				   unit_test->name);
		}
	}
	

}


int
zt_unit_main(struct zt_unit				* unit,
			 int						  argc,
			 char						* argv[])
{
	int				  i;
	int				  do_list = FALSE;
	int				  run_tests = TRUE;
	
	struct opt_args   options[] = {
		{ 'h', "help", "This help text", opt_help, NULL, NULL},
		{ 'l', "list", "list suites and test", opt_flag, &do_list, NULL},
		{ 0 }
	};

	if(opts_process(&argc, &argv, options, "[options] <suite | suite.test> ...", TRUE, TRUE) != 0) {
		exit(1);
	}

	if (do_list != FALSE) {
		zt_unit_list(unit);
		run_tests = FALSE;
	}

	if (run_tests != FALSE) {
		
		if (argc != 0) {
			for(i=0; i < argc; i++) {
				if(zt_unit_run_by_name(unit, argv[i]) != 0) {
					printf("Suite or Test \"%s\" not found\n", argv[i]);
				}
			}		
		} else {
			zt_unit_run(unit);
		}
	}
	
	return 0;
}

