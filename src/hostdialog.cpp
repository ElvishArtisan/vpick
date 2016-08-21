// hostdialog.cpp
//
// Configure a Host
//
//   (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
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

#include <QMessageBox>

#include "hostdialog.h"

HostDialog::HostDialog(Config *config,QWidget *parent)
  : QDialog(parent)
{
  host_config=config;
  host_id=-1;

  QFont check_font("helvetica",12,QFont::Bold);
  check_font.setPixelSize(12);
  QFont label_font("helvetica",14,QFont::Bold);
  label_font.setPixelSize(14);
  QFont button_font("helvetica",16,QFont::Bold);
  button_font.setPixelSize(16);

  host_label_label=new QLabel(tr("Label")+":",this);
  host_label_label->setFont(label_font);
  host_label_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  host_label_edit=new QLineEdit(this);

  host_hostname_label=new QLabel(tr("Hostname")+":",this);
  host_hostname_label->setFont(label_font);
  host_hostname_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  host_hostname_edit=new QLineEdit(this);

  host_password_label=new QLabel(tr("Password")+":",this);
  host_password_label->setFont(label_font);
  host_password_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  host_password_edit=new QLineEdit(this);
  host_password_edit->setEchoMode(QLineEdit::Password);

  host_autoconnect_check=new QCheckBox(this);
  host_autoconnect_label=new QLabel(tr("Autoconnect"),this);
  host_autoconnect_label->setFont(check_font);
  host_autoconnect_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  host_ok_button=new QPushButton(tr("OK"),this);
  host_ok_button->setFont(button_font);
  connect(host_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  host_cancel_button=new QPushButton(tr("Cancel"),this);
  host_cancel_button->setFont(button_font);
  connect(host_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize HostDialog::sizeHint() const
{
  return QSize(300,170);
}


int HostDialog::exec(int id)
{
  host_id=id;

  host_label_edit->setText(host_config->title(id));
  host_hostname_edit->setText(host_config->hostname(id));
  host_password_edit->setText(host_config->password(id));
  host_autoconnect_check->setChecked(host_config->autoconnect(id));

  return QDialog::exec();
}


void HostDialog::okData()
{
  host_config->setTitle(host_id,host_label_edit->text());
  host_config->setHostname(host_id,host_hostname_edit->text());
  host_config->setPassword(host_id,host_password_edit->text());
  host_config->setAutoconnect(host_id,host_autoconnect_check->isChecked());
  host_config->save();

  done(0);
}


void HostDialog::cancelData()
{
  done(-1);
}


void HostDialog::closeEvent(QCloseEvent *e)
{
  cancelData();
}


void HostDialog::resizeEvent(QResizeEvent *e)
{
  host_label_label->setGeometry(10,10,110,20);
  host_label_edit->setGeometry(125,10,size().width()-140,20);

  host_hostname_label->setGeometry(10,32,110,20);
  host_hostname_edit->setGeometry(125,32,size().width()-140,20);

  host_password_label->setGeometry(10,54,110,20);
  host_password_edit->setGeometry(125,54,size().width()-140,20);

  host_autoconnect_check->setGeometry(125,75,20,20);
  host_autoconnect_label->setGeometry(150,73,size().width()-165,20);

  host_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  host_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}
