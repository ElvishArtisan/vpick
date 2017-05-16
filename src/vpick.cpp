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
#include <string.h>
#include <unistd.h>

#include <QApplication>
#include <QMessageBox>
#include <QProcess>
#include <QResizeEvent>
#include <QStringList>

#include "cmdswitch.h"
#include "network.h"
#include "profile.h"
#include "vpick.h"

#include "../icons/lightbulb.xpm"
#include "../icons/minussign.xpm"
#include "../icons/noexit.xpm"
#include "../icons/plussign.xpm"
#include "../icons/settings.xpm"
#include "../icons/vpick-16x16.xpm"

MainWidget::MainWidget(QWidget *parent)
  : QMainWindow(parent)
{
  vpick_autoconnect_id=-1;

  CmdSwitch *cmd=new CmdSwitch(qApp->argc(),qApp->argv(),"vpick",VERSION);
  for(unsigned i=0;i<cmd->keys();i++) {
    if(!cmd->processed(i)) {
      fprintf(stderr,"unknown option\n");
      exit(256);
    }
  }
  vpick_process=NULL;
  setWindowTitle(tr("VNC Picker")+" ["+
  		 InterfaceIPv4Address(VPICK_NETWORK_INTERFACE).toString()+"]");
  setWindowIcon(QPixmap(vpick_16x16_xpm));

  //
  // Button Mapper
  //
  vpick_button_mapper=new QSignalMapper(this);
  connect(vpick_button_mapper,SIGNAL(mapped(int)),
	  this,SLOT(buttonClickedData(int)));

  vpick_config=new Config();
  vpick_config->load();
  LoadHosts();

  //
  // Dialogs
  //
  vpick_host_dialog=new HostDialog(vpick_config,this);
  vpick_settings_dialog=new SettingsDialog(vpick_config,this);

  //
  // Icons
  //
  vpick_minussign_map=new QPixmap(minussign_xpm);
  vpick_noexit_map=new QPixmap(noexit_xpm);
  vpick_plussign_map=new QPixmap(plussign_xpm);
  vpick_lightbulb_map=new QPixmap(lightbulb_xpm);

  //
  // Add Button
  //
  vpick_add_button=new QPushButton(this);
  vpick_add_button->setIcon(*vpick_plussign_map);
  connect(vpick_add_button,SIGNAL(clicked()),
	  this,SLOT(addClickedData()));

  //
  // Config Button
  //
  vpick_config_button=new QPushButton(this);
  vpick_config_button->setIcon(*vpick_lightbulb_map);
  vpick_config_button->setCheckable(true);
  connect(vpick_config_button,SIGNAL(toggled(bool)),
	  this,SLOT(setupToggledData(bool)));

  //
  // Remove Button
  //
  vpick_remove_button=new QPushButton(this);
  vpick_remove_button->setIcon(*vpick_minussign_map);
  vpick_remove_button->setCheckable(true);
  connect(vpick_remove_button,SIGNAL(toggled(bool)),
	  this,SLOT(removeToggledData(bool)));

  //
  // Settings Button
  //
  vpick_settings_button=new QPushButton(this);
  vpick_settings_button->setIcon(QPixmap(settings_xpm));
  connect(vpick_settings_button,SIGNAL(clicked()),
	  this,SLOT(settingsClickedData()));

  //
  // Process Timer
  //
  vpick_process_timer=new QTimer(this);
  vpick_process_timer->setSingleShot(true);
  connect(vpick_process_timer,SIGNAL(timeout()),this,SLOT(processKillData()));

  setMaximumSize(sizeHint());
  setMinimumSize(sizeHint());

  //
  // Autoconnect
  //
  for(unsigned i=0;i<vpick_config->hostQuantity();i++) {
    if(vpick_config->autoconnect(i)) {
      vpick_autoconnect_id=i;
      vpick_autoconnect_timer=new QTimer(this);
      connect(vpick_autoconnect_timer,SIGNAL(timeout()),
	      this,SLOT(autoconnectData()));
      vpick_autoconnect_timer->setSingleShot(true);
      vpick_autoconnect_timer->start(1);
      continue;
    }
  }
}


QSize MainWidget::sizeHint() const
{
  return QSize(220,vpick_height);
}


void MainWidget::buttonClickedData(int id)
{
  if(vpick_config_button->isChecked()) {
    EditViewer(id);
  }
  else {
    if(vpick_remove_button->isChecked()) {
      RemoveHost(id);
    }
    else {
      StartViewer(id);
    }
  }
}


void MainWidget::addClickedData()
{
  vpick_config_button->setChecked(false);
  vpick_remove_button->setChecked(false);

  vpick_config->addHost(Config::VncPlain,"[new host]","","",false);
  if(vpick_host_dialog->exec(vpick_config->hostQuantity()-1)==0) {
    AddHost(vpick_config->hostQuantity()-1);
    vpick_height+=50;
    Resize();
  }
  else {
    vpick_config->removeHost(vpick_config->hostQuantity()-1);
  }
}


void MainWidget::setupToggledData(bool state)
{
  if(state) {
    vpick_remove_button->setChecked(false);
    SetButtonIcons(*vpick_lightbulb_map);
  }
  else {
    SetButtonIcons(QPixmap());
  }
  vpick_add_button->setDisabled(state);
  vpick_remove_button->setDisabled(state);
  vpick_settings_button->setDisabled(state);
}


