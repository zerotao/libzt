#LDADD = ../libzt/libzt.la 
LDADD = ../libzt/.libs/libzt.a

DEFS = @DEFS@
AM_CPPFLAGS = -I$(top_builddir) -I$(top_builddir)/libzt -I$(top_srcdir) -I$(top_srcdir)/libzt -I$(srcdir)

unit_test_LDADD = ../libzt/.libs/libzt_unit.a ../libzt/.libs/libzt.a

TEST_APPS = 		\
	unit_test

noinst_PROGRAMS = 	\
	$(TEST_APPS)

unit_test_SOURCES = 	\
	assert_test.c	\
	unit_test.c	\
	array_test.c	\
	table_test.c	\
	bstream_test.c	\
	cfg_test.c	\
	coro_test.c	\
	cothread_test.c	\
	cstr_test.c	\
	event_test.c	\
	except_test.c	\
	format_test.c	\
	gc_test.c	\
	int_test.c	\
	list_test.c	\
	llist_test.c	\
	log_test.c	\
	macros_test.c	\
	opts_test.c	\
	progname_test.c \
	set_test.c	\
	stack_test.c	\
	time_test.c	\
	tree_test.c

#	msg_queue_test.c
#	bytearray_test.c
# 	adt_test.c

EXTRA_DIST = 		\
	cfg_test.ini

noinst_DATA = \
	cfg_test.ini

DUMMY_TEST = $(addprefix $(srcdir)/,$(noinst_DATA))

test.out:
	test

test: $(TEST_APPS)
	@echo Beginning Test Run
	@cp -p $(DUMMY_TEST) $(top_builddir)/tests
	@$(RM) -f test.out diff.out assert.out core
	@(for program in $^; do \
	  ./$$program > $$program.out 2>&1;		\
	   EXIT=$$?; 					\
	   cat $$program.out;				\
	   cat $$program.out >> test.out;		\
	   if [ ! $$EXIT == 0 ]; then 			\
	     echo "$$program exited with a non zero exitcode: $$EXIT"; \
	     exit 255; \
	   fi \
	done)
	@(python ../tools/ut_info.py test.out)

include $(top_srcdir)/Makefile.extra

CLEANFILES += \
	diff.out \
	test.out \
	assert.out \
	log.err

