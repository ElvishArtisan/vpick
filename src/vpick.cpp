// vpick.cpp
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

#include <stdio.h>
#include <stdlib.h>

#include <QApplication>
#include <QMessageBox>
#include <QStringList>

#include "cmdswitch.h"
#include "profile.h"
#include "vpick.h"

MainWidget::MainWidget(QWidget *parent)
  : QMainWindow(parent)
{
  CmdSwitch *cmd=new CmdSwitch(qApp->argc(),qApp->argv(),"vpick",VERSION);
  for(unsigned i=0;i<cmd->keys();i++) {
    if(!cmd->processed(i)) {
      fprintf(stderr,"unknown option\n");
      exit(256);
    }
  }
  vpick_process=NULL;

  //
  // Button Mapper
  //
  vpick_button_mapper=new QSignalMapper(this);
  connect(vpick_button_mapper,SIGNAL(mapped(int)),
	  this,SLOT(buttonClickedData(int)));

  LoadHosts();

  //
  // Process Timer
  //
  vpick_process_timer=new QTimer(this);
  vpick_process_timer->setSingleShot(true);
  connect(vpick_process_timer,SIGNAL(timeout()),this,SLOT(processKillData()));

  setMaximumSize(sizeHint());
  setMinimumSize(sizeHint());
}


QSize MainWidget::sizeHint() const
{
  return QSize(220,vpick_height);
}


void MainWidget::buttonClickedData(int id)
{
  if(vpick_process==NULL) {
    vpick_process=new QProcess(this);
    connect(vpick_process,SIGNAL(error(QProcess::ProcessError)),
	    this,SLOT(processErrorData(QProcess::ProcessError)));
    connect(vpick_process,SIGNAL(finished(int,QProcess::ExitStatus)),
	    this,SLOT(processFinishedData(int,QProcess::ExitStatus)));
    QStringList f0=vpick_commands[id].split(" ");
    QString cmd=f0[0];
    f0.erase(f0.begin());
    vpick_process->start(cmd,f0);
  }
}


void MainWidget::processErrorData(QProcess::ProcessError err)
{
  QMessageBox::critical(this,"VPick",tr("Process returned error")+
			QString().sprintf("%d!",err));
  vpick_process_timer->start(0);
}


void MainWidget::processFinishedData(int exit_code,QProcess::ExitStatus status)
{
  vpick_process_timer->start(0);
}


void MainWidget::processKillData()
{
  delete vpick_process;
  vpick_process=NULL;
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  qApp->quit();
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  for(unsigned i=0;i<vpick_buttons.size();i++) {
    vpick_buttons[i]->setGeometry(10,10+50*i,200,40);
  }
}


void MainWidget::LoadHosts()
{
  QString cmd;
  bool ok=false;
  int count=0;
  QString section=QString().sprintf("Host%d",count+1);
  QFont font("helvetica",16,QFont::Bold);
  Profile *p=new Profile();
  p->setSource(VPICK_CONF_FILE);
  
  cmd=p->stringValue(section,"Command","",&ok);
  while(ok) {
    vpick_commands.push_back(cmd);
    vpick_buttons.push_back(new QPushButton(p->stringValue(section,"Title",QString().sprintf("Host %d",count+1)),this));
    vpick_buttons.back()->setFont(font);
    vpick_button_mapper->
      setMapping(vpick_buttons.back(),vpick_buttons.size()-1);
    connect(vpick_buttons.back(),SIGNAL(clicked()),
	    vpick_button_mapper,SLOT(map()));
    count++;
    section=QString().sprintf("Host%d",count+1);
    cmd=p->stringValue(section,"Command","",&ok);
  }
  delete p;
  vpick_height=10+50*vpick_buttons.size();
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);
  MainWidget *w=new MainWidget();
  w->show();
  return a.exec();
}
