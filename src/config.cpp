// config.cpp
//
// vpick(1) Host Chooser Configuration
//
//   (C) Copyright 2016-2023 Fred Gleason <fredg@paravelsystems.com>
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
#include <unistd.h>

#include <QApplication>
#include <QObject>

#include "config.h"
#include "profile.h"

Config::Config(const QSize &screen_size)
{
#ifdef DESKTOP
  if(getenv("HOME")!=NULL) {
    conf_filename=QString(getenv("HOME"))+"/.vpickrc";
  }
  else {
    conf_filename="/etc/vpick.conf";
  }
#endif  // DESKTOP

  conf_screen_size=screen_size;
  conf_handedness=Config::RightHanded;
}


QSize Config::screenSize() const
{
  return conf_screen_size;
}


QSize Config::canvasSize() const
{
  int max_x=0;
  int max_y=0;

  for(int i=0;i<conf_positions.size();i++) {
    if(conf_positions.at(i).x()>max_x) {
      max_x=conf_positions.at(i).x();
    }
    if(conf_positions.at(i).y()>max_y) {
      max_y=conf_positions.at(i).y();
    }
  }
  return QSize(1+max_x,1+max_y);
}


Config::Handedness Config::pointerHandedness() const
{
  return conf_handedness;
}


void Config::setPointerHandedness(Handedness hand)
{
  conf_handedness=hand;
}


Config::SynergyMode Config::synergyMode() const
{
  return conf_synergy_mode;
}


void Config::setSynergyMode(Config::SynergyMode mode)
{
  conf_synergy_mode=mode;
}


QString Config::synergyScreenname() const
{
  return conf_synergy_screenname;
}


void Config::setSynergyScreenname(const QString &str)
{
  conf_synergy_screenname=str;
}


QHostAddress Config::synergyServerAddress() const
{
  return conf_synergy_server_address;
}


void Config::setSynergyServerAddress(const QHostAddress &addr)
{
  conf_synergy_server_address=addr;
}


int Config::hostQuantity() const
{
  return conf_types.size();
}


QPoint Config::position(int n) const
{
  return conf_positions.at(n);
}


void Config::setPosition(int n,const QPoint &pos)
{
  conf_positions[n]=pos;
}


void Config::setPosition(int n,int x,int y)
{
  conf_positions[n].setX(x);
  conf_positions[n].setY(y);
}


Config::Type Config::type(int n)
{
  return conf_types[n];
}


void Config::setType(int n,Config::Type type)
{
  conf_types[n]=type;
}


QString Config::title(int n) const
{
  return conf_titles[n];
}


void Config::setTitle(int n,const QString &str)
{
  conf_titles[n]=str;
}


QString Config::hostname(int n) const
{
  return conf_hostnames[n];
}


void Config::setHostname(int n,const QString &str)
{
  conf_hostnames[n]=str;
}


QString Config::password(int n) const
{
  return conf_passwords[n];
}


void Config::setPassword(int n,const QString &str)
{
  conf_passwords[n]=str;
}


bool Config::autoconnect(int n) const
{
  return conf_autoconnects[n];
}


void Config::setAutoconnect(int n,bool state)
{
  conf_autoconnects[n]=state;
}


bool Config::fullscreen(int n) const
{
  return conf_fullscreens[n];
}


void Config::setFullscreen(int n,bool state)
{
  conf_fullscreens[n]=state;
}


QColor Config::color(int n) const
{
  return conf_colors[n];
}


void Config::setColor(int n,const QColor &color)
{
  conf_colors[n]=color;
}

/*
QProcess *Config::viewerProcess(int n) const
{
  return conf_viewer_processes.at(n);
}


void Config::setViewerProcess(int n,QProcess *proc)
{
  conf_viewer_processes[n]=proc;
}


QString Config::startupFileName(int n) const
{
  return conf_startup_file_names.at(n);
}


void Config::setStartupFileName(int n,const QString &str)
{
  conf_startup_file_names[n]=str;
}
*/

int Config::addHost(Type type,const QString &title,const QString &hostname,
		    const QString &passwd,bool autoconnect,bool fullscreen,
		    const QColor &color)
{
  conf_positions.push_back(nextFreePosition());
  conf_types.push_back(type);
  conf_titles.push_back(title);
  conf_hostnames.push_back(hostname);
  conf_passwords.push_back(passwd);
  conf_autoconnects.push_back(autoconnect);
  conf_fullscreens.push_back(fullscreen);
  conf_colors.push_back(color);
  conf_startup_file_names.push_back(QString());
  conf_viewer_processes.push_back(NULL);

  return conf_types.size()-1;
}


