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
#include <QProcess>
#include <QPushButton>
#include <QSignalMapper>
#include <QTimer>

#define VPICK_CONF_FILE "/etc/vpick.conf"
#define VPICK_USAGE "[options]\n"

class MainWidget : public QMainWindow
{
 Q_OBJECT;
 public:
  MainWidget(QWidget *parent=0);
  QSize sizeHint() const;

 private slots:
  void buttonClickedData(int id);
  void processErrorData(QProcess::ProcessError err);
  void processFinishedData(int exit_code,QProcess::ExitStatus status);
  void processKillData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  void LoadHosts();
  std::vector<QPushButton *> vpick_buttons;
  std::vector<QString> vpick_commands;
  QSignalMapper *vpick_button_mapper;
  int vpick_height;
  QProcess *vpick_process;
  QTimer *vpick_process_timer;
};


#endif  // VPICK_USAGE_H
