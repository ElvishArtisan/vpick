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
AC_ARG_WITH(desktop,[  --with-desktop          Build for desktop use (default)],[TARGET_DESKTOP=yes],[])
AC_ARG_WITH(embedded,[  --with-embedded         Build embedded use],[TARGET_EMBEDDED=yes],[])

#
# VNC Viewer
#
AC_ARG_WITH(virtviewer,[  --with-virtviewer       Use Virt-Viewer viewer for VNC (default)],[VIEWER_VIRTVIEWER=yes],[])
AC_ARG_WITH(tigervnc,[  --with-tigervnc         Use TigerVNC viewer for VNC],[VIEWER_TIGERVNC=yes],[])
AC_ARG_WITH(ssvnc,[  --with-ssvnc            Use SSVNC viewer for VNC],[VIEWER_SSVNC=yes],[])
AC_ARG_WITH(realvnc,[  --with-realvnc          Use RealVNC viewer for VNC],[VIEWER_REALVNC=yes],[])


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
  AC_SUBST(WITH_PLATFORM_SWITCH,--with-desktop)
  AC_SUBST(VIRT_VIEWER_SETTINGS_ASKQUIT,"")
  ln -sf vpick-desktop.spec vpick.spec
fi
if test $TARGET_EMBEDDED ; then
  AC_MSG_NOTICE([Configuring for embedded platform])
  AC_DEFINE(EMBEDDED,"yes")
  AC_SUBST(WITH_PLATFORM_SWITCH,--with-embedded)
  AC_SUBST(VIRT_VIEWER_SETTINGS_ASKQUIT,ask-quit=false)
  ln -sf vpick-embedded.spec vpick.spec
fi

#
# Determine VNC Viewer
#
let vncviewers=0
if test $VIEWER_VIRTVIEWER ; then
  let vncviewers=vncviewers+1
fi
if test $VIEWER_SSVNC ; then
  let vncviewers=vncviewers+1
fi
if test $VIEWER_TIGERVNC ; then
  let vncviewers=vncviewers+1
fi
if test $VIEWER_REALVNC ; then
  let vncviewers=vncviewers+1
fi
if test $vncviewers -eq 0 ; then
  AC_MSG_NOTICE([No VNC viewer specified, assuming virt-viewer (--with-virt-viewer)])
  VIEWER_VIRTVIEWER=yes
fi
if test $vncviewers -gt 1 ; then
  AC_MSG_ERROR([*** Only one VNC viewer may be specified ***])
fi
if test $VIEWER_VIRTVIEWER ; then
  AC_MSG_NOTICE([Configuring for virt-viewer])
  AC_DEFINE(VIRTVIEWER,1)
  AC_SUBST(RPM_VNC_VIEWER_DEP)
  AC_SUBST(WITH_VNC_VIEWER_SWITCH,--with-virtviewer)
fi
if test $VIEWER_SSVNC ; then
  AC_MSG_NOTICE([Configuring for SSVNC])
  AC_DEFINE(SSVNC,1)
  AC_SUBST(RPM_VNC_VIEWER_DEP,ssvnc)
  AC_SUBST(WITH_VNC_VIEWER_SWITCH,--with-ssvnc)
fi
if test $VIEWER_REALVNC ; then
  AC_MSG_NOTICE([Configuring for RealVNC])
  AC_DEFINE(REALVNC,1)
  AC_SUBST(RPM_VNC_VIEWER_DEP,realvnc-vnc-viewer)
  AC_SUBST(WITH_VNC_VIEWER_SWITCH,--with-realvnc)
