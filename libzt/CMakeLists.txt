# ZT setup
set(zt_LIB_SRCS zt_assert.c zt_cfg.c zt_format.c
                zt_log.c zt_malloc.c zt_mem.c zt_progname.c zt_cstr.c
                zt_time.c zt_sha1.c zt_uuid.c zt_ipv4_tbl.c
                zt_ez_mempool.c zt_atexit.c zt_daemon.c zt_threads.c
                zt_threads/zt_threads_pthreads.c zt_opts.c)

set(zt_LIB_HDRS zt_internal.h ${PROJECT_BINARY_DIR}/libzt/zt_replace.h zt_assert.h zt_cfg.h
                zt_format.h zt_log.h zt_mem.h zt_progname.h
                zt_cstr.h zt_time.h zt_macros.h zt_int.h zt_sha1.h
                zt_uuid.h zt_ez_mempool.h zt_ipv4_tbl.h
                zt_atexit.h zt_daemon.h zt_threads.h zt_opts.h)

set(adt_SRCS adt/zt_array.c adt/zt_bstream.c adt/zt_hash.c adt/zt_llist.c
             adt/zt_ptr_array.c adt/zt_set.c adt/zt_table.c adt/zt_tree.c)
set(adt_HDRS adt/zt_array.h adt/zt_bstream.h adt/zt_hash.h adt/zt_list.h
             adt/zt_llist.h adt/zt_ptr_array.h adt/zt_queue.h adt/zt_set.h adt/zt_stack.h
             adt/zt_table.h adt/zt_tree.h)

set(log_SRCS zt_log/log_file.c zt_log/log_private.c zt_log/log_stderr.c
             zt_log/log_syslog.c)
set(log_HDRS zt_log/log_file.h zt_log/log_interface.h zt_log/log_private.h
             zt_log/log_stderr.h zt_log/log_syslog.h)

set(cfg_SRCS zt_cfg/cfg_ini.c zt_cfg/cfg_private.c)
set(cfg_HDRS zt_cfg/cfg_ini.h zt_cfg/cfg_interface.h zt_cfg/cfg_private.h)

if(WIN32)
    LIST(APPEND zt_LIB_HDRS zt_win32.h)
    LIST(APPEND zt_LIB_SRCS zt_win32.c)
    LIST(REMOVE_ITEM zt_LIB_SRCS zt_daemon.c zt_threads.c zt_threads/zt_threads_pthreads.c zt_opts.c)
    LIST(REMOVE_ITEM zt_LIB_HDRS zt_daemon.h zt_threads.h zt_opts.h)

    LIST(REMOVE_ITEM log_SRCS zt_log/log_syslog.c)
    LIST(REMOVE_ITEM log_HDRS zt_log/log_syslog.h)

else(WIN32)
    LIST(APPEND zt_LIB_HDRS zt_unix.h)

    if(NOT HAVE_SYSLOG)
        LIST(REMOVE_ITEM log_SRCS zt_log/log_syslog.c)
        LIST(REMOVE_ITEM log_HDRS zt_log/log_syslog.h)
    endif(NOT HAVE_SYSLOG)

    if(NOT WITH_THREADS)
        LIST(REMOVE_ITEM zt_LIB_SRCS zt_threads.c zt_threads/zt_threads_pthreads.c)
        LIST(REMOVE_ITEM zt_LIB_HDRS zt_threads.h)
        if(HAVE_PTHREADS)
            LIST(REMOVE_ITEM zt_LIB_SRCS zt_threads/zt_threads_pthreads.c)
        endif(HAVE_PTHREADS)
    endif(NOT WITH_THREADS)

endif(WIN32)

# UNIT setup
set(zt_unit_SRCS zt_unit.c zt_unit.h)

# GC setup
set(zt_gc_SRCS zt_gc.c zt_gc.h)

################ library definitions ################
add_library(zt ${zt_LIB_SRCS} zt.h ${zt_LIB_HDRS} ${adt_SRCS} ${adt_HDRS} ${log_SRCS} ${log_HDRS} ${cfg_SRCS} ${cfg_HDRS})
target_link_libraries(zt replace)
add_library(zt_gc ${zt_gc_SRCS})
target_link_libraries(zt_gc zt) # needed to resolve undefined symbols in shared libs
add_library(zt_unit ${zt_unit_SRCS})
target_link_libraries(zt_unit zt) # needed to resolve undefined symbols in shared libs

if(BUILD_SHARED_LIBS)
    set_target_properties(zt PROPERTIES VERSION 2.0.1 SOVERSION 2)
    set_target_properties(zt_gc PROPERTIES VERSION 1.0.0 SOVERSION 1)
    set_target_properties(zt_unit PROPERTIES VERSION 1.0.0 SOVERSION 1)
endif(BUILD_SHARED_LIBS)


if(WIN32)
    target_link_libraries(zt ws2_32)
endif(WIN32)

if(HAVE_PTHREADS)
    target_link_libraries(zt ${CMAKE_THREAD_LIBS_INIT})
endif(HAVE_PTHREADS)

#################### installation ####################
install(TARGETS zt zt_gc zt_unit ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
install(FILES zt.h zt_unit.h zt_gc.h DESTINATION include)
install(FILES ${zt_LIB_HDRS} ${zt_gc_HDRS} ${zt_unit_HDRS} DESTINATION include/libzt)
install(FILES ${adt_HDRS} DESTINATION include/libzt/adt)
install(FILES ${log_HDRS} DESTINATION include/libzt/zt_log)
install(FILES ${cfg_HDRS} DESTINATION include/libzt/zt_cfg)