void Config::removeHost(int n)
{
  if((conf_viewer_processes.at(n)!=NULL)&&
     (conf_viewer_processes.at(n)->state()==QProcess::Running)) {
    conf_viewer_processes.at(n)->terminate();
    qApp->processEvents();
    conf_viewer_processes.at(n)->deleteLater();
  }
  conf_viewer_processes.erase(conf_viewer_processes.begin()+n);
  if(!conf_startup_file_names.at(n).isEmpty()) {
    unlink(conf_startup_file_names.at(n).toUtf8());
    conf_startup_file_names.removeAt(n);
  }
  conf_positions.erase(conf_positions.begin()+n);
  conf_types.erase(conf_types.begin()+n);
  conf_titles.erase(conf_titles.begin()+n);
  conf_hostnames.erase(conf_hostnames.begin()+n);
  conf_passwords.erase(conf_passwords.begin()+n);
  conf_autoconnects.erase(conf_autoconnects.begin()+n);
  conf_fullscreens.erase(conf_fullscreens.begin()+n);
  conf_colors.erase(conf_colors.begin()+n);
}


bool Config::positionIsFree(const QPoint &pt) const
{

  for(int i=0;i<conf_positions.size();i++) {
    if(conf_positions.at(i)==pt) {
      return false;
    }
  }

  return true;
}


QPoint Config::nextFreePosition(bool *ok) const
{
  QSize screen_size=screenSize();

  for(int i=0;i<screen_size.width();i++) {
    for(int j=0;j<(screen_size.height()-1);j++) {  // Bottom row is navigation!
      bool found=false;
      for(int k=0;k<conf_positions.size();k++) {
	if((conf_positions.at(k).x()==i)&&(conf_positions.at(k).y()==j)) {
	  found=true;
	}
      }
      if(!found) {
	if(ok!=NULL) {
	  *ok=true;
	}
	return QPoint(i,j);
      }
    }
  }
  if(ok!=NULL) {
    *ok=false;
  }

  return QPoint(-1,-1);
}


bool Config::hasFreePosition() const
{
  QPoint pos=nextFreePosition();

  return (pos.x()>=0)&&(pos.y()>=0);
}


bool Config::load()
{
  Config::Type type;
  bool ok=false;
  int count=0;
  QString section=QString::asprintf("Host%d",count+1);
  Profile *p=new Profile();
#ifdef DESKTOP
  p->setSource(conf_filename);
#endif  // DESKTOP
#ifdef EMBEDDED
  p->setSource(VPICK_CONF_FILE);
#endif  // EMBEDDED

  //
  // Pointer Parameters
  //
  if(p->stringValue("Pointers","Handedness","right").toLower()=="left") {
    conf_handedness=Config::LeftHanded;
  }
  else {
    conf_handedness=Config::RightHanded;
  }

  //
  // Synergy Parameters
  //
  conf_synergy_mode=(Config::SynergyMode)p->intValue("Synergy","Mode");
  conf_synergy_screenname=
    p->stringValue("Synergy","Screenname",Config::hostName());
  conf_synergy_server_address=p->addressValue("Synergy","ServerAddress","");
  
  //
  // Clear previous Host entries
  //
  conf_positions.clear();
  conf_types.clear();
  conf_hostnames.clear();
  conf_passwords.clear();
  conf_autoconnects.clear();
  conf_fullscreens.clear();
  conf_colors.clear();
  conf_titles.clear();

  bool x_ok=false;
  bool y_ok=false;
  bool next_ok=false;
  QPoint next_free_pos=nextFreePosition(&next_ok);
  if(!next_ok) {
    return false;
  }

  //
  // Load new Host entries
  //
  type=(Config::Type)p->intValue(section,"Type",(int)Config::VncPlain,&ok);
  while(ok) {
    conf_positions.
      push_back(QPoint(p->intValue(section,"X",next_free_pos.x(),&x_ok),
		       p->intValue(section,"Y",next_free_pos.y(),&y_ok)));
    if((!x_ok)||(!y_ok)) {
      next_free_pos=nextFreePosition(&next_ok);
      if(!next_ok) {
	return false;
      }
    }
    conf_types.push_back(type);
    conf_hostnames.push_back(p->stringValue(section,"Hostname"));
    conf_passwords.push_back(p->stringValue(section,"Password"));
    conf_autoconnects.push_back(p->boolValue(section,"Autoconnect"));
    conf_fullscreens.push_back(p->boolValue(section,"Fullscreen"));
    conf_colors.push_back(p->stringValue(section,"Color"));
    conf_titles.push_back(p->stringValue(section,"Title",
					 QString::asprintf("Host %d",count+1)));
    conf_startup_file_names.push_back(QString());
    conf_viewer_processes.push_back(NULL);
    count++;
    section=QString::asprintf("Host%d",count+1);
    type=(Config::Type)p->intValue(section,"Type",(int)Config::VncPlain,&ok);
  }
  delete p;

  return true;
}


