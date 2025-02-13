// cmdswitch.cpp
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

#include <stdlib.h>
#include <stdio.h>

#include <QApplication>

#include "cmdswitch.h"

CmdSwitch::CmdSwitch(const char *modname,const char *usage)
{
  QStringList args=qApp->arguments();

  for(int i=1;i<args.size();i++) {
#ifndef WIN32
    if(args.at(i)=="--version") {
      printf("%s v%s\n",modname,VERSION);
      exit(0);
    }
#endif  // WIN32
    if(args.at(i)=="--help") {
      printf("\n%s %s\n",modname,usage);
      exit(0);
    }
    QStringList f0=args.at(i).split("=",Qt::KeepEmptyParts);
    if(f0.size()>=2) {
      if(f0.at(0).left(1)=="-") {
	switch_keys.push_back(f0.at(0));
	for(int i=2;i<f0.size();i++) {
	  f0[1]+="="+f0.at(i);
	}
	if(f0.at(1).isEmpty()) {
	  switch_values.push_back("");
	}
	else {
	  switch_values.push_back(f0.at(1));
	}
      }
      else {
	switch_keys.push_back(f0.join("="));
	switch_values.push_back("");
      }
      switch_processed.push_back(false);
    }
    else {
      switch_keys.push_back(f0.at(0));
      switch_values.push_back("");
      switch_processed.push_back(false);
    }
  }
}


int CmdSwitch::keys() const
{
  return switch_keys.size();
}


QString CmdSwitch::key(unsigned n) const
{
  return switch_keys[n];
}


QString CmdSwitch::value(unsigned n) const
{
  return switch_values[n];
}


bool CmdSwitch::processed(unsigned n) const
{
  return switch_processed[n];
}


void CmdSwitch::setProcessed(unsigned n,bool state)
{
  switch_processed[n]=state;
}


bool CmdSwitch::allProcessed() const
{
  for(int i=0;i<switch_processed.size();i++) {
    if(!switch_processed[i]) {
      return false;
    }
  }
  return true;
}
