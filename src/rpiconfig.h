// rpiconfig.h
//
// Raspberry Pi Low-Level Configuration
//
//   (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef RPICONFIG_H
#define RPICONFIG_H

#include <vector>

#include <QSize>
#include <QString>
#include <QStringList>

#define VPICK_RPICONF_FILE "/boot/config.txt"

class RpiConfig
{
 public:
  RpiConfig();
  QSize framebufferSize() const;
  void setFramebufferSize(const QSize &size);
  bool wasChanged() const;
  void load();
  void save();

 private:
  QSize rpi_framebuffer_size;
  QStringList rpi_lines;
  bool rpi_was_changed;
};


#endif  // RPICONFIG_H
