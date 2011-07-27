add_subdirectory(replace)

# ZT setup
set(zt_LIB_SRCS zt_assert.c zt_cfg.c zt_format.c
                zt_log.c zt_malloc.c zt_mem.c zt_progname.c zt_cstr.c
                zt_time.c zt_sha1.c zt_uuid.c zt_ipv4_tbl.c
                zt_ez_mempool.c zt_atexit.c zt_daemon.c zt_threads.c
                zt_threads/zt_threads_pthreads.c zt_opts.c)

set(zt_LIB_HDRS zt_internal.h ${PROJECT_BINARY_DIR}/src/zt_replace.h zt_assert.h zt_cfg.h
                zt_format.h zt_log.h zt_mem.h zt_progname.h
                zt_cstr.h zt_time.h zt_macros.h zt_int.h zt_sha1.h
                zt_uuid.h zt_ez_mempool.h zt_ipv4_tbl.h
                zt_atexit.h zt_daemon.h zt_threads.h zt_opts.h)

set(adt_SRCS zt_array.c zt_bstream.c zt_hash.c zt_llist.c
             zt_ptr_array.c zt_set.c zt_table.c zt_tree.c)
set(adt_HDRS zt_array.h zt_bstream.h zt_hash.h zt_list.h
             zt_llist.h zt_ptr_array.h zt_queue.h zt_set.h zt_stack.h
             zt_table.h zt_tree.h)

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

############## Replacements ###############
if (NOT HAVE_BASENAME)
    LIST(APPEND zt_REPLACE_SRCS replace/basename.c)
endif(NOT HAVE_BASENAME)

if (NOT HAVE_VSYSLOG AND NOT WIN32)
    LIST(APPEND zt_REPLACE_SRCS replace/vsyslog.c)
endif(NOT HAVE_VSYSLOG AND NOT WIN32)

if (NOT HAVE_ASPRINTF)
    LIST(APPEND zt_REPLACE_SRCS replace/asprintf.c)
endif(NOT HAVE_ASPRINTF)

configure_file(replace/zt_replace.h.in ${PROJECT_BINARY_DIR}/src/zt_replace.h)

################ library definitions ################
add_library(zt ${zt_REPLACE_SRCS} ${zt_LIB_SRCS} zt.h ${zt_LIB_HDRS} ${adt_SRCS} ${adt_HDRS} ${log_SRCS} ${log_HDRS} ${cfg_SRCS} ${cfg_HDRS})

add_library(zt_gc ${zt_gc_SRCS})
target_link_libraries(zt_gc zt) # needed to resolve undefined symbols in shared libs
add_library(zt_unit ${zt_unit_SRCS})
target_link_libraries(zt_unit zt) # needed to resolve undefined symbols in shared libs

if(BUILD_SHARED_LIBS)
    set_target_properties(zt PROPERTIES VERSION 2.1.1 SOVERSION 2)
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
set(ZT_INSTALL_HEADERS zt.h zt_unit.h zt_gc.h ${zt_LIB_HDRS} ${zt_gc_HDRS} ${zt_unit_HDRS} ${adt_HDRS})
set(ZT_INSTALL_TARGETS zt zt_gc zt_unit)

if (WIN32)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" CACHE PATH "Windows install prefix" FORCE)
    endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX})
    set(LIBRARY_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX})

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/x64)
    endif()

    install(FILES ${ZT_INSTALL_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(FILES ${log_HDRS} DESTINATION ${CMAKE_INSTALL_PREFIX}/zt_log)
    install(FILES ${cfg_HDRS} DESTINATION ${CMAKE_INSTALL_PREFIX}/zt_cfg)
else (WIN32)
    install(TARGETS ${ZT_INSTALL_TARGETS}
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib)

    install(FILES ${ZT_INSTALL_HEADERS} DESTINATION include/libzt)
    install(FILES ${log_HDRS} DESTINATION include/libzt/zt_log)
    install(FILES ${cfg_HDRS} DESTINATION include/libzt/zt_cfg)
endif(WIN32)
