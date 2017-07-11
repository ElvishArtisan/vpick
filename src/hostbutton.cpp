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

HostButton::HostButton(const QString &text,Qt::GlobalColor color,QWidget *parent)
  : QPushButton(text,parent)
{
  if(color!=Qt::transparent) {
    setStyleSheet("color: "+QColor(TextColor(color)).name()+
		  ";background-color: "+QColor(color).name());
  }
}


void HostButton::setText(const QString &text,Qt::GlobalColor color)
{
  QPushButton::setText(text);
  if(color!=Qt::transparent) {
    setStyleSheet("color: "+QColor(TextColor(color)).name()+
		  ";background-color: "+QColor(color).name());
  }
  else {
    setStyleSheet("");
  }
}


Qt::GlobalColor HostButton::TextColor(Qt::GlobalColor color)
{
  Qt::GlobalColor ret=Qt::black;
  switch(color) {
  case Qt::white:
    ret=Qt::black;
    break;

  case Qt::black:
    ret=Qt::white;
    break;

  case Qt::cyan:
    ret=Qt::black;
    break;

  case Qt::darkCyan:
    ret=Qt::white;
    break;

  case Qt::red:
    ret=Qt::white;
    break;

  case Qt::darkRed:
    ret=Qt::white;
    break;

  case Qt::magenta:
    ret=Qt::white;
    break;

  case Qt::darkMagenta:
    ret=Qt::white;
    break;

  case Qt::green:
    ret=Qt::black;
    break;

  case Qt::darkGreen:
    ret=Qt::black;
    break;

  case Qt::yellow:
    ret=Qt::black;
    break;

  case Qt::darkYellow:
    ret=Qt::black;
    break;

  case Qt::blue:
    ret=Qt::white;
    break;

  case Qt::darkBlue:
    ret=Qt::white;
    break;

  case Qt::gray:
    ret=Qt::black;
    break;

  case Qt::darkGray:
    ret=Qt::black;
    break;

  case Qt::lightGray:
    ret=Qt::black;
    break;

  case Qt::transparent:
  case Qt::color0:
  case Qt::color1:
    break;

  }
  return ret;
}
