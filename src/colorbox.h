// combobox.h
//
// ComboBox widget for picking colors
//
//   (C) Copyright 2015-2021 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef COLORBOX_H
#define COLORBOX_H

#include <QColor>
#include <QComboBox>
#include <QKeyEvent>
#include <QVariant>

class ColorBox : public QComboBox
{
 Q_OBJECT;
 public:
  ColorBox(QWidget *parent=0);
  QColor currentColor() const;
  bool setCurrentColor(const QColor &color);

 private:
  void InsertColor(const QString &name,const QColor &color);
  QColor d_default_color;
};


#endif  // COLORBOX_H
