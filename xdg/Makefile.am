## automake.am
##
## xdg/automake.am
##
##   (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
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
## Use automake to process this into a Makefile.in

install-exec-am:	
	mkdir -p $(DESTDIR)@prefix@/share/applications
	cp vpick.desktop $(DESTDIR)@prefix@/share/applications/

uninstall-local:	
	rm -f $(DESTDIR)@prefix@/share/applications/vpick.desktop

EXTRA_DIST = vpick.desktop.in

CLEANFILES = *~

DISTCLEANFILES = vpic.desktop

MAINTAINERCLEANFILES = *~\
                       Makefile.in
