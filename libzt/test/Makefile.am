LDADD = ../libzt.la

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
	except_test

#	bytearray_test
noinst_PROGRAMS = 	\
	$(TEST_APPS)	\
	types_test	\
	mem_tools_test	\
	mem_tools_test2

# 	chash_test	
# 	chash64_test	

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

update: test
	cp test.out out.test

test: $(TEST_APPS)
	@echo Beginning Test Run
	@$(RM) -f test.out diff.out assert.out core
	@(for program in $^; do \
	  if [ "$$program" = "opts_test" ]; then \
	    ./$$program --int 1 --bool=t --string hello --flag > opts_test.out; \
	    F=`cat opts_test.out`; \
	    if [ "$$F" = "integer = 1, bool = 1, string = hello flag = 1" ]; then \
	      echo "opts[1]:                                     => success" | tee -a test.out; \
	    fi; \
	  elif [ "$$program" = "assert_test" ]; then \
	    ./$$program  > assert.out 2>&1 ; \
	    F=`cat assert.out | awk -F\[ '{ print $$1 }'`; \
	    if [ "$$F" = "Error: Assertion \"1==0\" failed at: assert_test.c" ]; then \
	      echo "assert[1]:                                   => success" | tee -a test.out; \
	    else \
	      echo "assert[1]:                                   => failure" | tee -a test.out; \
	    fi; \
	    if [ -f core -o -d /cores ]; then \
	      echo "assert[2]:                                   => success" | tee -a test.out; \
	      rm -f core; \
            else \
              echo "assert[2]:                                   => failure" | tee -a test.out; \
	    fi; \
	  else \
	    ./$$program 2>&1 | tee -a test.out; \
	    if [ -f core ]; then \
	      echo "$$program failed with a core dump"; \
	      exit 255; \
	    fi \
	  fi \
	done)
	@(diff test.out out.test > diff.out 2>&1; \
	  F=`grep Fail test.out`; \
	  if ! [ -s diff.out -o -n "$F" ]; then \
	    echo "All Tests Passed!"; \
	  else \
	    echo "Some Tests Failed!  See test.out for details."; \
	  fi \
	)

noinst_DATA = \
	out.test \
	cfg_test.ini


include $(top_srcdir)/Makefile.extra

CLEANFILES += \
	diff.out \
	test.out \
	assert.out \
	log.err

