// vpick.cpp
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

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include <QApplication>
#include <QList>
#include <QMessageBox>
#include <QProcess>
#include <QResizeEvent>
#include <QStringList>

#include "cmdswitch.h"
#include "network.h"
#include "profile.h"
#include "viewerprocess.h"
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
  bool ok=false;
  int screen_number=0;
  bool debug=false;
  
  vpick_autoconnect_id=-1;
  vpick_display_profile=false;
  
  CmdSwitch *cmd=new CmdSwitch("vpick",VERSION,VPICK_USAGE);
  for(int i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--debug") {
      debug=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--display-profile") {
      vpick_display_profile=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--screen") {
      screen_number=cmd->value(i).toUInt(&ok);
      if(!ok) {
	fprintf(stderr,"vpick: invalid screen argument");
	exit(1);
      }
      cmd->setProcessed(i,true);
    }
    if(!cmd->processed(i)) {
      fprintf(stderr,"unknown option\n");
      exit(256);
    }
  }

  if(QGuiApplication::screens().size()>1) {
    setWindowTitle(tr("Host Picker")+
		   QString::asprintf(" [v%s, Screen %d]",
				     VERSION,screen_number));
  }
  else {
    setWindowTitle(tr("Host Picker")+" [v"+VERSION+"]");
  }
  setWindowIcon(QPixmap(vpick_16x16_xpm));

  //
  // Button Mapper
  //
  vpick_button_mapper=new QSignalMapper(this);
  connect(vpick_button_mapper,SIGNAL(mappedInt(int)),
	  this,SLOT(buttonClickedData(int)));
  vpick_buttonstate_timer=new QTimer(this);
  vpick_buttonstate_timer->setSingleShot(true);
  connect(vpick_buttonstate_timer,SIGNAL(timeout()),
	  this,SLOT(buttonstateData()));
  
  //
  // Load Configuration
  //
  vpick_config=new Config(screen_number);
  vpick_config->setDebug(debug);
  if((!vpick_config->load())||(!vpick_config->fixup())) {
    QMessageBox::critical(this,"VPick - "+tr("Error"),
			  tr("Too many buttons to fit on this screen!"));
    exit(1);
  }
  UpdatePointerDevices();

  //
  // Dialogs
  //
  vpick_host_dialog=new HostDialog(vpick_config,this);
  vpick_settings_dialog=new SettingsDialog(vpick_config,this);
  vpick_layout_dialog=new LayoutDialog(vpick_config,this);

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

  LoadHosts();

  Resize();

  //
  // Titlebar Updates
  //
  vpick_titlebar_timer=new QTimer(this);
  connect(vpick_titlebar_timer,SIGNAL(timeout()),this,SLOT(titlebarData()));
#ifdef EMBEDDED
  vpick_titlebar_timer->start(10000);
#endif  // EMBEDDED

  //
  // Intialize Button States and Autoconnect Hosts
  //
  for(int i=0;i<vpick_config->hostQuantity();i++) {
    vpick_buttons.at(i)->setChecked(vpick_config->liveWindowPid(i)>0);

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

  vpick_buttonstate_timer->start(0);
}


QSize MainWidget::sizeHint() const
{
  QSize canvas_size=vpick_config->canvasSize();
  return QSize(VPICK_BUTTON_MARGIN+(VPICK_BUTTON_MARGIN+VPICK_BUTTON_WIDTH)*canvas_size.width(),
	       VPICK_BUTTON_MARGIN+(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT)*(1+canvas_size.height()));
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
      if(vpick_config->liveWindowPid(id)>0) {
	switch(vpick_config->viewerButtonMode()) {
	case Config::ButtonToggles:
	  KillViewer(id);
	  break;

	case Config::ButtonRaises:
	  RaiseViewer(id);
	  break;
	}
      }
      else {
	StartViewer(id);
      }
    }
  }
}


