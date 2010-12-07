if (NOT HAVE_BASENAME)
   LIST(APPEND REPLACE_FILES basename.c)
endif(NOT HAVE_BASENAME)

if (NOT HAVE_VSYSLOG AND NOT WIN32)
    LIST(APPEND REPLACE_FILES vsyslog.c)
endif(NOT HAVE_VSYSLOG AND NOT WIN32)

add_library(replace STATIC dummy.c ${REPLACE_FILES})
configure_file(zt_replace.h.in ${PROJECT_BINARY_DIR}/src/zt_replace.h)
