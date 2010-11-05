prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@CMAKE_INSTALL_PREFIX@/bin
libdir=@CMAKE_INSTALL_PREFIX@/lib
includedir=@CMAKE_INSTALL_PREFIX@/include/libzt

Name: @PROJECT_NAME@
Description: ZeroTao tools library
# Requires:
# Conflicts:
Version: @PROJECT_VERSION@
Libs: -L${libdir} -lzt
Cflags: -I${includedir}
