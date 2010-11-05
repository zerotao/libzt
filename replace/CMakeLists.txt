if (NOT HAVE_BASENAME)
   LIST(APPEND REPLACE_FILES basename.c)
endif(NOT HAVE_BASENAME)

if (NOT HAVE_VSYSLOG)
    LIST(APPEND REPLACE_FILES vsyslog.c)
endif(NOT HAVE_VSYSLOG)

add_library(replace STATIC dummy.c ${REPLACE_FILES})
configure_file(zt_replace.h.in ${PROJECT_BINARY_DIR}/libzt/zt_replace.h)
