## automake.am
##
## conf/Automake.am for vpick
##
##   (C) Copyright 2015-2021 Fred Gleason <fredg@paravelsystems.com>
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
##
## by Fred Gleason <fredg@paravelsystems.com>
##
## Use automake to process this into a Makefile.in

install-exec-am:	
	mkdir -p $(DESTDIR)/etc
	cp system.fvwm2rc $(DESTDIR)/etc/
	mkdir -p $(DESTDIR)/root/.config/virt-viewer
	cp virt-viewer-settings $(DESTDIR)/root/.config/virt-viewer/settings

uninstall-local:	
	rm -f $(DESTDIR)/etc/system.fvwm2rc
	rm -f $(DESTDIR)/root/.config/virt-viewer/settings

EXTRA_DIST = config-sample.txt\
             system.fvwm2rc\
             virt-viewer-settings.in\
             vpick-sample.conf\
             xorg-vpick.conf

CLEANFILES = *~
DISTCLEANFILES = moc_*\
                 virt-viewer-settings
MAINTAINERCLEANFILES = *~\
                       Makefile.in