fi
if test $VIEWER_TIGERVNC ; then
  AC_MSG_NOTICE([Configuring for TigerVNC])
  AC_DEFINE(TIGERVNC,1)
  AC_SUBST(RPM_VNC_VIEWER_DEP,tigervnc)
  AC_SUBST(WITH_VNC_VIEWER_SWITCH,--with-tigervnc)
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
AC_MSG_NOTICE($ar_distro_id)
DISTRO_NAME="|                   Distribution Name ... Unknown     |"
case "$ar_distro_id" in
     debian)
       AC_MSG_NOTICE([Distro is Debian-ish])
       DISTRO_FAMILY="|                 Distribution Family ... Debian       |"
       DISTRO_IS_DEBIANISH=yes
       DISTRO_NAME="|                   Distribution Name ... Debian       |"
       AC_DEFINE(DEBIAN)
       ;;

     raspbian)
       AC_MSG_NOTICE([Distro is Debian-ish])
       DISTRO_FAMILY="|                Distribution Family ... Debian       |"
       DISTRO_IS_DEBIANISH=yes
       DISTRO_NAME="|                  Distribution Name ... Raspbian     |"
       AC_DEFINE(DEBIAN)
     ;;

     ubuntu)
       AC_MSG_NOTICE([Distro is Debian-ish])
       DISTRO_FAMILY="|                 Distribution Family ... Debian       |"
       DISTRO_IS_DEBIANISH=yes
       DISTRO_NAME="|                   Distribution Name ... Ubuntu      |"
       AC_DEFINE(DEBIAN)
     ;;

     centos|rhel|fedora|rocky|redsleeve)
       AC_MSG_NOTICE([Distro is RedHat-ish])
       DISTRO_FAMILY="|                 Distribution Family ... RedHat      |"
       if test $ar_distro_id = "centos" ; then
         DISTRO_NAME="|                   Distribution Name ... CentOS      |"
       fi
       if test $ar_distro_id = "fedora" ; then
         DISTRO_NAME="|                   Distribution Name ... Fedora      |"
       fi
       if test $ar_distro_id = "rhel" ; then
         DISTRO_NAME="|                   Distribution Name ... RHEL        |"
       fi
       if test $ar_distro_id = "rocky" ; then
         DISTRO_NAME="|                   Distribution Name ... Rocky       |"
       fi
       if test $ar_distro_id = "redsleeve" ; then
         DISTRO_NAME="|                   Distribution Name ... Redsleeve   |"
       fi
       AC_DEFINE(REDHAT)
     ;;

     *)
       AC_MSG_NOTICE([Distro is unknown])
       DISTRO_FAMILY="|                 Distribution Family ... Unknown     |"
     ;;
esac

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

#
# Generated File Warnings
#
AC_SUBST_FILE(GENERATED_C_FILE_WARNING)
GENERATED_C_FILE_WARNING=$srcdir/helpers/c_file_warning

AC_SUBST_FILE(GENERATED_SCRIPT_FILE_WARNING)
GENERATED_SCRIPT_FILE_WARNING=$srcdir/helpers/script_file_warning

AC_SUBST_FILE(GENERATED_XML_FILE_WARNING)
GENERATED_XML_FILE_WARNING=$srcdir/helpers/xml_file_warning

AC_CONFIG_FILES([ \
    conf/virt-viewer-settings \
    conf/Makefile \
    debian/Makefile \
    debian/source/Makefile \
    helpers/Makefile \
    icons/Makefile \
    src/Makefile \
    systemd/Makefile \
    xdg/Makefile \
    build_debs.sh \
    vpick-embedded.spec \
    vpick-desktop.spec \
    Makefile ])
AC_OUTPUT()
chmod 755 build_debs.sh

#
# Configuration Results
#
AC_MSG_NOTICE()
AC_MSG_NOTICE("|-----------------------------------------------------|")
AC_MSG_NOTICE("|         *** VPICK CONFIGURATION SUMMARY ***         |")
AC_MSG_NOTICE("|-----------------------------------------------------|")
AC_MSG_NOTICE("| Platform Information:                               |")
AC_MSG_NOTICE("$DISTRO_NAME")
AC_MSG_NOTICE("$DISTRO_FAMILY")
AC_MSG_NOTICE("|                                                     |")
if test $TARGET_DESKTOP ; then
AC_MSG_NOTICE("| Build Options:                                      |")
AC_MSG_NOTICE("|                        Build Target ... Desktop     |")
fi
if test $TARGET_EMBEDDED ; then
AC_MSG_NOTICE("|                        Build Target ... Embedded    |")
fi
if test $VIEWER_VIRTVIEWER ; then
AC_MSG_NOTICE("|                          VNC Viewer ... virt-viewer |")
fi
if test $VIEWER_SSVNC ; then
AC_MSG_NOTICE("|                          VNC Viewer ... ssvnc       |")
fi
if test $VIEWER_TIGERVNC ; then
AC_MSG_NOTICE("|                          VNC Viewer ... TigerVnc    |")
fi
if test $VIEWER_REALVNC ; then
AC_MSG_NOTICE("|                          VNC Viewer ... RealVnc     |")
fi
AC_MSG_NOTICE("|-----------------------------------------------------|")
AC_MSG_NOTICE()
AC_MSG_NOTICE(Now enter 'make' to build the software.)
AC_MSG_NOTICE()
