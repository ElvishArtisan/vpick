// vppick.h
//
// vpick(1) Host Chooser
//
//   (C) Copyright 2016-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <QLabel>
#include <QList>
#include <QMainWindow>
#include <QMap>
#include <QPixmap>
#include <QProcess>
#include <QPushButton>
#include <QSignalMapper>
#include <QTimer>

#include "config.h"
#include "hostbutton.h"
#include "hostdialog.h"
#include "layoutdialog.h"
#include "settingsdialog.h"
#include "viewerprocess.h"

#define VPICK_USAGE "[--logical-screen-size=<x-buttons>x<y-buttons>] [--display-profile]\n"

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
  void autoconnectData();
  void titlebarData();
  void processStartedData(int id);
  void processFinishedData(int id);
  void saveButtonPositionData(int id,const QPoint &pos);
  
 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  void EditViewer(int id);
  void StartViewer(int id);
  void RaiseViewer(int id) const;
  void KillViewer(int id);
  void StartVnc(int id);
  QString GenerateVncPassword(int id);
  void StartSpice(int id);
  QString GenerateConnectionFile(int id);
  void SetButtonIcons(const QPixmap &pix);
  void LoadHosts();
  void AddHost(int id);
  void RemoveHost(int id);
  void UpdateNavigationButtons();
  void UpdateLayout();
  void Resize();
  void UpdatePointerDevices();
  QList<HostButton *> vpick_buttons;
  QList<QString> vpick_commands;
  QMap<int,ViewerProcess *> vpick_viewer_processes;
  QSignalMapper *vpick_button_mapper;
  QPushButton *vpick_add_button;
  QPushButton *vpick_config_button;
  QPushButton *vpick_remove_button;
  QPushButton *vpick_settings_button;
  int vpick_height;
  HostDialog *vpick_host_dialog;
  SettingsDialog *vpick_settings_dialog;
  LayoutDialog *vpick_layout_dialog;
  Config *vpick_config;
  QPixmap *vpick_lightbulb_map;
  QPixmap *vpick_noexit_map;
  QPixmap *vpick_plussign_map;
  QPixmap *vpick_minussign_map;
  QTimer *vpick_autoconnect_timer;
  int vpick_autoconnect_id;
  QTimer *vpick_titlebar_timer;
  bool vpick_display_profile;
};


#endif  // VPICK_USAGE_H
