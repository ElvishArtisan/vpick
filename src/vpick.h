// vppick.h
//
// vpick(1) Host Chooser
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

#ifndef VPICK_H
#define VPICK_H

#include <vector>

#include <QLabel>
#include <QMainWindow>
#include <QPixmap>
#include <QProcess>
#include <QPushButton>
#include <QSignalMapper>
#include <QTimer>

#include "config.h"
#include "hostdialog.h"
#include "settingsdialog.h"

#define VPICK_USAGE "[options]\n"

class MainWidget : public QMainWindow
{
 Q_OBJECT;
 public:
  MainWidget(QWidget *parent=0);
  QSize sizeHint() const;

 private slots:
  void buttonClickedData(int id);
  void addClickedData();
  void setupToggledData(bool state);
  void removeToggledData(bool state);
  void settingsClickedData();
  void processErrorData(QProcess::ProcessError err);
  void processFinishedData(int exit_code,QProcess::ExitStatus status);
  void processKillData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  void EditViewer(int id);
  void StartViewer(int id);
  void SetButtonIcons(const QPixmap &pix);
  void LoadHosts();
  void AddHost(int id);
  void RemoveHost(int id);
  void SaveHosts();
  void Resize();
  std::vector<QPushButton *> vpick_buttons;
  std::vector<QString> vpick_commands;
  QSignalMapper *vpick_button_mapper;
  QPushButton *vpick_add_button;
  QPushButton *vpick_config_button;
  QPushButton *vpick_remove_button;
  QPushButton *vpick_settings_button;
  int vpick_height;
  QProcess *vpick_process;
  QTimer *vpick_process_timer;
  QString vpick_password_file;
  HostDialog *vpick_host_dialog;
  SettingsDialog *vpick_settings_dialog;
  Config *vpick_config;
  QPixmap *vpick_lightbulb_map;
  QPixmap *vpick_noexit_map;
  QPixmap *vpick_plussign_map;
  QPixmap *vpick_minussign_map;
};


#endif  // VPICK_USAGE_H
