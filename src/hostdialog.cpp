// hostdialog.cpp
//
// Configure a Host
//
//   (C) Copyright 2016-2021 Fred Gleason <fredg@paravelsystems.com>
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

  setWindowTitle(tr("Host Settings"));

  host_label_label=new QLabel(tr("Label")+":",this);
  host_label_label->setFont(label_font);
  host_label_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  host_label_edit=new QLineEdit(this);

  host_type_label=new QLabel(tr("Host Type")+":",this);
  host_type_label->setFont(label_font);
  host_type_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  host_type_box=new QComboBox(this);
  for(int i=0;i<Config::LastType;i++) {
    host_type_box->insertItem(host_type_box->count(),
			      Config::typeString((Config::Type)i));
  }

  host_hostname_label=new QLabel(tr("Host Name")+":",this);
  host_hostname_label->setFont(label_font);
  host_hostname_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  host_hostname_edit=new QLineEdit(this);

  host_password_label=new QLabel(tr("Password")+":",this);
  host_password_label->setFont(label_font);
  host_password_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  host_password_edit=new QLineEdit(this);
  host_password_edit->setEchoMode(QLineEdit::Password);
  host_showpassword_check=new QCheckBox(this);
  connect(host_showpassword_check,SIGNAL(toggled(bool)),
	  this,SLOT(showPasswordToggledData(bool)));
  host_showpassword_label=new QLabel(tr("Show Password"),this);
  host_showpassword_label->setFont(check_font);
  host_showpassword_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);


  host_color_label=new QLabel(tr("Color")+":",this);
  host_color_label->setFont(label_font);
  host_color_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  host_color_box=new ColorBox(this);

  host_autoconnect_check=new QCheckBox(this);
  host_autoconnect_label=new QLabel(tr("Autoconnect"),this);
  host_autoconnect_label->setFont(label_font);
  host_autoconnect_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  host_fullscreen_check=new QCheckBox(this);
  host_fullscreen_label=new QLabel(tr("Fullscreen"),this);
  host_fullscreen_label->setFont(label_font);
  host_fullscreen_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
#ifdef EMBEDDED
  host_fullscreen_check->setDisabled(true);
  host_fullscreen_label->setDisabled(true);
#endif  // EMBEDDED
  host_ok_button=new QPushButton(tr("OK"),this);
  host_ok_button->setFont(button_font);
  connect(host_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  host_cancel_button=new QPushButton(tr("Cancel"),this);
  host_cancel_button->setFont(button_font);
  connect(host_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize HostDialog::sizeHint() const
{
  return QSize(400,233+26);
}


int HostDialog::exec(int id)
{
  host_id=id;

  host_label_edit->setText(host_config->title(id));
  host_type_box->setCurrentIndex((int)host_config->type(id));
  host_hostname_edit->setText(host_config->hostname(id));
  host_password_edit->setText(host_config->password(id));
  host_password_edit->setEchoMode(QLineEdit::Password);
  host_color_box->setCurrentColor(host_config->color(id));
  host_autoconnect_check->setChecked(host_config->autoconnect(id));
  host_fullscreen_check->setChecked(host_config->fullscreen(id));

  return QDialog::exec();
}


void HostDialog::showPasswordToggledData(bool state)
{
  if(state) {
    host_password_edit->setEchoMode(QLineEdit::Normal);
  }
  else {
    host_password_edit->setEchoMode(QLineEdit::Password);
  }
}


void HostDialog::okData()
{
  host_config->setTitle(host_id,host_label_edit->text());
  host_config->setType(host_id,(Config::Type)host_type_box->currentIndex());
  host_config->setHostname(host_id,host_hostname_edit->text());
  host_config->setPassword(host_id,host_password_edit->text());
  host_config->setColor(host_id,host_color_box->currentColor());
  host_config->setAutoconnect(host_id,host_autoconnect_check->isChecked());
  host_config->setFullscreen(host_id,host_fullscreen_check->isChecked());
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

  host_type_label->setGeometry(10,32,110,20);
  host_type_box->setGeometry(125,32,size().width()-140,20);

  host_hostname_label->setGeometry(10,54,110,20);
  host_hostname_edit->setGeometry(125,54,size().width()-140,20);

  host_password_label->setGeometry(10,76,110,20);
  host_password_edit->setGeometry(125,76,size().width()-140,20);
  host_showpassword_check->setGeometry(130,96,20,20);
  host_showpassword_label->setGeometry(155,97,size().width()-165,20);

  host_color_label->setGeometry(10,124,110,20);
  host_color_box->setGeometry(125,124,size().width()-140,20);

  host_autoconnect_check->setGeometry(130-30,146,20,20);
  host_autoconnect_label->setGeometry(155-30,147,size().width()-165,20);

  host_fullscreen_check->setGeometry(130-30,168,20,20);
  host_fullscreen_label->setGeometry(155-30,169,size().width()-165,20);

  host_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  host_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}
