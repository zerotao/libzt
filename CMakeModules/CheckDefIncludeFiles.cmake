include (CheckIncludeFiles)

MACRO(CHECK_DEF_INCLUDE_FILES INCLUDE VARIABLE)
    CHECK_INCLUDE_FILES(${INCLUDE} ${VARIABLE})
    IF(${VARIABLE})
        ADD_DEFINITIONS(-D${VARIABLE})
    ENDIF(${VARIABLE})
ENDMACRO(CHECK_DEF_INCLUDE_FILES)
