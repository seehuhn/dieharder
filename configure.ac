#==================================================================
# Process this file with autoconf to produce a configure script.
# Make changes to the version number(s) in particular ONLY here.
#==================================================================

AC_PREREQ(2.59)

#==================================================================
# Be sure to set DIEHARDER_LT_VERSION below at the same time
# you change this:
#==================================================================
AC_INIT([dieharder], [3.31.1])

AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_SRCDIR([dieharder.abs])
RELEASE=1
AC_SUBST(RELEASE)

AC_CONFIG_HEADERS([config.h])
# AM_MAINTAINER_MODE

AC_DEFINE([RELEASED], [], [Description])

#==================================================================
# Library versioning (current:revision:age)
# See the libtool manual for an explanation of the numbers.
# NOTE WELL!  One MUST bump LT_VERSION along with AC_INIT,
# apparently.
# 
# dieharder-1.0    libdieharder 0:0:0
#==================================================================
# This command "should" keep libtool up to date.
AC_CONFIG_MACRO_DIR([m4])
LT_INIT
AC_SUBST([LIBTOOL_DEPS])
DIEHARDER_LT_VERSION="3:31:1"
AC_SUBST(DIEHARDER_LT_VERSION)
DIEHARDER_LIB_VERSION=3.31.1
AC_SUBST(DIEHARDER_LIB_VERSION)

case "$VERSION" in
    *+)
        AC_DEFINE(RELEASED,0)
        ;;
    *)
        AC_DEFINE(RELEASED,1)
        ;;
esac
AC_SUBST(RELEASED)      

#==================================================================
# Check for which system.
#==================================================================
AC_CANONICAL_HOST

#==================================================================
# Checks for programs.
#==================================================================
AC_LANG(C)
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_MAKE_SET

#==================================================================
# OK, these five macros prevent my personal path from appearing
# in the default Makefile, and actually should make it work to
# rebuilt itself even if somebody completely blows off running
# ./autogen.sh.  And it ALMOST WORKS
#==================================================================
ACLOCAL="${SHELL} ./missing --run aclocal"
ACTAR="${SHELL} ./missing --run tar"
AUTOCONF="${SHELL} ./missing --run autoconf;echo \"Run ./configure\";exit"
AUTOHEADER="${SHELL} ./missing --run autoheader"
AUTOMAKE="${SHELL} ./missing --run automake --add-missing --copy --gnu"

AC_SUBST(ACLOCAL)
AC_SUBST(ACTAR)
AC_SUBST(AUTOCONF)
AC_SUBST(AUTOHEADER)
AC_SUBST(AUTOMAKE)

#==================================================================
# Disable unnecessary libtool tests for c++,fortran,java
#==================================================================
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])
define([AC_LIBTOOL_LANG_GCJ_CONFIG], [:])
AC_PROG_LIBTOOL

#==================================================================
# Check compiler features
#==================================================================
AC_TYPE_SIZE_T
# AC_C_CONST
AC_C_VOLATILE
AC_C_INLINE

DIEHARDER_CFLAGS="-I$includedir"
dieharder_CFLAGS="-std=c99 -Wall -pedantic -I$includedir"
libdieharder_lo_CFLAGS="-Wall -pedantic -I$includedir"
DIEHARDER_LIBS="-L$libdir -ldieharder"
ACLOCAL_AMFLAGS="-I m4"

AC_SUBST(DIEHARDER_CFLAGS)
AC_SUBST(dieharder_CFLAGS)
AC_SUBST(libdieharder_lo_CFLAGS)
AC_SUBST(DIEHARDER_LIBS)
AC_SUBST(ACLOCAL_AMFLAGS)

#==================================================================
# Checks for libraries, and headers.  Test for dependency libraries
# FIRST in reverse order that you need -lwhatever to appear on
# compile line as it accumulates libraries to build e.g.
#   -lgsl -lgslcblas
# for the SECOND test, required (in that order) to succeed.
#==================================================================
AC_CHECK_HEADER([gsl/gsl_sf_gamma.h],,[AC_MSG_ERROR([Couldn't find GSL headers.  Please install the gsl-devel package.])])
AC_CHECK_LIB([gslcblas], [main],,[AC_MSG_ERROR([Couldn't find libgsl. Please install the gsl package.])])
AC_CHECK_LIB([gsl],[gsl_sf_gamma])


#==================================================================
# Check if we're a little-endian or a big-endian system, needed by
# brg_endian.h in the build of rng_threefish.  This is a very
# certain test, and therefore is checked FIRST in this header file.
#==================================================================
AC_DEFUN([AC_C_ENDIAN],
[AC_CACHE_CHECK(for endianness, ac_cv_c_endian,
[
  AC_RUN_IFELSE(
    [AC_LANG_PROGRAM([], [dnl
        long val = 1;
        char *c = (char *) &val;
        exit(*c == 1);
    ])
  ],[
    ac_cv_c_endian=big
  ],[
    ac_cv_c_endian=little
  ])
])
if test $ac_cv_c_endian = big; then
  AC_SUBST(LITTLE_ENDIAN,0) 
fi
if test $ac_cv_c_endian = little; then
  AC_SUBST(LITTLE_ENDIAN,1) 
fi
])

AC_C_ENDIAN
 

#==================================================================
# Checks for typedefs, structures, and compiler characteristics.
#==================================================================
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME

#==================================================================
# Files that configure needs to process with this so that they are
# all happy and macro-replaced.
#==================================================================
AC_CONFIG_FILES([
 dieharder_version.h 
 dieharder.spec
 dieharder.html
 dieharder-config
 dieharder/Makefile 
 include/Makefile
 libdieharder/Makefile 
 manual/Makefile
 manual/macros.tex
 Makefile])

AC_OUTPUT
