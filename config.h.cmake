#ifndef __ZT_CONFIG_H__
#define __ZT_CONFIG_H__

#cmakedefine LIBZT_COVERAGE_TESTING /* undefined */
#cmakedefine PTHREAD_CREATE_JOINABLE /* undefined */
#cmakedefine01 HOST_BIGENDIAN

/********************* types *********************/
#cmakedefine HAVE_UINT64_T
#cmakedefine HAVE_UINT32_T
#cmakedefine HAVE_UINT16_T
#cmakedefine HAVE_UINT8_T

#cmakedefine HAVE_INT8_T
#cmakedefine HAVE_INT16_T
#cmakedefine HAVE_INT32_T
#cmakedefine HAVE_INT64_T

#cmakedefine HAVE_PID_T
#cmakedefine HAVE_SIZE_T

#cmakedefine HAVE_CONST

/********************* services *********************/
#cmakedefine WITH_DMALLOC

#endif /* __ZT_CONFIG_H__ */
