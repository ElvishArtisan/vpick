// layoutdialog.cpp
//
// Manage button layout
//
//   (C) Copyright 2021-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <QDropEvent>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>

#include "layoutdialog.h"

LayoutDialog::LayoutDialog(Config *config,QWidget *parent)
  : QDialog(parent)
{
  d_changed=false;
  d_config=config;
  setAcceptDrops(true);

  setWindowTitle(tr("Host Picker - Layout"));

  QFont label_font(font().family(),font().pointSize(),QFont::Bold);

  d_instructions_label=new QLabel(this);
  d_instructions_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  d_instructions_label->setWordWrap(true);
  d_instructions_label->setText(tr("Drag buttons to desired locations."));

  d_save_button=new QPushButton(tr("Save"),this);
  d_save_button->setFont(label_font);
  connect(d_save_button,SIGNAL(clicked()),this,SLOT(saveData()));

  d_cancel_button=new QPushButton(tr("Cancel"),this);
  d_cancel_button->setFont(label_font);
  connect(d_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize LayoutDialog::sizeHint() const
{
  return d_size_hint;
}


int LayoutDialog::exec()
{
  QFont font("helvetica",16,QFont::Bold);
  font.setPixelSize(16);

  d_size_hint=QSize(d_config->screenSize().width()*(VPICK_BUTTON_WIDTH+VPICK_BUTTON_MARGIN),
		    d_config->screenSize().height()*(VPICK_BUTTON_HEIGHT+VPICK_BUTTON_MARGIN)+60);
  setMaximumSize(d_size_hint);
  setMinimumSize(d_size_hint);
  
  d_config->save();
  d_changed=false;

  for(int i=0;i<d_config->hostQuantity();i++) {
    d_buttons.
      push_back(new HostButton(i,d_config->title(i),d_config->color(i),this));
    d_buttons.back()->setFont(font);
    d_buttons.back()->setAllowDrags(true);
  }
  UpdateLayout();

  return QDialog::exec();
}


void LayoutDialog::saveData()
{
  ClearButtons();
  d_config->save();
  d_changed=false;

  done(true);
}


bool LayoutDialog::cancelData()
{
  if(d_changed) {
    switch(QMessageBox::question(this,"VPick - "+tr("Layout Changed"),
				 tr("Save changes to the button layout?"),
				 QMessageBox::Yes|QMessageBox::No|
				 QMessageBox::Cancel)) {
    case QMessageBox::Yes:
      saveData();
      return true;

    case QMessageBox::No:
      ClearButtons();
      d_config->load();
      d_changed=false;
      done(true);
      return true;

    default:
      return false;
    }
  }
  done(false);

  return false;
}


void LayoutDialog::closeEvent(QCloseEvent *e)
{
  if(cancelData()) {
    e->accept();
  }
  else {
    e->ignore();
  }
}


void LayoutDialog::resizeEvent(QResizeEvent *e)
{
  UpdateLayout();
}


void LayoutDialog::paintEvent(QPaintEvent *e)
{
  int w=size().width();
  int h=size().height();
  QPainter *p=new QPainter(this);

  p->setPen(palette().color(QPalette::Inactive,QPalette::WindowText));

  // Vertical Lines
  for(int i=1;i<d_config->screenSize().width();i++) {
    p->drawLine(i*(VPICK_BUTTON_WIDTH+VPICK_BUTTON_MARGIN),
		0,i*(VPICK_BUTTON_WIDTH+VPICK_BUTTON_MARGIN),h-60);
  }

  // Horizontal Lines
  for(int i=1;i<(d_config->screenSize().height()+1);i++) {
    p->drawLine(0,i*(VPICK_BUTTON_HEIGHT+VPICK_BUTTON_MARGIN),
		w,i*(VPICK_BUTTON_HEIGHT+VPICK_BUTTON_MARGIN));
  }
  p->end();
  delete p;
}


void LayoutDialog::dragEnterEvent(QDragEnterEvent *e)
{
  if(e->mimeData()->hasFormat(VPICK_MIMETYPE)) {
    e->accept();
  }
}


void LayoutDialog::dragMoveEvent(QDragMoveEvent *e)
{
  QPoint pt=e->position().toPoint();
  QPoint pos=QPoint(pt.x()/(VPICK_BUTTON_MARGIN+VPICK_BUTTON_WIDTH),
		    pt.y()/(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT));

  if((pos.y()<d_config->screenSize().height()-1)&&
     d_config->positionIsFree(pos)) {
    e->accept();
  }
  else {
    e->ignore();
  }
}


void LayoutDialog::dropEvent(QDropEvent *e)
{
  bool ok=false;
  QPoint pt=e->position().toPoint();
  QPoint pos=QPoint(pt.x()/(VPICK_BUTTON_MARGIN+VPICK_BUTTON_WIDTH),
		    pt.y()/(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT));

  if(d_config->positionIsFree(pos)) {
    int id=e->mimeData()->data(VPICK_MIMETYPE).toInt(&ok);
    if(ok&&(id>=0)&&(id<d_config->hostQuantity())) {
      e->acceptProposedAction();
      d_config->setPosition(id,pos);
      UpdateLayout();
      d_changed=true;
    }
  }
}


void LayoutDialog::UpdateLayout()
{
  int w=size().width();
  int h=size().height();

  for(int i=0;i<d_config->hostQuantity();i++) {
    d_buttons.at(i)->
      setGeometry(VPICK_BUTTON_MARGIN/2+(VPICK_BUTTON_MARGIN+VPICK_BUTTON_WIDTH)*
		  d_config->position(i).x(),
		  VPICK_BUTTON_MARGIN/2+(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT)*
		  d_config->position(i).y(),
		  VPICK_BUTTON_WIDTH,
		  VPICK_BUTTON_HEIGHT);
  }
  int button_y=h-50;
  d_instructions_label->setGeometry(VPICK_BUTTON_MARGIN,
				    button_y,
				    w-180,VPICK_BUTTON_HEIGHT);
  d_save_button->setGeometry(w-160,
			     button_y,
			     70,VPICK_BUTTON_HEIGHT);
  d_cancel_button->setGeometry(w-80,
			       button_y,
			       70,VPICK_BUTTON_HEIGHT);
}


void LayoutDialog::ClearButtons()
{
  for(int i=0;i<d_config->hostQuantity();i++) {
    delete d_buttons.at(i);
  }
  d_buttons.clear();
}
