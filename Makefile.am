## automake.am
##
## Top level automake.am
##
## Use automake to process this into a Makefile.in
##
##   (C) Copyright 2014-2021 Fred Gleason <fredg@paravelsystems.com>
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License version 2 as
##   published by the Free Software Foundation.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##

#if DOCBOOK_AM
#  DOCBOOK_AM_OPT = docs
#endif

#SUBDIRS = $(DOCBOOK_AM_OPT) src
SUBDIRS = conf\
          debian\
          helpers\
          icons\
          src\
          systemd\
          xdg

deb:	dist
	./build_debs.sh

rpm:	dist
	cp vpick-$(VERSION).tar.gz $(HOME)/rpmbuild/SOURCES/
	$(RPMBUILD) -ba --target $(ARCH)-$(VENDOR)-linux vpick.spec
	mv $(HOME)/rpmbuild/RPMS/$(ARCH)/vpick-*.rpm $(top_srcdir)/
	mv $(HOME)/rpmbuild/SRPMS/vpick-*.src.rpm $(top_srcdir)/
	rm $(HOME)/rpmbuild/SOURCES/vpick-$(VERSION).tar.gz
	rm -rf $(HOME)/rpmbuild/BUILD/vpick-$(VERSION)

EXTRA_DIST = vpick-embedded.spec.in\
             vpick-desktop.spec.in\
             autogen.sh\
             get_distro.pl\
             get_target.sh\
             PACKAGE_VERSION

CLEANFILES = *~\
             *.buildinfo\
             *.changes\
             *.ddeb\
             *.deb\
             *.dsc\
             *.mo\
             *.rpm\
             *.spec\
             *.tar.gz\
             *.tar.xz\
             *.tgz\
             *.zip\
             py-compile

DISTCLEANFILES = *.spec\
                 build_debs.sh\
                 config.guess\
                 config.sub\
                 ltmain.sh

MAINTAINERCLEANFILES = *~\
                       *.tar.gz\
                       aclocal.m4\
                       compile\
                       config.guess\
                       config.status\
                       config.sub\
                       configure\
                       depcomp\
                       install-sh\
                       libtool\
                       ltmain.sh\
                       Makefile.in\
                       missing\
                       mkinstalldirs