void MainWidget::addClickedData()
{
  vpick_config_button->setChecked(false);
  vpick_remove_button->setChecked(false);

  vpick_config->
    addHost(Config::VncPlain,"[new host]","","",false,true,Qt::transparent);
  if(vpick_host_dialog->exec(vpick_config->hostQuantity()-1)==0) {
    AddHost(vpick_config->hostQuantity()-1);
    vpick_height+=50;
  }
  else {
    vpick_config->removeHost(vpick_config->hostQuantity()-1);
  }
  UpdateLayout();
  Resize();
  UpdateNavigationButtons();
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
  UpdateNavigationButtons();
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
  UpdateLayout();
  Resize();
  UpdateNavigationButtons();
}


void MainWidget::settingsClickedData()
{
#ifdef EMBEDDED
  if(vpick_settings_dialog->exec()==0) {
    vpick_config->save();
    UpdateLayout();
    Resize();
    UpdatePointerDevices();

    QStringList args;
    args.push_back("restart");
    args.push_back("network");
    QProcess *proc=new QProcess();
    proc->start("systemctl",args);
    proc->waitForFinished();
    delete proc;
    setWindowTitle(tr("VNC Picker")+" ["+
		  InterfaceIPv4Address(VPICK_NETWORK_INTERFACE).toString()+"]");
  }
  else {
    UpdateLayout();
    Resize();
  }
#endif  // EMBEDDED

#ifdef DESKTOP
  if(vpick_settings_dialog->exec()==0) {
    vpick_config->save();
    UpdateLayout();
    Resize();
  }
#endif  // DESKTOP
  UpdateNavigationButtons();
}

void MainWidget::autoconnectData()
{
  buttonClickedData(vpick_autoconnect_id);
}


void MainWidget::titlebarData()
{
  QHostAddress addr=InterfaceIPv4Address(VPICK_NETWORK_INTERFACE);
  if(addr.isNull()) {
    setWindowTitle(tr("Host Picker"));
  }
  else {
    setWindowTitle(tr("Host Picker")+" - "+addr.toString());
  }    
}


void MainWidget::processStartedData(int id)
{
  bool is_set=false;
  
  vpick_config->updateLiveParameters(id);

  //
  // Set Viewer Geometry
  //
  QPoint pos=vpick_config->windowPosition(id,&is_set);
  if(is_set) {
    QStringList args;
    args.push_back("-r");
    args.push_back(vpick_config->liveWindowId(id));
    args.push_back("-i");
    args.push_back("-e");
    args.push_back(QString::asprintf("0,%d,%d,%d,%d",
			    pos.x(),
			    pos.y(),
			    vpick_config->liveWindowGeometry(id).width(),
			    vpick_config->liveWindowGeometry(id).height()));
    ViewerProcess::sendCommand("/usr/bin/wmctrl",args);
    vpick_config->debugToWmctrl(args.join(" "));
  }

  if(vpick_config->fullscreen(id)) {
    QStringList args;
    args.push_back("-r");
    args.push_back(vpick_config->liveWindowId(id));
    args.push_back("-i");
    args.push_back("-b");
    args.push_back("add,fullscreen");
    ViewerProcess::sendCommand("/usr/bin/wmctrl",args);
    vpick_config->debugToWmctrl(args.join(" "));
  }
}


void MainWidget::processFinishedData(int id)
{
  vpick_config->clearLiveParameters(id);
  vpick_viewer_processes.remove(id);  // Memory is freed automatically!
}


void MainWidget::saveButtonPositionData(int id,const QPoint &pos)
{
  vpick_config->setWindowPosition(id,pos);
  vpick_config->save();
}


void MainWidget::buttonstateData()
{
  vpick_config->updateLiveParameters(-1);
  for(int i=0;i<vpick_buttons.size();i++) {
    vpick_buttons.at(i)->setChecked(vpick_config->liveWindowPid(i)>0);
  }
  vpick_buttonstate_timer->start(1000);
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  vpick_settings_dialog->stopSynergy();
  qApp->quit();
}


void MainWidget::resizeEvent(QResizeEvent *e)
{
  UpdateLayout();
}


void MainWidget::EditViewer(int id)
{
  if(vpick_host_dialog->exec(id)==0) {
    vpick_buttons[id]->setText(vpick_config->title(id),vpick_config->color(id));
  }
}


void MainWidget::StartViewer(int id)
{
  switch(vpick_config->type(id)) {
  case Config::VncPlain:
    StartVnc(id);
    break;

  case Config::Spice:
    StartSpice(id);
    break;

  case Config::LastType:
    break;
  }
}


