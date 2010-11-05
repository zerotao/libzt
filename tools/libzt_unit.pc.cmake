prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@CMAKE_INSTALL_PREFIX@/bin
libdir=@CMAKE_INSTALL_PREFIX@/lib
includedir=@CMAKE_INSTALL_PREFIX@/include/libzt

Name: @PROJECT_NAME@_unit
Description: ZeroTao unit test library
# Requires:
# Conflicts:
Version: @PROJECT_VERSION@
Libs: -L${libdir} -lzt -lzt_unit
Cflags: -I${includedir}
