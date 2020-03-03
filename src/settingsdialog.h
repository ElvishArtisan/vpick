// settingsdialog.h
//
// Configure the local host device
//
//   (C) Copyright 2016-2020 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <map>

#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "combobox.h"
#include "config.h"
#include "rpiconfig.h"
#include "synergydialog.h"

class SettingsDialog : public QDialog
{
 Q_OBJECT;
 public:
  SettingsDialog(Config *c,QWidget *parent=0);
  QSize sizeHint() const;
  void stopSynergy();

 public slots:
  int exec();

 private slots:
  void dhcpChangedData(int n);
  void calibrateData();
  void okData();
  void cancelData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  void Load();
  bool Save();
  bool ValidIp(const QString &str) const;
  QLabel *set_dhcp_label;
  QComboBox *set_dhcp_box;
  QLabel *set_ipaddress_label;
  QLineEdit *set_ipaddress_edit;
  QLabel *set_ipnetmask_label;
  QLineEdit *set_ipnetmask_edit;
  QLabel *set_ipgateway_label;
  QLineEdit *set_ipgateway_edit;
  QLabel *set_dns_label;
  QLineEdit *set_dns1_edit;
  QLineEdit *set_dns2_edit;
  QLabel *set_resolution_label;
  ComboBox *set_resolution_box;
  QPushButton *set_synergy_button;
  QPushButton *set_calibrate_button;
  QPushButton *set_ok_button;
  QPushButton *set_cancel_button;
  std::map<QString,QString> set_values;
  RpiConfig *set_rpiconfig;
  Config *set_config;
  SynergyDialog *set_synergy_dialog;
};


#endif  // SETTINGSDIALOG_H
