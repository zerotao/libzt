
add_executable(cpp_test cpp_test.cpp test.h)
target_link_libraries(cpp_test zt zt_unit)

add_executable(mem_pools mem_pools.c test.h)
target_link_libraries(mem_pools zt)

add_executable(mem_timing mem_timing.c test.h)
target_link_libraries(mem_timing zt)

add_executable(types types.c test.h)
target_link_libraries(types zt)

add_executable(abort abort.c test.h)
target_link_libraries(abort zt)

add_executable(hexdump hexdump.c)
target_link_libraries(hexdump zt)

add_executable(log_syslog log_syslog.c)
target_link_libraries(log_syslog zt)

add_executable(base64_speed base64_speed.c)
target_link_libraries(base64_speed zt)

if(HAVE_PTHREADS)
add_executable(threads threads.c test.h)
target_link_libraries(threads zt)
endif(HAVE_PTHREADS)
