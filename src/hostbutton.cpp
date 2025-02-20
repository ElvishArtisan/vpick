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

#include <QDrag>
#include <QMessageBox>
#include <QMimeData>
#include <QMouseEvent>

#include "config.h"
#include "hostbutton.h"

#include "../icons/vpick-16x16.xpm"

//HostButton::HostButton(int id,const QString &text,const QColor &color,
//		       Config *c,QWidget *parent)
HostButton::HostButton(int id,Config *c,QWidget *parent)
  : QPushButton(c->title(id),parent)
{
  d_id=id;
  d_allow_drags=false;
  d_move_count=-1;
  d_config=c;
  d_menu_palette=parent->palette();
  
  d_rightclick_menu=new QMenu(this);
  d_rightclick_menu->setStyleSheet("");
  connect(d_rightclick_menu,SIGNAL(aboutToShow()),this,SLOT(aboutToShowData()));
  d_remember_position_action=d_rightclick_menu->
    addAction(tr("Remember position"),this,SLOT(rememberPositionData()));

  if(c->color(id).isValid()) {
    setStyleSheet("color: "+TextColor(c->color(id)).name()+
    		  ";background-color: "+c->color(id).name());
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


void HostButton::setAllowDrags(bool state)
{
  d_allow_drags=state;
}


void HostButton::aboutToShowData()
{
  QPalette::ColorGroup cg=QPalette::Active;
  if(d_config->liveWindowGeometry(d_id).isNull()) {
    cg=QPalette::Disabled;
  }
  d_rightclick_menu->setPalette(d_menu_palette);
  d_rightclick_menu->
    setStyleSheet(QString("background-color: ")+
		  d_menu_palette.color(cg,QPalette::Window).name()+";"+
		  QString("color: ")+
		  d_menu_palette.color(cg,QPalette::WindowText).name()+";");
  d_window_position=QPoint();
  d_config->updateLiveParameters(d_id);
}


void HostButton::rememberPositionData()
{
  emit savePosition(d_id,d_config->liveWindowGeometry(d_id).topLeft());
}


void HostButton::mousePressEvent(QMouseEvent *e)
{
  d_move_count=10;
  if(e->button()==Qt::RightButton) {
    d_rightclick_menu->
      popup((QPoint(e->globalPosition().x(),e->globalPosition().y())));
  }
  QPushButton::mousePressEvent(e);
}


void HostButton::mouseMoveEvent(QMouseEvent *e)
{
  if(d_allow_drags) {
    d_move_count--;
    if(d_move_count==0) {
      QPushButton::mouseReleaseEvent(e);
      QDrag *drag=new QDrag(this);
      QMimeData *mdata=new QMimeData();
      mdata->setData(VPICK_MIMETYPE,QString::asprintf("%d",d_id).toUtf8());
      drag->setMimeData(mdata);
      drag->setPixmap(QPixmap(vpick_16x16_xpm));
      drag->exec();
    }
  }
}


void HostButton::mouseReleaseEvent(QMouseEvent *e)
{
  d_move_count=-1;
  QPushButton::mouseReleaseEvent(e);
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
