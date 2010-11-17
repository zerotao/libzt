cmake_minimum_required(VERSION 2.8)

project(libzt)
set(PROJECT_VERSION 1.1.0)

# set(CMAKE_VERBOSE_MAKEFILE ON)

if(CMAKE_COMPILER_IS_GNUCC)
  # set(CMAKE_BUILD_TYPE distribution)
  # set(CMAKE_C_FLAGS_DISTRIBUTION "-Wall -Wextra -O2")
  set(CMAKE_C_FLAGS "-Wall -Wextra -Werror")
  if(APPLE)
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshorten-64-to-32")
  endif(APPLE)
endif(CMAKE_COMPILER_IS_GNUCC)


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
message("MODULE_PATH ${CMAKE_MODULE_PATH}")

############################################################

########## Check for System Packages known by CMAKE ##########
find_package(Threads)

################## Check for System Headers ##################
include(CheckDefIncludeFiles)

# usage: CHECK_DEF_INCLUDE_FILES (<header> <RESULT_VARIABLE> )

check_def_include_files( assert.h HAVE_ASSERT_H )
check_def_include_files( dlfcn.h HAVE_DLFCN_H )
check_def_include_files( inttypes.h HAVE_INTTYPES_H )
check_def_include_files( memory.h HAVE_MEMORY_H )
check_def_include_files( stdbool.h HAVE_STDBOOL_H )
check_def_include_files( stdint.h HAVE_STDINT_H )
check_def_include_files( stdlib.h HAVE_STDLIB_H )
check_def_include_files( strings.h HAVE_STRINGS_H )
check_def_include_files( string.h HAVE_STRING_H )
check_def_include_files( syslog.h HAVE_SYSLOG_H )
check_def_include_files( sys/stat.h HAVE_SYS_STAT_H )
check_def_include_files( sys/types.h HAVE_SYS_TYPES_H )
check_def_include_files( sys/wait.h HAVE_SYS_WAIT_H )
check_def_include_files( unistd.h HAVE_UNISTD_H )
check_def_include_files( libgen.h HAVE_LIBGEN_H )
check_def_include_files( ctype.h HAVE_CTYPE_H )

################## Check for System Functions ##################
include(CheckFunctionExists)
# usage: CHECK_FUNCTION_EXISTS(function variable)

include(CheckDefFunctionExists)

check_def_function_exists( openlog HAVE_SYSLOG )

check_def_function_exists( basename HAVE_BASENAME )
check_def_function_exists( pthread_create HAVE_PTHREAD )
check_def_function_exists( break HAVE_BREAK )
check_def_function_exists( bzero HAVE_BZERO )
check_def_function_exists( calloc HAVE_CALLOC )
check_def_function_exists( memset HAVE_MEMSET )
check_def_function_exists( strchr HAVE_STRCHR )
check_def_function_exists( strrchr HAVE_STRRCHR )
check_def_function_exists( strdup HAVE_STRDUP )
check_def_function_exists( vsyslog HAVE_VSYSLOG )
check_def_function_exists( getopt_long HAVE_GETOPT_LONG )

################## Check for System Symbols ##################
include(CheckSymbolExists)
# usage: CHECK_SYMBOL_EXISTS(symbol headers variable)

################## Check for System Types ##################
include(CheckTypeSize)
# SET(CMAKE_EXTRA_INCLUDE_FILES header)
# CHECK_TYPE_SIZE(type variable)
# SET(CMAKE_EXTRA_INCLUDE_FILES)

# not available
# include(CheckPrototypeExists)
# usage: CHECK_PROTOTYPE_EXISTS(function headers variable)

################## Check if code compiles ##################
include(CheckCSourceCompiles)
# usage: CHECK_C_SOURCE_COMPILES(source variable)

CHECK_C_SOURCE_COMPILES( "int main(int argc, char * argv) { long long ll; }" HAVE_LONG_LONG )
if(HAVE_LONG_LONG)
    ADD_DEFINITIONS(-DHAVE_LONG_LONG)
endif(HAVE_LONG_LONG)

CHECK_C_SOURCE_COMPILES( "int main(int argc, char * argv) { long double ld; }" HAVE_LONG_DOUBLE )
if(HAVE_LONG_DOUBLE)
    ADD_DEFINITIONS(-DHAVE_LONG_DOUBLE)
endif(HAVE_LONG_DOUBLE)

add_definitions(-DHAVE_CONFIG_H)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/libzt
                    ${CMAKE_CURRENT_SOURCE_DIR}/libzt)

configure_file(config.h.cmake libzt/config.h)

# subdirectories
add_subdirectory(tools)
add_subdirectory(replace)
add_subdirectory(libzt)
add_subdirectory(examples)
add_subdirectory(tests)
add_subdirectory(doc)