void MainWidget::removeToggledData(bool state)
{
  if(state) {
    vpick_add_button->setChecked(false);
    vpick_config_button->setChecked(false);
    SetButtonIcons(*vpick_noexit_map);
  }
  else {
    SetButtonIcons(QPixmap());
  }
  vpick_add_button->setDisabled(state);
  vpick_config_button->setDisabled(state);
  vpick_settings_button->setDisabled(state);
}


void MainWidget::settingsClickedData()
{
  if(vpick_settings_dialog->exec()==0) {
    QStringList args;
    args.push_back("restart");
    args.push_back("network");
    QProcess *proc=new QProcess();
    proc->start("systemctl",args);
    proc->waitForFinished();
    delete proc;
  }
}


void MainWidget::processErrorData(QProcess::ProcessError err)
{
  QMessageBox::critical(this,"VPick",tr("Process returned error")+
			QString().sprintf("%d!",err));
  vpick_process_timer->start(0);
}


void MainWidget::autoconnectData()
{
  buttonClickedData(vpick_autoconnect_id);
}


void MainWidget::processFinishedData(int exit_code,QProcess::ExitStatus status)
{
  vpick_process_timer->start(0);
}


void MainWidget::processKillData()
{
  unlink(vpick_password_file.toUtf8());
  delete vpick_process;
  vpick_process=NULL;
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  vpick_settings_dialog->stopSynergy();
  qApp->quit();
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  for(unsigned i=0;i<vpick_buttons.size();i++) {
    vpick_buttons[i]->show();
    vpick_buttons[i]->setGeometry(10,10+50*i,200,40);
  }

  vpick_add_button->setGeometry(10,10+50*vpick_buttons.size(),40,40);
  vpick_config_button->setGeometry(55,10+50*vpick_buttons.size(),40,40);
  vpick_remove_button->setGeometry(100,10+50*vpick_buttons.size(),40,40);
  vpick_settings_button->
    setGeometry(size().width()-50,10+50*vpick_buttons.size(),40,40);
}


void MainWidget::EditViewer(int id)
{
  if(vpick_host_dialog->exec(id)==0) {
    vpick_buttons[id]->setText(vpick_config->title(id));
  }
}


void MainWidget::StartViewer(int id)
{
  char tempname[PATH_MAX];
  QStringList args;

  if(vpick_process==NULL) {
    //
    // Generate Password File
    //
    strncpy(tempname,"/tmp/vpickXXXXXX",PATH_MAX);
    vpick_password_file=mktemp(tempname);
    args.push_back("-f");
    vpick_process=new QProcess(this);
    vpick_process->setStandardOutputFile(vpick_password_file);
    vpick_process->start("/usr/bin/vncpasswd",args);
    vpick_process->write(vpick_config->password(id).toUtf8());
    vpick_process->write("\n");
    vpick_process->waitForFinished();
    delete vpick_process;

    //
    // Start Viewer
    //
    vpick_process=new QProcess(this);
    connect(vpick_process,SIGNAL(error(QProcess::ProcessError)),
	    this,SLOT(processErrorData(QProcess::ProcessError)));
    connect(vpick_process,SIGNAL(finished(int,QProcess::ExitStatus)),
	    this,SLOT(processFinishedData(int,QProcess::ExitStatus)));
    args.clear();
    args.push_back("-passwd");
    args.push_back(vpick_password_file);
    args.push_back("-fullscreen");
    args.push_back("-nograbkbd");  // So we don't break Synergy server
    args.push_back(vpick_config->hostname(id));
    vpick_process->start("/usr/lib/ssvnc/vncviewer",args);
  }
}


void MainWidget::LoadHosts()
{
  for(unsigned i=0;i<vpick_config->hostQuantity();i++) {
    AddHost(i);
  }
  vpick_height=10+50*(vpick_buttons.size()+1);
}


void MainWidget::AddHost(int id)
{
  QFont font("helvetica",16,QFont::Bold);
  font.setPixelSize(16);

  vpick_buttons.push_back(new QPushButton(vpick_config->title(id),this));
  vpick_buttons.back()->setFont(font);
  vpick_button_mapper->setMapping(vpick_buttons.back(),vpick_buttons.size()-1);
  connect(vpick_buttons.back(),SIGNAL(clicked()),
	  vpick_button_mapper,SLOT(map()));
}


void MainWidget::RemoveHost(int id)
{
  for(unsigned i=id+1;i<vpick_buttons.size();i++) {
    vpick_button_mapper->removeMappings(vpick_buttons[i]);
    vpick_button_mapper->setMapping(vpick_buttons[i],i-1);
  }
  vpick_config->removeHost(id);
  vpick_config->save();
  delete vpick_buttons[id];
  vpick_buttons.erase(vpick_buttons.begin()+id);
  vpick_height-=50;
  Resize();
}


void MainWidget::SetButtonIcons(const QPixmap &pix)
{
  for(unsigned i=0;i<vpick_buttons.size();i++) {
    vpick_buttons[i]->setIcon(pix);
  }
}


void MainWidget::SaveHosts()
{
}


void MainWidget::Resize()
{
  setMaximumHeight(vpick_height);
  setMinimumHeight(vpick_height);
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);
  MainWidget *w=new MainWidget();
  w->show();
  return a.exec();
}
