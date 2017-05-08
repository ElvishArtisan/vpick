// synergydialog.h
//
// Configure Synergy
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

#ifndef SYNERGYDIALOG_H
#define SYNERGYDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QPushButton>

#include "combobox.h"
#include "config.h"

class SynergyDialog : public QDialog
{
 Q_OBJECT;
 public:
  SynergyDialog(Config *c,QWidget *parent=0);
  QSize sizeHint() const;
  void startSynergy();
  void stopSynergy();

 public slots:
  int exec();

 private slots:
  void modeActivatedData(int index);
  void okData();
  void cancelData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  ComboBox *synergy_mode_box;
  QLabel *synergy_screenname_label;
  QLineEdit *synergy_screenname_edit;
  QLabel *synergy_server_label;
  QLineEdit *synergy_server_edit;
  QPushButton *synergy_ok_button;
  QPushButton *synergy_cancel_button;
  Config *synergy_config;
  QProcess *synergy_process;
};


#endif  // SYNERGYDIALOG_H
