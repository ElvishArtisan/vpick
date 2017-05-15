// screensource.cpp
//
// Drag and drop source for Synergy screens
//
//   (C) Copyright 2017 Fred Gleason <fredg@paravelsystems.com>
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

#include <QApplication>
#include <QMouseEvent>

#include "screendata.h"
#include "screensource.h"

#include "../icons/vpick-16x16.xpm"
#include "../icons/vpick-48x48.xpm"

ScreenSource::ScreenSource(QWidget *parent)
  : QLabel(parent)
{
  setPixmap(QPixmap(vpick_48x48_xpm));
}


void ScreenSource::mousePressEvent(QMouseEvent *e)
{
  if(e->button()==Qt::LeftButton) {
    screen_drag_start_pos=e->pos();
  }
}


void ScreenSource::mouseMoveEvent(QMouseEvent *e)
{
  if(!(e->buttons()&Qt::LeftButton)) {
    return;
  }
  if((e->pos()-screen_drag_start_pos).manhattanLength()<
     QApplication::startDragDistance()) {
    return;
  }
  QDrag *drag=new QDrag(this);
  drag->setPixmap(QPixmap(vpick_16x16_xpm));
  ScreenData *data=new ScreenData();
  data->setData("synergy/screen","");
  drag->setMimeData(data);

  drag->exec(Qt::CopyAction);
}
