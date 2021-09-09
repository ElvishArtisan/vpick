// hostbutton.h
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

#ifndef HOSTBUTTON_H
#define HOSTBUTTON_H

#include <QPushButton>

class HostButton : public QPushButton
{
 Q_OBJECT;
 public:
 HostButton(const QString &text,const QColor &color,QWidget *parent=0);
 void setText(const QString &text,const QColor &color);

 private:
  QColor TextColor(const QColor &color);
};


#endif  // HOSTBUTTON_H
