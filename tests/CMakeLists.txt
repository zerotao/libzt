FILE(GLOB unit_SRCS *.c)

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
