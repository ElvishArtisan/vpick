dnl configure.ac
dnl
dnl Autoconf configuration
dnl
dnl Use autoconf to process this into a configure script
dnl
dnl   (C) Copyright 2016-2021 Fred Gleason <fredg@paravelsystems.com>
dnl
dnl   This program is free software; you can redistribute it and/or modify
dnl   it under the terms of the GNU General Public License version 2 as
dnl   published by the Free Software Foundation.
dnl
dnl   This program is distributed in the hope that it will be useful,
dnl   but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl   GNU General Public License for more details.
dnl
dnl   You should have received a copy of the GNU General Public
dnl   License along with this program; if not, write to the Free Software
dnl   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
dnl

AC_INIT(vpick,m4_esyscmd(cat PACKAGE_VERSION))
AM_INIT_AUTOMAKE
AC_SUBST(RPM_RELEASE,1)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

#
# Basic Compiler Checks
#
AC_PROG_CXX
AC_LANG(C++)

#
# Target Platform
#
AC_ARG_WITH(desktop,[  --with-desktop          Build for desktop use],[TARGET_DESKTOP=yes],[])
AC_ARG_WITH(embedded,[  --with-embedded         Build embedded use],[TARGET_EMBEDDED=yes],[])

#
# Determine Target Platform
#
if test -z $TARGET_DESKTOP ; then
  if test -z $TARGET_EMBEDDED ; then
    AC_MSG_NOTICE([No target platform specified, assuming desktop (--with-desktop)])
    TARGET_DESKTOP=yes
  fi
fi
if test $TARGET_DESKTOP ; then
  if test $TARGET_EMBEDDED ; then
    AC_MSG_ERROR(*** Only one target platform may be specified! ***)
  fi
fi
if test $TARGET_DESKTOP ; then
  AC_MSG_NOTICE([Configuring for desktop platform])
  AC_DEFINE(DESKTOP,"yes")
  ln -sf vpick-desktop.spec vpick.spec
fi
if test $TARGET_EMBEDDED ; then
  AC_MSG_NOTICE([Configuring for embedded platform])
  AC_DEFINE(EMBEDDED,"yes")
  ln -sf vpick-embedded.spec vpick.spec
fi

#
# Check for Qt (Mandatory)
#
PKG_CHECK_MODULES(QT5,Qt5Gui Qt5Widgets Qt5Core Qt5Network,[],[AC_MSG_ERROR([*** Qt5 not found ***])])
AC_CHECK_PROG(MOC_NAME,moc-qt5,[moc-qt5],[moc])
AC_SUBST(QT_MOC,$MOC_NAME)

#
# Check for DocBook Toolchain
#
if test -z $DOCBOOK_DISABLED ; then
  USING_DOCBOOK=yes
fi
AM_CONDITIONAL([DOCBOOK_AM], [test "$USING_DOCBOOK" = yes])

#
# Determine the target architecture
#
AR_GCC_TARGET()
AC_SUBST(VENDOR,$ar_gcc_distro)
AC_SUBST(ARCH,$ar_gcc_arch)

#
# Determine number of available CPU cores for building packages
#
AX_COUNT_CPUS([],[])
if test $CPU_COUNT -lt 2 ; then
   AC_SUBST(CPUS_AVAIL,1)
else
   if test $CPU_COUNT -gt 16 ; then
      AC_SUBST(CPUS_AVAIL,16)
   else
      AC_SUBST(CPUS_AVAIL,$(($CPU_COUNT-1)))
   fi
fi

#
# Determine Distro
#
AR_GET_DISTRO()
AC_SUBST(DISTRO,$ar_gcc_distro)

#
# Configure RPM Build
#
AC_CHECK_PROG(RPMBUILD_FOUND,rpmbuild,[yes],[])
if test -z $RPMBUILD_FOUND ; then
  AC_SUBST(RPMBUILD,rpm)
else
  AC_SUBST(RPMBUILD,rpmbuild)
fi
if test -d /usr/src/redhat ; then
  AC_SUBST(RPM_ROOT,/usr/src/redhat)
  AC_SUBST(VENDOR,redhat)
else
  AC_SUBST(RPM_ROOT,/usr/src/packages)
  AC_SUBST(VENDOR,suse)
fi 

AC_CONFIG_FILES([ \
    conf/Makefile \
    icons/Makefile \
    src/Makefile \
    systemd/Makefile \
    xdg/Makefile \
    vpick.service\
    vpick-embedded.spec \
    vpick-desktop.spec \
    Makefile ])
AC_OUTPUT()

#
# Configuration Results
#
AC_MSG_NOTICE()
AC_MSG_NOTICE("|-----------------------------------------------------|")
AC_MSG_NOTICE("|         *** VPICK CONFIGURATION SUMMARY ***         |")
AC_MSG_NOTICE("|-----------------------------------------------------|")
if test $TARGET_DESKTOP ; then
AC_MSG_NOTICE("|                            Build Target: Desktop    |")
fi
if test $TARGET_EMBEDDED ; then
AC_MSG_NOTICE("|                            Build Target: Embedded   |")
fi
AC_MSG_NOTICE("|-----------------------------------------------------|")
AC_MSG_NOTICE()
AC_MSG_NOTICE(Now enter 'make' to build the software.)
AC_MSG_NOTICE()
