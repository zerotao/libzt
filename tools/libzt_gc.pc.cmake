prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@CMAKE_INSTALL_PREFIX@/bin
libdir=@CMAKE_INSTALL_PREFIX@/lib
includedir=@CMAKE_INSTALL_PREFIX@/include/libzt

Name: @PROJECT_NAME@_gc
Description: ZeroTao GC library
# Requires:
# Conflicts:
Version: @PROJECT_VERSION@
Libs: -L${libdir} -lzt -lzt_gc
Cflags: -I${includedir}