bool Config::save()
{
  FILE *f=NULL;

#ifdef DESKTOP
  if((f=fopen((conf_filename+"-back").toUtf8(),"w"))==NULL) {
#endif  // DESKTOP
#ifdef EMBEDDED
  if((f=fopen((QString(VPICK_CONF_FILE)+"-back").toUtf8(),"w"))==NULL) {
#endif  // EMBEDDED
    return false;
  }
  fprintf(f,"[Pointers]\n");
  if(conf_handedness==Config::LeftHanded) {
    fprintf(f,"Handedness=left\n");
  }
  else {
    fprintf(f,"Handedness=right\n");
  }
  fprintf(f,"\n");

  fprintf(f,"[Synergy]\n");
  fprintf(f,"Mode=%d\n",conf_synergy_mode);
  fprintf(f,"Screenname=%s\n",(const char *)conf_synergy_screenname.toUtf8());
  fprintf(f,"ServerAddress=%s\n",
	  (const char *)conf_synergy_server_address.toString().toUtf8());
  fprintf(f,"\n");
  for(int i=0;i<conf_types.size();i++) {
    fprintf(f,"[Host%u]\n",i+1);
    fprintf(f,"X=%d\n",conf_positions.at(i).x());
    fprintf(f,"Y=%d\n",conf_positions.at(i).y());
    fprintf(f,"Type=%u\n",conf_types.at(i));
    fprintf(f,"Title=%s\n",conf_titles.at(i).toUtf8().constData());
    fprintf(f,"Hostname=%s\n",conf_hostnames.at(i).toUtf8().constData());
    fprintf(f,"Password=%s\n",conf_passwords.at(i).toUtf8().constData());
    fprintf(f,"Autoconnect=%u\n",conf_autoconnects[i]);
    fprintf(f,"Fullscreen=%u\n",conf_fullscreens[i]);
    if(conf_colors[i].isValid()) {
      fprintf(f,"Color=%s\n",conf_colors[i].name().toUtf8().constData());
    }
    else {
      fprintf(f,"Color=\n");
    }
    fprintf(f,"\n");
  }
  fclose(f);
#ifdef DESKTOP
  rename((conf_filename+"-back").toUtf8(),conf_filename.toUtf8());
#endif  // DESKTOP
#ifdef EMBEDDED
  rename((VPICK_CONF_FILE+"-back").toUtf8(),VPICK_CONF_FILE.toUtf8());
#endif  // EMBEDDED

  return true;
}


bool Config::fixup()
{
  for(int i=0;i<hostQuantity();i++) {
    QPoint pos=position(i);
    if((pos.x()>=screenSize().width())||(pos.y()>=(screenSize().height()-1))) {
      //
      // Outside of the current screen, try to relocate to a free position
      //
      pos=nextFreePosition();
      if((pos.x()<0)||(pos.y()<0)) {  // No free positions left!
	return false;
      }
      setPosition(i,pos);
    }
  }

  return true;
}


QString Config::hostName()
{
  char hostname[256];
  QString ret="hostname";

  memset(hostname,0,256);
  if(gethostname(hostname,255)==0) {
    ret=hostname;
  }
  return ret;
}


QString Config::typeString(Config::Type type)
{
  QString ret=QObject::tr("Unknown");

  switch(type) {
  case Config::VncPlain:
    ret="VNC";
    break;

  case Config::Spice:
    ret="SPICE";
    break;

  case Config::LastType:
    break;
  }

  return ret;
}
