// cmdswitch.h
//
// Process Command-Line Switches
//
//   (C) Copyright 2012-2021 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef CMDSWITCH_H
#define CMDSWITCH_H

#include <QList>
#include <QString>

class CmdSwitch
{
 public:
  CmdSwitch(const char *modname,const char *usage);
  int keys() const;
  QString key(unsigned n) const;
  QString value(unsigned n) const;
  bool processed(unsigned n) const;
  void setProcessed(unsigned n,bool state);
  bool allProcessed() const;

 private:
  QList<QString> switch_keys;
  QList<QString> switch_values;
  QList<bool> switch_processed;
};


#endif  // CMDSWITCH_H
