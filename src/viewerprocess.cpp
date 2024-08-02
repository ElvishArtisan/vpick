// viewerprocess.cpp
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

#include <unistd.h>

#include <QTime>

#include "viewerprocess.h"

ViewerProcess::ViewerProcess(const QString &startup_filename,
			     bool display_profile,QWidget *parent)
  : QMessageBox(parent)
{
  d_startup_filename=startup_filename;
  d_display_profile=display_profile;

  d_process=new QProcess(this);
  connect(d_process,SIGNAL(started()),this,SLOT(startedData()));
  connect(d_process,SIGNAL(finished(int,QProcess::ExitStatus)),
	  this,SLOT(finishedData(int,QProcess::ExitStatus)));
}


ViewerProcess::~ViewerProcess()
{
  delete d_process;
}


void ViewerProcess::start(const QString &cmd,const QStringList &args)
{
  d_command=cmd;
  d_arguments=args;
  d_process->start(cmd,args);
  if(d_display_profile) {
    printf("\n");
    printf("%s - start(): %s %s\n",
	   QTime::currentTime().toString("hh:mm:ss.zzz").
	   toUtf8().constData(),
	   d_command.toUtf8().constData(),
	   d_arguments.join(" ").toUtf8().constData());
  }
}


void ViewerProcess::startedData()
{
  if(d_display_profile) {
    printf("%s - startedData(): %s %s\n",
	   QTime::currentTime().toString("hh:mm:ss.zzz").
	   toUtf8().constData(),
	   d_command.toUtf8().constData(),
	   d_arguments.join(" ").toUtf8().constData());
  }
}


void ViewerProcess::finishedData(int exit_code,QProcess::ExitStatus status)
{
  if(d_display_profile) {
    printf("%s - finishedData: %s %s\n",
	   QTime::currentTime().toString("hh:mm:ss.zzz").
	   toUtf8().constData(),
	   d_command.toUtf8().constData(),
	   d_arguments.join(" ").toUtf8().constData());
    printf("\n");
  }

  //
  // Process results
  //
  if(status!=QProcess::NormalExit) {
    setText(tr("The viewer process has crashed."));
    setDetailedText(tr("Process invocation was")+": "+
		    d_command+" "+d_arguments.join(" "));
    exec();
  }
  else {
    if(exit_code!=0) {
      setText(tr("The viewer process returned an error."));
      QString err_msg=
	QString::fromUtf8(d_process->readAllStandardError()).trimmed();
      if(err_msg.isEmpty()) {
	setDetailedText(tr("Process invocation was")+": "+
			d_command+" "+d_arguments.join(" ")+".");
      }
      else {
	setDetailedText(tr("Process invocation was")+": "+
			d_command+" "+d_arguments.join(" ")+".\n\n"+
			err_msg);
      }
      exec();
    }
  }

  //
  // Clean up
  //
  if(!d_startup_filename.isEmpty()) {
    unlink(d_startup_filename.toUtf8());
  }
  deleteLater();
}
