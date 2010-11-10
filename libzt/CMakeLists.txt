# ZT setup
set(zt_LIB_SRCS zt_assert.c zt_cfg.c  zt_except.c zt_format.c
                zt_log.c zt_malloc.c zt_mem.c zt_progname.c zt_cstr.c
                zt_time.c zt_sha1.c zt_uuid.c zt_exceptions.c zt_ipv4_tbl.c
                zt_ez_mempool.c zt_atexit.c)

set(zt_LIB_HDRS zt.h zt_internal.h zt_replace.h zt_assert.h zt_cfg.h
                zt_except.h zt_format.h zt_log.h zt_mem.h zt_progname.h
                zt_cstr.h zt_time.h zt_macros.h zt_int.h zt_sha1.h
                zt_uuid.h zt_exceptions.h zt_ez_mempool.h zt_ipv4_tbl.h
                zt_atexit.h)


if(NOT WIN32)
    LIST(APPEND zt_LIB_SRCS zt_daemon.c)
    LIST(APPEND zt_LIB_HDRS zt_daemon.h)

    if(HAVE_SYSLOG)
        LIST(APPEND zt_LIB_SRCS zt_log/log_syslog.c)
        LIST(APPEND zt_LIB_HDRS zt_log/log_syslog.h)
    endif(HAVE_SYSLOG)

    if(HAVE_PTHREAD)
        LIST(APPEND zt_LIB_SRCS zt_threads.c)
        LIST(APPEND zt_LIB_HDRS zt_threads.h)
        LIST(APPEND zt_LIB_HDRS zt_threads/zt_threads_pthreads.c)
    endif(HAVE_PTHREAD)

    LIST(APPEND zt_LIB_SRCS zt_opts.c)
    LIST(APPEND zt_LIB_HDRS zt_opts.h)

    LIST(APPEND zt_LIB_HDRS zt_unix.h)
else(NOT WIN32)
    LIST(APPEND zt_LIB_HDRS zt_win32.h)
endif(NOT WIN32)


file(GLOB adt_SRCS adt/*.c)
file(GLOB adt_HDRS adt/*.h)

file(GLOB log_SRCS zt_log/*.c)
file(GLOB log_HDRS zt_log/*.h)

file(GLOB cfg_SRCS zt_cfg/*.c)
file(GLOB cfg_HDRS zt_cfg/*.h)

# UNIT setup
set(zt_unit_SRCS zt_unit.c zt_unit.h)

# GC setup
set(zt_gc_SRCS zt_gc.c zt_gc.h)

################ library definitions ################
add_library(zt SHARED ${zt_LIB_SRCS} ${zt_LIB_HDRS} ${adt_SRCS} ${adt_HDRS} ${log_SRCS} ${log_HDRS} ${cfg_SRCS} ${cfg_HDRS})
set_target_properties(zt PROPERTIES VERSION 2.0.1 SOVERSION 2)
target_link_libraries(zt replace)

# add_library(zt-static STATIC ${zt_LIB_SRCS} ${zt_LIB_HDRS} ${adt_SRCS} ${adt_HDRS} ${log_SRCS} ${log_HDRS} ${cfg_SRCS} ${cfg_HDRS})
# set_target_properties(zt-static PROPERTIES VERSION 2.0.1 SOVERSION 1 OUTPUT_NAME "zt")
# target_link_libraries(zt replace)

add_library(zt_gc SHARED ${zt_gc_SRCS})
set_target_properties(zt_gc PROPERTIES VERSION 1.0.0 SOVERSION 1)
target_link_libraries(zt_gc zt) # needed to resolve undefined symbols in shared libs

# add_library(zt_gc-static STATIC ${zt_gc_SRCS})
# set_target_properties(zt_gc-static PROPERTIES VERSION 1.0.0 SOVERSION 1 OUTPUT_NAME "zt_gc")

add_library(zt_unit SHARED ${zt_unit_SRCS})
set_target_properties(zt_unit PROPERTIES VERSION 1.0.0 SOVERSION 1)
target_link_libraries(zt_unit zt) # needed to resolve undefined symbols in shared libs

# add_library(zt_unit-static STATIC ${zt_unit_SRCS})
# set_target_properties(zt_unit-static PROPERTIES VERSION 1.0.0 SOVERSION 1 OUTPUT_NAME "zt_gc")

#################### installation ####################
install(TARGETS zt zt_gc zt_unit ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
install(FILES ${zt_LIB_HDRS} ${zt_gc_HDRS} ${zt_unit_HDRS} DESTINATION include/libzt)
install(FILES ${adt_HDRS} DESTINATION include/libzt/adt)
install(FILES ${log_HDRS} DESTINATION include/libzt/zt_log)
install(FILES ${cfg_HDRS} DESTINATION include/libzt/zt_cfg)
