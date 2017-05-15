// synergynode.cpp
//
// Synergy server configuration node
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
#include <QDrag>
#include <QMouseEvent>

#include "screendata.h"
#include "synergynode.h"

#include "../icons/vpick-16x16.xpm"
#include "../icons/vpick-48x48.xpm"

SynergyNode::SynergyNode(QWidget *parent,bool is_root)
  : QWidget(parent)
{
  node_is_root=is_root;
  node_active=false;
  node_placed=false;

  node_dialog=new SynergyNodeDialog(this);

  node_icon_label=new QLabel(this);
  node_icon_label->setPixmap(QPixmap(vpick_48x48_xpm));
  node_icon_label->setAlignment(Qt::AlignCenter|Qt::AlignVCenter);
  node_icon_label->hide();

  node_screenname_label=new QLabel(this);
  node_screenname_label->setAlignment(Qt::AlignCenter|Qt::AlignTop);
  node_screenname_label->hide();

  setAcceptDrops(!is_root);
}


bool SynergyNode::isRoot() const
{
  return node_is_root;
}


bool SynergyNode::isActive() const
{
  return node_active;
}


void SynergyNode::setActive(bool state)
{
  if(state!=node_active) {
    if(state) {
      node_icon_label->show();
      node_screenname_label->show();
    }
    else {
      node_icon_label->hide();
      node_screenname_label->hide();
    }
    node_active=state;
  }
}


QString SynergyNode::screenName() const
{
  return node_screenname_label->text();
}


void SynergyNode::setScreenName(const QString &str)
{
  node_screenname_label->setText(str);
}


bool SynergyNode::placed() const
{
  return node_placed;
}


void SynergyNode::setPlaced(bool state)
{
  node_placed=state;
}


void SynergyNode::dragEnterEvent(QDragEnterEvent *e)
{
  if(e->mimeData()->hasFormat("synergy/screen")) {
    const ScreenData *data=(ScreenData *)(e->mimeData());
   if(data->screenName()!=screenName()) {
      e->acceptProposedAction();
    }
  }
}


void SynergyNode::dropEvent(QDropEvent *e)
{
  const ScreenData *data=(ScreenData *)(e->mimeData());
  if(data->screenName()!=screenName()) {
    node_screenname_label->setText(data->screenName());
    setActive(true);
    e->acceptProposedAction();
  }
}


void SynergyNode::mouseDoubleClickEvent(QMouseEvent *e)
{
  if((!isRoot())&&isActive()) {
    QString screenname=node_screenname_label->text();
    if(node_dialog->exec(&screenname)) {
      node_screenname_label->setText(screenname);
    }
  }
}


void SynergyNode::mousePressEvent(QMouseEvent *e)
{
  if((!isRoot())&&(node_active&&(e->button()==Qt::LeftButton))) {
    node_drag_start_pos=e->pos();
  }
}


void SynergyNode::mouseMoveEvent(QMouseEvent *e)
{
  if(isRoot()) {
    return;
  }
  if((!(e->buttons()&Qt::LeftButton))||(!node_active)) {
    return;
  }
  if((e->pos()-node_drag_start_pos).manhattanLength()<
     QApplication::startDragDistance()) {
    return;
  }
  QDrag *drag=new QDrag(this);
  drag->setPixmap(QPixmap(vpick_16x16_xpm));
  ScreenData *data=new ScreenData();
  data->setData("synergy/screen",node_icon_label->text().toUtf8());
  data->setScreenName(screenName());
  drag->setMimeData(data);

  if(drag->exec(Qt::MoveAction)==Qt::MoveAction) {
    setActive(false);
  }
}


void SynergyNode::resizeEvent(QResizeEvent *e)
{
  node_icon_label->setGeometry(1,1,size().width()-2,3*size().height()/4);
  node_screenname_label->setGeometry(1,3*size().height()/5,size().width()-2,2*size().height()/5);
}
