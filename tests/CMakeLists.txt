set(unit_SRCS array_test.c assert_test.c bstream_test.c cfg_test.c cstr_test.c
    format_test.c gc_test.c include_test.c int_test.c ipv4_tbl_test.c list_test.c
    llist_test.c log_test.c macros_test.c opts_test.c progname_test.c set_test.c
    sha1_test.c stack_test.c table_test.c time_test.c tree_test.c unit_test.c
    uuid_test.c daemon_test.c path_test.c)

# if(WIN32)
#    LIST(REMOVE_ITEM unit_SRCS daemon_test.c time_test.c)
# endif(WIN32)

add_executable(unit_test ${unit_SRCS})
target_link_libraries(unit_test zt_unit zt_gc zt)

# prep the tests
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cfg_test.ini
               ${CMAKE_CURRENT_BINARY_DIR}/cfg_test.ini COPYONLY)

add_custom_target(test
                  COMMAND python ${PROJECT_SOURCE_DIR}/tools/ut_info.py ${CMAKE_CURRENT_BINARY_DIR}/unit_test
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  DEPENDS unit_test)

# rules to delete generate files
# this is not really necessary if you are doing an out-of-source build
set_directory_properties( . ADDITIONAL_MAKE_CLEAN_FILES log.err )
