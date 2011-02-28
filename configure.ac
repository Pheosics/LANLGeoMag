#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.



AC_PREREQ([2.63])
AC_INIT([LanlGeoMag], [1.5.8], [mghenderson@lanl.gov])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_LANG([C])
#AC_DEFINE([LGM_EOP_DATA_DIR],["/usr/local/share/LanlGeoMag/EopData"],["Define directory where Eop data can be found"])
#AH_VERBATIM([PDDH], [#define LGM_EOP_DATA_DIR "@datadir@/LanlGeoMag/EopData"])
#AC_DEFINE([PDD], ["PDD @prefix@/data"], ["dude"])
AC_DEFINE([LGM_EOP_DATA_DIR],[(DATADIR "/LanlGeoMag/EopData")],[Define directory where Eop data can be found])
AC_CONFIG_SRCDIR([libLanlGeoMag/quicksort.c])
AM_INIT_AUTOMAKE
AC_PROG_LIBTOOL

DX_INIT_DOXYGEN($PACKAGE_NAME, libLanlGeoMag/Doxyfile)
DX_HTML_FEATURE(ON)
DX_MAN_FEATURE(ON)
DX_PDF_FEATURE(ON)

dnl Add LDFLAGS for embedded perl
PERL_LDFLAGS=`perl -MExtUtils::Embed -e ldopts`
AC_MSG_NOTICE([Adding perl LIBS ${PERL_LDFLAGS}])
LDFLAGS=${LDFLAGS}\ ${PERL_LDFLAGS}
#AC_CHECK_LIB([perl], [perl_alloc],, AC_MSG_ERROR([No libperl found]))

dnl Add CFLAGS for embedded perl
PERL_CFLAGS=`perl -MExtUtils::Embed -e ccopts`
AC_MSG_NOTICE([Adding perl CFLAGS ${PERL_CFLAGS}])
CFLAGS=${CFLAGS}\ ${PERL_CFLAGS}

dnl Add LDFLAGS for gsl
dnl GSL_LDFLAGS=-lgsl 
GSL_LDFLAGS=-lgsl
LDFLAGS=${LDFLAGS}\ ${GSL_LDFLAGS}
GSLCBLAS_LDFLAGS=-lgslcblas
LDFLAGS=${LDFLAGS}\ ${GSLCBLAS_LDFLAGS}


AM_CONDITIONAL(LINUX, [test -n "`echo ${host_os} | grep -i linux`"])
AM_CONDITIONAL(APPLE, [test -n "`echo ${host_os} | grep -i apple`"])

# Checks for programs.
AC_PROG_CC
#AC_PROG_F77

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lLanlGeoMag':
#AC_CHECK_LIB([LanlGeoMag], [Lgm_Convert_Coords])
# FIXME: Replace `main' with a function in `-lgfortran':
#AC_CHECK_LIB([gfortran], [main])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [sin])
# FIXME: Replace `main' with a function in `-lgsl':
#AC_CHECK_LIB([gsl], [gsl_spline_init], [], [AC_MSG_ERROR(could not find required version of GSL)])
# FIXME: Replace `main' with a function in `-lgslcblas':
#AC_CHECK_LIB([gslcblas], [main])

AM_CONDITIONAL([HAVE_CHECK], false)
PKG_CHECK_MODULES([CHECK], [check >= 0.9.4], AM_CONDITIONAL([HAVE_CHECK], true), AC_MSG_WARN([check not found; testsuite not available]))


# Checks for header files.
AC_CHECK_HEADERS([fcntl.h float.h limits.h stdlib.h string.h sys/timeb.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AM_PROG_CC_C_O

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor memset pow sqrt strstr])

AC_CONFIG_FILES([Makefile
                 libLanlGeoMag/Lgm/Makefile
                 libLanlGeoMag/Makefile
                 libLanlGeoMag/EopData/GetEopFiles
                 libLanlGeoMag/lgm.pc]
                 tests/Makefile)
AC_OUTPUT