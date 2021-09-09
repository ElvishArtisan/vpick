// hostbutton.cpp
//
// Button widget for host entries
//
//   (C) Copyright 2015-2017 Fred Gleason <fredg@paravelsystems.com>
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

#include <stdio.h>

#include "hostbutton.h"

HostButton::HostButton(const QString &text,const QColor &color,QWidget *parent)
  : QPushButton(text,parent)
{
  if(color.isValid()) {
    setStyleSheet("color: "+TextColor(color).name()+
		  ";background-color: "+color.name());
  }
}


void HostButton::setText(const QString &text,const QColor &color)
{
  QPushButton::setText(text);
  if(color.isValid()) {
    setStyleSheet("color: "+TextColor(color).name()+
		  ";background-color: "+color.name());
  }
  else {
    setStyleSheet("");
  }
}


QColor HostButton::TextColor(const QColor &color)
{
  int h,s,v;
  QColor ret=color;

  color.getHsv(&h,&s,&v);
  if(v<128) {
    ret=Qt::white;
  }
  else {
    if((h>210)&&(h<270)&&(s>128)) {  // Special case for blue
      ret=Qt::white;
    }
    else {
      ret=Qt::black;
    }
  }

  return ret;
}