void MainWidget::RaiseViewer(int id) const
{
  QStringList args;

  args.push_back("-a");
  args.push_back(vpick_config->title(id));
  vpick_config->debugToWmctrl(args.join(" "));
  ViewerProcess::sendCommand("/usr/bin/wmctrl",args);
}


void MainWidget::KillViewer(int id)
{
  if(vpick_viewer_processes.contains(id)) {
    vpick_viewer_processes.value(id)->terminate();
  }
  else {
    if(vpick_config->liveWindowPid(id)>0) {
      kill(vpick_config->liveWindowPid(id),SIGTERM);
      vpick_config->clearLiveParameters(id);
    }
  }
}


void MainWidget::StartVnc(int id)
{
  ViewerProcess *proc=NULL;
  QStringList args;
  QString conn_file;

#ifdef VIRTVIEWER
  conn_file=GenerateConnectionFile(id);
  if(conn_file.isEmpty()) {
    return;
  }
  args.push_back(conn_file);
  proc=new ViewerProcess(conn_file,vpick_display_profile,this);
  connect(proc,SIGNAL(started(int)),this,SLOT(processStartedData(int)));
  connect(proc,SIGNAL(finished(int)),this,SLOT(processFinishedData(int)));
  vpick_viewer_processes[id]=proc;
  proc->start(id,"/usr/bin/remote-viewer",args);
#endif  // VIRTVIEWER

#ifdef SSVNC
  conn_file=GenerateVncPassword(id);
  if(conn_file.isEmpty()) {
    return;
  }
  args.push_back("-x11cursor");
  args.push_back("-passwd");
  args.push_back(conn_file);
#ifdef EMBEDDED
  args.push_back("-fullscreen");
#endif  // EMBEDDED
#ifdef DESKTOP
  if(vpick_config->fullscreen(id)) {
    args.push_back("-fullscreen");
  }
#endif  // DESKTOP
  args.push_back("-nograbkbd");  // So we don't break Synergy server
  args.push_back(vpick_config->hostname(id));
  proc=new ViewerProcess(conn_file,this);
  connect(proc,SIGNAL(started(int)),this,SLOT(processStartedData(int)));
  connect(proc,SIGNAL(finished(int)),this,SLOT(processFinishedData(int)));
  vpick_viewer_processes[id]=proc;
  proc->start("/usr/lib/ssvnc/vncviewer",args);
#endif  // SSVNC

#ifdef TIGERVNC
  conn_file=GenerateVncPassword(id);
  if(conn_file.isEmpty()) {
    return;
  }
  args.push_back("-passwd");
  args.push_back(conn_file);
  args.push_back(vpick_config->hostname(id));
  proc=new ViewerProcess(conn_file,this);  
  connect(proc,SIGNAL(started(int)),this,SLOT(processStartedData(int)));
  connect(proc,SIGNAL(finished(int)),this,SLOT(processFinishedData(int)));
  vpick_viewer_processes[id]=proc;
  proc->start("/usr/bin/vncviewer",args);
#endif  // TIGERVNC

#ifdef REALVNC
  conn_file=GenerateVncPassword(id);
  if(conn_file.isEmpty()) {
    return;
  }
  args.push_back("-passwd");
  args.push_back(conn_file);
  if(vpick_config->fullscreen(id)) {
    args.push_back("--FullScreen");
  }
  args.push_back(vpick_config->hostname(id));
  proc=new ViewerProcess(conn_file,this);
  connect(proc,SIGNAL(started(int)),this,SLOT(processStartedData(int)));
  connect(proc,SIGNAL(finished(int)),this,SLOT(processFinishedData(int)));
  vpick_viewer_processes[id]=proc;
  proc->start("/usr/bin/vncviewer",args);
#endif  // REALVNC
}


