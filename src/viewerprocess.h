// viewerprocess.h
//
// Process container for viewers
//
//   (C) Copyright 2023-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef VIEWERPROCESS_H
#define VIEWERPROCESS_H

#include <QMessageBox>
#include <QPoint>
#include <QProcess>
#include <QTimer>

class ViewerProcess : public QMessageBox
{
 Q_OBJECT;
 public:
  ViewerProcess(const QString &startup_filename,bool display_profile,
		QWidget *parent);
  ~ViewerProcess();
  void start(int id,const QString &cmd,const QStringList &args);
  void terminate();
  static bool sendCommand(const QString &cmd,const QStringList &args,
			  QByteArray *data_out=NULL);
  
 signals:
  void started(int id);
  void finished(int id);

 private slots:
  void startedData();
  void startupTimeoutData();
  void finishedData(int exit_code,QProcess::ExitStatus status);

 private:
  QProcess *d_process;
  QString d_startup_filename;
  QString d_command;
  QStringList d_arguments;
  int d_id;
  bool d_display_profile;
  QTimer *d_startup_timer;
  bool d_stopping;
};


#endif  // VIEWERPROCESS_H
