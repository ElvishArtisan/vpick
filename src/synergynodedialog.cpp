// synergynodedialog.cpp
//
// Configure a Synergy node.
//
//   (C) Copyright 2017-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include "synergynodedialog.h"

SynergyNodeDialog::SynergyNodeDialog(QWidget *parent)
  : QDialog(parent)
{
  setWindowTitle(tr("Host Picker - Synergy Node Settings"));

  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  synergy_screenname_label=new QLabel(tr("Screen Name")+":",this);
  synergy_screenname_label->setFont(bold_font);
  synergy_screenname_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  synergy_screenname_edit=new QLineEdit(this);

  synergy_ok_button=new QPushButton(tr("OK"),this);
  synergy_ok_button->setFont(bold_font);
  connect(synergy_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  synergy_cancel_button=new QPushButton(tr("Cancel"),this);
  synergy_cancel_button->setFont(bold_font);
  connect(synergy_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize SynergyNodeDialog::sizeHint() const
{
  return QSize(300,100);
}


int SynergyNodeDialog::exec(QString *screenname)
{
  synergy_screenname=screenname;
  synergy_screenname_edit->setText(*screenname);
  synergy_screenname_edit->selectAll();
  return QDialog::exec();
}


void SynergyNodeDialog::okData()
{
  *synergy_screenname=synergy_screenname_edit->text();
  done(true);
}


void SynergyNodeDialog::cancelData()
{
  done(false);
}


void SynergyNodeDialog::closeEvent(QCloseEvent *e)
{
  cancelData();
}


void SynergyNodeDialog::resizeEvent(QResizeEvent *e)
{
  synergy_screenname_label->setGeometry(10,10,125,20);
  synergy_screenname_edit->setGeometry(140,10,size().width()-150,20);

  synergy_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);

  synergy_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}
