## automake.am
##
## Top level automake.am
##
## Use automake to process this into a Makefile.in
##
##   (C) Copyright 2014,2016 Fred Gleason <fredg@paravelsystems.com>
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
          icons\
          src\
          xdg

install-exec-local:	
	mkdir -p $(DESTDIR)/usr/lib/systemd/system
	cp vpick.service $(DESTDIR)/usr/lib/systemd/system/vpick.service

uninstall-local:	
	rm -f $(DESTDIR)/usr/lib/systemd/system/vpick.service

rpm:	dist
	cp vpick-$(VERSION).tar.gz $(HOME)/rpmbuild/SOURCES/
	$(RPMBUILD) -ba --target $(ARCH)-$(VENDOR)-linux vpick.spec
	mv $(HOME)/rpmbuild/RPMS/$(ARCH)/vpick-*.rpm $(top_srcdir)/
	mv $(HOME)/rpmbuild/SRPMS/vpick-*.src.rpm $(top_srcdir)/
	rm $(HOME)/rpmbuild/SOURCES/vpick-$(VERSION).tar.gz
	rm -rf $(HOME)/rpmbuild/BUILD/vpick-$(VERSION)

EXTRA_DIST = vpick.spec.in\
             autogen.sh\
             get_distro.sh\
             get_target.sh\
             PACKAGE_VERSION\
             vpick.service.in

CLEANFILES = *~\
             *.deb\
             *.mo\
             *.rpm\
             *.tar.gz\
             *.tgz\
             *.zip

DISTCLEANFILES = vpick.spec\
                 config.guess\
                 config.sub\
                 ltmain.sh\
                 vpick.service

MAINTAINERCLEANFILES = *~\
                       *.tar.gz\
                       aclocal.m4\
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
