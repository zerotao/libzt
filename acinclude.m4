dnl Your own macros go here
AC_DEFUN([AC_WITH_OPTFLAGS],
[ AC_MSG_CHECKING(whether to enable optimization flags)
  AC_ARG_WITH(optflags,
  [  --with-optflags[=flags]  Enable optimization flags (default).
  --without-optflags      Disable optimization flags. ],
  [
	case "$with_optflags" in
	     no)
		AC_MSG_RESULT(no)
		;;
	    yes)
	        AC_MSG_RESULT(yes)
		CFLAGS="$CFLAGS -O3"
		;;
	      *)
	        AC_MSG_RESULT($with_optflags)
		CFLAGS="$CFLAGS $with_optflags"
		;;
	esac
  ],
  [ 
    AC_MSG_RESULT(yes)
    CFLAGS="$CFLAGS -O3"
  ])
])


AC_DEFUN([AC_WITH_DEBUG],
[ AC_MSG_CHECKING(whether to enable debugging)
  AC_ARG_WITH(debug,
  [  --with-debug[=level]    Enable debugging (default only -g).
  --without-debug	  Disable debugging. ],
  [
	case "$with_debug" in
	     no)
		AC_MSG_RESULT(no)
		CFLAGS="$CFLAGS"
		;;
	     yes)
	        AC_MSG_RESULT(-DDEBUG)
		CFLAGS="$CFLAGS -g -DDEBUG"
		;;
	     *)
	        AC_MSG_RESULT(-DDEBUG$with_debug)
		CFLAGS="$CFLAGS -g -DDEBUG$with_debug"
	        ;;
	esac
  ],
  [ 
    AC_MSG_RESULT(yes)
    CFLAGS="$CFLAGS -g"
  ])
])

AC_DEFUN([AC_WITH_PROFILING],
[ AC_MSG_CHECKING(whether to enable/disable profiling)
  AC_ARG_WITH(profiling,
  [  --with-profiling[=flags]  Enable profiling.
  --without-profiling	  Disable profiling(default). ],
  [
	case "$with_profiling" in
	     no)
		AC_MSG_RESULT(no)
		;;
	     yes)
	        AC_MSG_RESULT(yes)
		CFLAGS="$CFLAGS -pg"
		;;
	     *)
	        AC_MSG_RESULT(yes)
		CFLAGS="$CFLAGS -pg $with_profiling"
	        ;;
	esac
  ],
  [ AC_MSG_RESULT(no)
  ])
])

AC_DEFUN([AC_WITH_DIST_RELEASE],
 [AC_MSG_CHECKING(whether to add a release variable)
  AC_ARG_WITH(release,
  [  --with-release[=REL]    Enable release REL. 
  --without-release       Disable release versioning (default).],
  [
	case "$with_release" in
	     no)
		AC_MSG_RESULT(no)
		RELEASE_VERSION=
		;;
	     yes)
		AC_MSG_RESULT(yes release=0)
		RELEASE_VERSION=0
		;;
	     *)
		AC_MSG_RESULT($with_release)
		;;
	esac
  ],
  [
	AC_MSG_RESULT(no)
	RELEASE_VERSION=
  ])
  AC_SUBST(RELEASE_VERSION)
])


dnl AX_MSG_RESULT_YN(a)
dnl results "yes" iff a==1, "no" else
AC_DEFUN(AX_MSG_RESULT_YN, [x=no
test "x$1" = "x1" && x=yes
AC_MSG_RESULT($x)])

dnl AX_EGREP(expr, file, action-if-found, action-if-not-found)
dnl egrep for expr in file
AC_DEFUN(AX_EGREP, [dnl
changequote(, )dnl
  if egrep "$1" $2 >/dev/null 2>&1; then
changequote([, ])dnl
  ifelse([$3], , :, [$3])
ifelse([$4], , , [else
  $4
])dnl
fi
])

dnl AX_MACRO_STR(macro, file)
dnl grep string #define out of header
dnl result value in $ax_result
AC_DEFUN(AX_MACRO_STR,
[AC_REQUIRE_CPP()dnl
cat > conftest.$ac_ext <<EOF
[#]line __oline__ "configure"
#include "confdefs.h"
#include <$2>
[$1]
EOF
(eval "$ac_cpp conftest.$ac_ext") 2>&AC_FD_CC |\
 sed -n -e 's/^"\(.*\)".*$/\1/p' >conftest
changequote(, )dnl
set "X" "`cat conftest`"
changequote([, ])dnl
ax_result=[$]2
rm -f conftest*
])

AC_DEFUN(AX_SAVE, [eval $1_AX_[$ax_stack]=[\"]$$1[\"]])
AC_DEFUN(AX_RESTORE, [eval $1=[\"\$]{$1_AX_[$ax_stack]}[\"]])

dnl AX_PUSH
dnl save the contents of CFLAGS etc.
AC_DEFUN(AX_PUSH, [dnl
ax_stack=`expr 0$ax_stack + 1`
AX_SAVE(CFLAGS)
AX_SAVE(CPPFLAGS)
AX_SAVE(LDFLAGS)
AX_SAVE(LIBS)
])

dnl AX_POP
dnl restore the contents of CFLAGS etc.
AC_DEFUN(AX_POP, [dnl
AX_RESTORE(CFLAGS)
AX_RESTORE(CPPFLAGS)
AX_RESTORE(LDFLAGS)
AX_RESTORE(LIBS)
ax_stack=`expr 0$ax_stack - 1`
])
