// viewerprocess.h
//
// Process container for viewers
//
//   (C) Copyright 2023-2024 Fred Gleason <fredg@paravelsystems.com>
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
#include <QProcess>

class ViewerProcess : public QMessageBox
{
 Q_OBJECT;
 public:
  ViewerProcess(const QString &startup_filename,bool display_profile,
		QWidget *parent);
  ~ViewerProcess();
  void start(const QString &cmd,const QStringList &args);

 private slots:
  void startedData();
  void finishedData(int exit_code,QProcess::ExitStatus status);

 private:
  QProcess *d_process;
  QString d_startup_filename;
  QString d_command;
  QStringList d_arguments;
  bool d_display_profile;
};


#endif  // VIEWERPROCESS_H
