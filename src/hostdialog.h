// hostdialog.h
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

#ifndef HOSTDIALOG_H
#define HOSTDIALOG_H

#include <QCheckBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "colorbox.h"
#include "config.h"

class HostDialog : public QDialog
{
 Q_OBJECT;
 public:
  HostDialog(Config *config,QWidget *parent=0);
  QSize sizeHint() const;

 public slots:
  int exec(int id);

 private slots:
  void okData();
  void cancelData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *host_label_label;
  QLineEdit *host_label_edit;
  QLabel *host_hostname_label;
  QLineEdit *host_hostname_edit;
  QLabel *host_password_label;
  QLineEdit *host_password_edit;
  QLabel *host_color_label;
  ColorBox *host_color_box;
  QCheckBox *host_autoconnect_check;
  QLabel *host_autoconnect_label;
  QPushButton *host_ok_button;
  QPushButton *host_cancel_button;
  Config *host_config;
  int host_id;
};


#endif  // HOSTDIALOG_H
