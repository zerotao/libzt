
add_executable(mem_pools mem_pools.c test.h)
target_link_libraries(mem_pools zt)

add_executable(mem_timing mem_timing.c test.h)
target_link_libraries(mem_timing zt)

add_executable(types types.c test.h)
target_link_libraries(types zt)

if(HAVE_PTHREAD)
add_executable(threads threads.c test.h)
target_link_libraries(threads zt)
endif(HAVE_PTHREAD)
