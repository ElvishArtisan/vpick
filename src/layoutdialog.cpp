// layoutdialog.cpp
//
// Manage button layout
//
//   (C) Copyright 2021 Fred Gleason <fredg@paravelsystems.com>
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

#include "layoutdialog.h"

LayoutDialog::LayoutDialog(Config *config,QWidget *parent)
  : QDialog(parent)
{
  d_config=config;
  //  setAcceptDrags(true);
}


QSize LayoutDialog::sizeHint() const
{
  return QSize(d_config->screenSize().width()*
	       (VPICK_BUTTON_MARGIN+VPICK_BUTTON_WIDTH),
	       d_config->screenSize().height()*
	       (VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT));
}


int LayoutDialog::exec()
{
  QFont font("helvetica",16,QFont::Bold);
  font.setPixelSize(16);

  setMaximumSize(sizeHint());
  setMinimumSize(sizeHint());

  for(int i=0;i<d_config->hostQuantity();i++) {
    d_buttons.push_back(new HostButton(i,d_config->title(i),d_config->color(i),this));
    d_buttons.back()->setFont(font);
    d_buttons.back()->setAllowDrags(true);
    d_buttons.back()->
      setGeometry(VPICK_BUTTON_MARGIN+(VPICK_BUTTON_MARGIN+VPICK_BUTTON_WIDTH)*d_config->position(i).x(),
		  VPICK_BUTTON_MARGIN+(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT)*d_config->position(i).y(),
		  VPICK_BUTTON_WIDTH,
		  VPICK_BUTTON_HEIGHT);
  }

  return QDialog::exec();
}


void LayoutDialog::closeEvent(QCloseEvent *e)
{
  for(int i=0;i<d_config->hostQuantity();i++) {
    delete d_buttons.at(i);
  }
  d_buttons.clear();
  done(true);
}


void LayoutDialog::resizeEvent(QResizeEvent *e)
{
  //  int w=size().width();
  //  int h=size().height();

  for(int i=0;i<d_config->hostQuantity();i++) {
    d_buttons.at(i)->
      setGeometry(VPICK_BUTTON_MARGIN+(VPICK_BUTTON_MARGIN+VPICK_BUTTON_WIDTH)*d_config->position(i).x(),
		  VPICK_BUTTON_MARGIN+(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT)*d_config->position(i).y(),
		  VPICK_BUTTON_WIDTH,
		  VPICK_BUTTON_HEIGHT);
  }
}


void LayoutDialog::paintEvent(QPaintEvent *e)
{
  int w=size().width();
  int h=size().height();
  QPainter *p=new QPainter(this);

  p->setPen(palette().color(QPalette::Inactive,QPalette::WindowText));
  for(int i=1;i<d_config->screenSize().width();i++) {
    p->drawLine(i*(VPICK_BUTTON_MARGIN+VPICK_BUTTON_WIDTH)+VPICK_BUTTON_MARGIN/2,0,
		i*(VPICK_BUTTON_MARGIN+VPICK_BUTTON_WIDTH)+VPICK_BUTTON_MARGIN/2,h);
  }
  for(int i=1;i<d_config->screenSize().height();i++) {
    p->drawLine(0,i*(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT)+VPICK_BUTTON_MARGIN/2,
		w,i*(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT)+VPICK_BUTTON_MARGIN/2);
  }
  p->end();
  delete p;
}


void LayoutDialog::dragEnterEvent(QDragEnterEvent *e)
{
  QPoint logical_pos=QPoint(e->pos().x()/(VPICK_BUTTON_MARGIN+VPICK_BUTTON_WIDTH),
			    e->pos().y()/(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT));
  printf("drag pos: %dx%d\n",logical_pos.x(),logical_pos.y());
}


void LayoutDialog::dropEvent(QDropEvent *e)
{
}
