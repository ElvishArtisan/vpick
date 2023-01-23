## automake.am
##
## Makefile for the vpick(1) host chooser
##
## (C) Copyright 2016-2023 Fred Gleason <fredg@paravelsystems.com>
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
## Use automake to process this into a Makefile.in

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -Wno-strict-aliasing -std=c++11 -fPIC @QT5_CFLAGS@
MOC = @QT_MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@


bin_PROGRAMS = vpick

dist_vpick_SOURCES = cmdswitch.cpp cmdswitch.h\
                     colorbox.cpp colorbox.h\
                     combobox.cpp combobox.h\
                     config.cpp config.h\
                     hostbutton.cpp hostbutton.h\
                     hostdialog.cpp hostdialog.h\
                     layoutdialog.cpp layoutdialog.h\
                     network.cpp network.h\
                     profile.cpp profile.h\
                     rpiconfig.cpp rpiconfig.h\
                     screendata.cpp screendata.h\
                     screensource.cpp screensource.h\
                     settingsdialog.cpp settingsdialog.h\
                     synergydialog.cpp synergydialog.h\
                     synergygrid.cpp synergygrid.h\
                     synergynode.cpp synergynode.h\
                     synergynodedialog.cpp synergynodedialog.h\
                     trashcan.cpp trashcan.h\
                     viewerprocess.cpp viewerprocess.h\
                     vpick.cpp vpick.h

nodist_vpick_SOURCES = moc_colorbox.cpp\
                       moc_combobox.cpp\
                       moc_hostbutton.cpp\
                       moc_hostdialog.cpp\
                       moc_layoutdialog.cpp\
                       moc_screensource.cpp\
                       moc_settingsdialog.cpp\
                       moc_synergydialog.cpp\
                       moc_synergygrid.cpp\
                       moc_synergynode.cpp\
                       moc_synergynodedialog.cpp\
                       moc_trashcan.cpp\
                       moc_viewerprocess.cpp\
                       moc_vpick.cpp

vpick_LDADD = @QT5_LIBS@

CLEANFILES = *~\
             moc_*\
             *.obj\
             *.idb\
             *.pdb\
             *ilk

MAINTAINERCLEANFILES = *~\
                       Makefile.in
