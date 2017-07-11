// combobox.cpp
//
// ComboBox widget for picking colors
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

#include <QPainter>
#include <QPixmap>

#include "colorbox.h"

ColorBox::ColorBox(QWidget *parent)
  : QComboBox(parent)
{
  InsertColor(tr("None"),Qt::transparent);
  InsertColor(tr("White"),Qt::white);
  InsertColor(tr("Black"),Qt::black);
  InsertColor(tr("Cyan"),Qt::cyan);
  InsertColor(tr("Dark Cyan"),Qt::darkCyan);
  InsertColor(tr("Red"),Qt::red);
  InsertColor(tr("Dark Red"),Qt::darkRed);
  InsertColor(tr("Magenta"),Qt::magenta);
  InsertColor(tr("Dark Magenta"),Qt::darkMagenta);
  InsertColor(tr("Green"),Qt::green);
  InsertColor(tr("Dark Green"),Qt::darkGreen);
  InsertColor(tr("Yellow"),Qt::yellow);
  InsertColor(tr("Dark Yellow"),Qt::darkYellow);
  InsertColor(tr("Blue"),Qt::blue);
  InsertColor(tr("Dark Blue"),Qt::darkBlue);
  InsertColor(tr("Gray"),Qt::gray);
  InsertColor(tr("Dark Gray"),Qt::darkGray);
  InsertColor(tr("Light Gray"),Qt::lightGray);
}


Qt::GlobalColor ColorBox::currentColor() const
{
  return (Qt::GlobalColor)itemData(currentIndex()).toInt();
}


bool ColorBox::setCurrentColor(Qt::GlobalColor color)
{
  for(int i=0;i<count();i++) {
    if((Qt::GlobalColor)itemData(i).toInt()==color) {
      setCurrentIndex(i);
      return true;
    }
  }
  return false;
}


void ColorBox::InsertColor(const QString &name,Qt::GlobalColor color)
{
  if(color!=Qt::transparent) {
    QPixmap pix(20,font().pointSize());
    QPainter *p=new QPainter(&pix);
    p->setPen(Qt::black);
    p->setBrush(Qt::black);
    p->drawRect(0,0,pix.size().width()-1,pix.size().height()-1);
    p->setPen(color);
    p->setBrush(color);
    p->drawRect(1,1,pix.size().width()-3,pix.size().height()-3);
    delete p;
    insertItem(count(),pix,name,(int)color);
  }
  else {
    insertItem(count(),name,color);
  }
}
