cmake_minimum_required(VERSION 2.8)

project(libzt)
set(PROJECT_VERSION 1.6.0)

# set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
############################################################

########## Check for System Packages known by CMAKE ##########
find_package(Threads)
if (CMAKE_USE_PTHREADS_INIT)
    # FIXME: require pthreads
    SET(HAVE_PTHREADS 1)
    ADD_DEFINITIONS(-DHAVE_PTHREADS)

    SET(WITH_THREADS 1)
    ADD_DEFINITIONS(-DWITH_THREADS)
endif (CMAKE_USE_PTHREADS_INIT)

include(BaseConfig)

################## Check for System Headers ##################
include(CheckDefIncludeFiles)

# usage: CHECK_DEF_INCLUDE_FILES (<header> <RESULT_VARIABLE> )

check_def_include_files( assert.h HAVE_ASSERT_H )
check_def_include_files( dlfcn.h HAVE_DLFCN_H )
check_def_include_files( inttypes.h HAVE_INTTYPES_H )
check_def_include_files( memory.h HAVE_MEMORY_H )
check_def_include_files( stdbool.h HAVE_STDBOOL_H )
check_def_include_files( stdlib.h HAVE_STDLIB_H )
check_def_include_files( stdint.h HAVE_STDINT_H )
check_def_include_files( strings.h HAVE_STRINGS_H )
check_def_include_files( string.h HAVE_STRING_H )
check_def_include_files( syslog.h HAVE_SYSLOG_H )
check_def_include_files( sys/stat.h HAVE_SYS_STAT_H )
check_def_include_files( sys/types.h HAVE_SYS_TYPES_H )
check_def_include_files( sys/wait.h HAVE_SYS_WAIT_H )
check_def_include_files( unistd.h HAVE_UNISTD_H )
check_def_include_files( libgen.h HAVE_LIBGEN_H )
check_def_include_files( ctype.h HAVE_CTYPE_H )
check_def_include_files( errno.h HAVE_ERRNO_H )

# windows specific headers
CHECK_INCLUDE_FILES(process.h HAVE_PROCESS_H)

################## Check for System Functions ##################
include(CheckFunctionExists)
# usage: CHECK_FUNCTION_EXISTS(function variable)

include(CheckDefFunctionExists)

check_def_function_exists( basename HAVE_BASENAME )
check_def_function_exists( break HAVE_BREAK )
check_def_function_exists( bzero HAVE_BZERO )
check_def_function_exists( calloc HAVE_CALLOC )
check_def_function_exists( memset HAVE_MEMSET )
check_def_function_exists( strchr HAVE_STRCHR )
check_def_function_exists( strrchr HAVE_STRRCHR )
check_def_function_exists( strdup HAVE_STRDUP )
check_def_function_exists( vsyslog HAVE_VSYSLOG )
check_def_function_exists( openlog HAVE_OPENLOG )
check_def_function_exists( getopt_long HAVE_GETOPT_LONG )

if(HAVE_VSYSLOG AND HAVE_OPENLOG)
	set(HAVE_SYSLOG 1)
endif()

include(CheckDefSymbolExists)
check_def_symbol_exists( va_copy stdarg.h HAVE_VA_COPY )
check_def_symbol_exists( asprintf stdio.h HAVE_ASPRINTF )
check_def_symbol_exists( vasprintf stdio.h HAVE_VASPRINTF )
check_def_symbol_exists( va_copy stdarg.h HAVE_VA_COPY )
check_def_symbol_exists( getpid process.h HAVE_WIN_GETPID )

################## Check for System Symbols ##################
include(CheckSymbolExists)
# usage: CHECK_SYMBOL_EXISTS(symbol headers variable)

################## Check for System Attributes ##################
include(TestBigEndian)
test_big_endian(HOST_BIGENDIAN)

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

CHECK_C_SOURCE_COMPILES( "int main() { long long ll = 1L; return 0; }" HAVE_LONG_LONG )
if(HAVE_LONG_LONG)
    ADD_DEFINITIONS(-DHAVE_LONG_LONG)
endif(HAVE_LONG_LONG)

CHECK_C_SOURCE_COMPILES( "int main() { long double ld = 1.0; return 0; }" HAVE_LONG_DOUBLE )
if(HAVE_LONG_DOUBLE)
    ADD_DEFINITIONS(-DHAVE_LONG_DOUBLE)
endif(HAVE_LONG_DOUBLE)

add_definitions(-DHAVE_CONFIG_H)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src
                    ${CMAKE_CURRENT_SOURCE_DIR}/src
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/adt
                    ${CMAKE_CURRENT_SOURCE_DIR}/compat)

if(WIN32)
    # try to find requirements in the install directory as well
    include_directories(${CMAKE_INSTALL_PREFIX}/include
                        ${CMAKE_INSTALL_PREFIX})
endif(WIN32)

configure_file(config.h.cmake src/zt_config.h)

# subdirectories
add_subdirectory(tools)
add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)
add_subdirectory(doc)