QString MainWidget::GenerateVncPassword(int id)
{
  char tempname[PATH_MAX];
  QStringList args;
  int fd;
  QByteArray data;

  strncpy(tempname,"/tmp/vpickXXXXXX",PATH_MAX);
  if((fd=mkstemp(tempname))<0) {
    QMessageBox::critical(this,tr("Host Picker"),
			  tr("Unable to start viewer!")+"\n"+
			  "["+strerror(errno)+"].");
    return QString();
  }
  args.push_back("-f");
  ViewerProcess::sendCommand("/usr/bin/vncpasswd",args,&data);
  if(write(fd,data,data.size())!=data.size()) {
    QMessageBox::warning(this,"Vpick - "+tr("Error"),
		 tr("An error occurred when attempting to set a password!")+
			 QString::asprintf("[%s]",strerror(errno)));
  }
  ::close(fd);

  return QString(tempname);
}


void MainWidget::StartSpice(int id)
{
  QStringList args;
  QString conn_file=GenerateConnectionFile(id);

  if(conn_file.isEmpty()) {
    return;
  }
  args.push_back(conn_file);

  ViewerProcess *proc=new ViewerProcess(conn_file,vpick_display_profile,this);
  connect(proc,SIGNAL(started(int)),this,SLOT(processStartedData(int)));
  connect(proc,SIGNAL(finished(int)),this,SLOT(processFinishedData(int)));
  vpick_viewer_processes[id]=proc;
  proc->start(id,"/usr/bin/remote-viewer",args);
}


QString MainWidget::GenerateConnectionFile(int id)
{
  char tempname[PATH_MAX];
  FILE *f=NULL;
  QStringList f0=vpick_config->hostname(id).split(":");
  int fd=-1;

  strncpy(tempname,"/tmp/vpickXXXXXX",PATH_MAX);
  if((fd=mkstemp(tempname))<0) {
    QMessageBox::critical(this,tr("Host Picker"),
			  tr("Unable to start viewer!")+"\n"+
			  "["+strerror(errno)+"].");
    return QString();
  }
  if((f=fopen(tempname,"w"))==NULL) {
    QMessageBox::critical(this,tr("Host Picker"),
			  tr("Unable to start viewer!")+"\n"+
			  "["+strerror(errno)+"].");
    return QString();
  }
  fprintf(f,"[virt-viewer]\n");
  switch(vpick_config->type(id)) {
  case Config::VncPlain:
    fprintf(f,"type=vnc\n");
    break;

  case Config::Spice:
    fprintf(f,"type=spice\n");
    break;

  case Config::LastType:
    QMessageBox::critical(this,tr("Host Picker"),
			  tr("Unable to start viewer!")+"\n"+
			  "["+tr("Internal error")+"].");
    return QString();
  }
  fprintf(f,"host=%s\n",f0.at(0).toUtf8().constData());
  if(f0.size()==2) {
    fprintf(f,"port=%d\n",f0.at(1).toInt());
  }
  else {
    fprintf(f,"port=5900\n");
  }
  fprintf(f,"password=%s\n",vpick_config->password(id).toUtf8().constData());
  fprintf(f,"title=%s [%d]\n",
	  vpick_config->title(id).toUtf8().constData(),id);
#ifdef EMBEDDED
  fprintf(f,"fullscreen=1\n");
#endif  // EMBEDDED  
  fprintf(f,"delete-this-file=1\n");
  fclose(f);

  return QString(tempname);
}


void MainWidget::LoadHosts()
{
  for(int i=0;i<vpick_config->hostQuantity();i++) {
    AddHost(i);
  }
  vpick_height=10+50*(vpick_buttons.size()+1);
  UpdateNavigationButtons();
}


void MainWidget::AddHost(int id)
{
  QFont font("helvetica",16,QFont::Bold);
  font.setPixelSize(16);

  vpick_buttons.
    push_back(new HostButton(vpick_buttons.size(),vpick_config,this));
  vpick_buttons.back()->setFont(font);
  vpick_button_mapper->setMapping(vpick_buttons.back(),vpick_buttons.size()-1);
  connect(vpick_buttons.back(),SIGNAL(clicked()),
	  vpick_button_mapper,SLOT(map()));
  connect(vpick_buttons.back(),SIGNAL(savePosition(int,const QPoint &)),
	  this,SLOT(saveButtonPositionData(int,const QPoint &)));
}


