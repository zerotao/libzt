LDADD = ../libzt.la
CFLAGS = -I$(top_srcdir)

TEST_APPS = 		\
	array_test	\
	cstr_test 	\
	macros_test 	\
	assert_test 	\
	progname_test 	\
	log_test 	\
	cfg_test 	\
	opts_test 	\
	list_test	\
	table_test	\
	format_test	\
	except_test	\
	time_test	\
	stack_test	\
	bstream_test	\
	set_test	\
	tree_test	\
	event_test	\
	coro_test	\
	cothread_test

#	bytearray_test
noinst_PROGRAMS = 	\
	$(TEST_APPS)	\
	types_test	\
	mem_tools_test	\
	mem_tools_test2

# 	chash_test	
# 	chash64_test	

event_test_SOURCES = \
	event_test.c

tree_test_SOURCES = \
	tree_test.c

time_test_SOURCES = \
	time_test.c

bstream_test_SOURCES = \
	bstream_test.c

types_test_SOURCES = 	\
	types_test.c

# chash_test_SOURCES = 
# 	chash_test.c

# chash64_test_SOURCES = 
# 	chash64_test.c

array_test_SOURCES = \
	array_test.c

format_test_SOURCES = \
	format_test.c

# bytearray_test_SOURCES = \
# 	bytearray_test.c

table_test_SOURCES = 	\
	table_test.c

mem_tools_test2_SOURCES =\
	mem_tools_test2.c

mem_tools_test_SOURCES =\
	mem_tools_test.c

list_test_SOURCES =	\
	list_test.c

except_test_SOURCES = 	\
	except_test.c

opts_test_SOURCES = 	\
	opts_test.c

cfg_test_SOURCES = 	\
	cfg_test.c

log_test_SOURCES = 	\
	log_test.c

progname_test_SOURCES = \
	progname_test.c

cstr_test_SOURCES = 	\
	cstr_test.c

assert_test_SOURCES = 	\
	assert_test.c

macros_test_SOURCES = 	\
	macros_test.c

noinst_HEADERS = 	\
	test.h

EXTRA_DIST = 		\
	out.test 	\
	cfg_test.ini

noinst_DATA = \
	out.test \
	cfg_test.ini

DUMMY_TEST = $(addprefix $(srcdir)/,$(noinst_DATA))

update: test.out
	cp test.out $(top_srcdir)/libzt/test/out.test

test.out:
	test

test: $(TEST_APPS)
	@echo Beginning Test Run
	@cp -p $(DUMMY_TEST) $(top_builddir)/libzt/test
	@$(RM) -f test.out diff.out assert.out core
	@(for program in $^; do \
	  if [ "$$program" = "opts_test" ]; then \
	    ./$$program --int 1 --bool=t --string hello --flag > opts_test.out; \
	    F=`cat opts_test.out`; \
	    if [ "$$F" = "integer = 1, bool = 1, string = hello flag = 1" ]; then \
	      echo "opts[0]:                                                    => success" | tee -a test.out; \
	    fi; \
	  elif [ "$$program" = "assert_test" ]; then \
	    ./$$program  > assert.out 2>&1 ; \
	    F=`head -1 assert.out | awk -F\[ '{ print $$1 }'`; \
	    if [ "$$F" = "Error: Assertion \"1==0\" failed at: assert_test.c" ]; then \
	      echo "assert[0]:                                                  => success" | tee -a test.out; \
	    else \
	      echo "assert[0]:                                                  => failure" | tee -a test.out; \
	    fi; \
	    if [ -f core -o -d /cores ]; then \
	      echo "assert[1]:                                                  => success" | tee -a test.out; \
	      rm -f core; \
            else \
              echo "assert[1]:                                                  => failure" | tee -a test.out; \
	    fi; \
	  else \
	    ./$$program > $$program.out 2>&1		\
	    EXIT=$$?; 					\
	    cat $$program.out;				\
	    cat $$program.out >> test.out; 		\
	    if [ ! $$EXIT == 0 ]; then 			\
	      echo "$$program exited with a non zero exitcode: $$EXIT"; \
	      exit 255; \
	    fi \
	  fi \
	done)
	@(diff test.out out.test > diff.out 2>&1; \
	  N=`cat test.out | wc -l | awk '{ print $$1 }'`; 	\
	  N2=`cat out.test | wc -l | awk '{ print $$1 }'`; 	\
	  F=`grep failure test.out`;				\
	  FN=`echo $$F | wc -l | awk '{ print $$1 }'`;	 	\
	  echo; 						\
	  if [ -n "$$F" ]; then 				\
	    echo "$$FN of $$N tests Failed!";			\
	    echo;						\
	    echo "Failed tests were:";				\
	    echo "$$F";						\
	  else							\
	    echo "$$N of $$N tests Passed!"; 			\
	  fi;							\
	  if [ ! $$N == $$N2 ]; then				\
	    echo "$$N tests run but $$N2 expected.";		\
	  fi;							\
	  if [ -s diff.out ]; then				\
	    echo;						\
	    echo "Test output does not match expected output.";	\
	    echo "See test.out and diff.out for details."; \
	  fi; \
	)

include $(top_srcdir)/Makefile.extra

CLEANFILES += \
	diff.out \
	test.out \
	assert.out \
	log.err