void MainWidget::RemoveHost(int id)
{
  for(int i=id+1;i<vpick_buttons.size();i++) {
    vpick_button_mapper->removeMappings(vpick_buttons[i]);
    vpick_button_mapper->setMapping(vpick_buttons[i],i-1);
  }
  vpick_config->removeHost(id);
  vpick_config->save();
  delete vpick_buttons[id];
  vpick_buttons.erase(vpick_buttons.begin()+id);
  vpick_height-=50;
  UpdateNavigationButtons();
  Resize();
}


void MainWidget::SetButtonIcons(const QPixmap &pix)
{
  for(int i=0;i<vpick_buttons.size();i++) {
    vpick_buttons[i]->setIcon(pix);
  }
}


void MainWidget::UpdateNavigationButtons()
{
  vpick_add_button->setEnabled(vpick_config->hasFreePosition());
  vpick_config_button->setEnabled(vpick_config->hostQuantity()>0);
  vpick_remove_button->setEnabled(vpick_config->hostQuantity()>0);
#ifdef EMBEDDED
  vpick_settings_button->setEnabled(true);
#endif  // EMBEDDED

#ifdef DESKTOP
  vpick_settings_button->setEnabled((vpick_config->hostQuantity()>0)&&
				    vpick_config->hasFreePosition());
#endif  // DESKTOP
}


void MainWidget::UpdateLayout()
{
  int w=size().width();
  int h=size().height();

  for(int i=0;i<vpick_buttons.size();i++) {
    vpick_buttons[i]->show();
    vpick_buttons[i]->
      setGeometry(VPICK_BUTTON_MARGIN+(VPICK_BUTTON_MARGIN+VPICK_BUTTON_WIDTH)*vpick_config->position(i).x(),
		  VPICK_BUTTON_MARGIN+(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT)*vpick_config->position(i).y(),
		  VPICK_BUTTON_WIDTH,
		  VPICK_BUTTON_HEIGHT);
  }

  vpick_add_button->setGeometry(VPICK_BUTTON_MARGIN,
				h-(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT),
				VPICK_BUTTON_HEIGHT,
				VPICK_BUTTON_HEIGHT);
  vpick_config_button->setGeometry(3*VPICK_BUTTON_MARGIN/2+VPICK_BUTTON_HEIGHT,
				   h-(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT),
				   VPICK_BUTTON_HEIGHT,
				   VPICK_BUTTON_HEIGHT);
  vpick_remove_button->setGeometry(2*VPICK_BUTTON_MARGIN+2*VPICK_BUTTON_HEIGHT,
				   h-(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT),
				   VPICK_BUTTON_HEIGHT,
				   VPICK_BUTTON_HEIGHT);
  vpick_settings_button->
    setGeometry(w-(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT),
		h-(VPICK_BUTTON_MARGIN+VPICK_BUTTON_HEIGHT),
		VPICK_BUTTON_HEIGHT,
		VPICK_BUTTON_HEIGHT);
}


void MainWidget::Resize()
{
  setMaximumSize(sizeHint());
  setMinimumSize(sizeHint());
  resize(sizeHint());
}


void MainWidget::UpdatePointerDevices()
{
#ifdef EMBEDDED
  QStringList args;
  QProcess *proc=NULL;

  args.push_back("-e");
  if(vpick_config->pointerHandedness()==Config::LeftHanded) {
    args.push_back("pointer = 3 2 1");
  }
  else {
    args.push_back("pointer = 1 2 3");
  }
  proc=new QProcess(this);
  proc->start("/usr/bin/xmodmap",args);
  proc->waitForFinished();
  if(proc->exitStatus()!=QProcess::NormalExit) {
    QMessageBox::warning(this,tr("System Error"),
			 tr("Mouse configuration failed!")+"\n"+
			 "["+tr("xmodmap(1) crashed")+"]");
    delete proc;
    return;
  }
  if(proc->exitCode()!=0) {
    QMessageBox::warning(this,tr("System Error"),
			 tr("Mouse configuration failed!")+"\n"+
			 tr("xmodmap(1) returned error")+":\n"+
			 QString(proc->readAllStandardError()));
    delete proc;
    return;
  }
  delete proc;
#endif  // EMBEDDED
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);
  MainWidget *w=new MainWidget();
  w->show();

  return a.exec();
}
