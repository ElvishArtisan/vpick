// config.cpp
//
// vpick(1) Host Chooser Configuration
//
//   (C) Copyright 2016-2021 Fred Gleason <fredg@paravelsystems.com>
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

#include <QObject>

#include "config.h"
#include "profile.h"

Config::Config()
{
#ifdef DESKTOP
  if(getenv("HOME")!=NULL) {
    conf_filename=QString(getenv("HOME"))+"/vpick.conf";
  }
  else {
    conf_filename="/etc/vpick.conf";
  }
#endif  // DESKTOP
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


unsigned Config::hostQuantity() const
{
  return conf_types.size();
}


Config::Type Config::type(unsigned n)
{
  return conf_types[n];
}


void Config::setType(unsigned n,Config::Type type)
{
  conf_types[n]=type;
}


QString Config::title(unsigned n) const
{
  return conf_titles[n];
}


void Config::setTitle(unsigned n,const QString &str)
{
  conf_titles[n]=str;
}


QString Config::hostname(unsigned n) const
{
  return conf_hostnames[n];
}


void Config::setHostname(unsigned n,const QString &str)
{
  conf_hostnames[n]=str;
}


QString Config::password(unsigned n) const
{
  return conf_passwords[n];
}


void Config::setPassword(unsigned n,const QString &str)
{
  conf_passwords[n]=str;
}


bool Config::autoconnect(unsigned n) const
{
  return conf_autoconnects[n];
}


void Config::setAutoconnect(unsigned n,bool state)
{
  conf_autoconnects[n]=state;
}


bool Config::fullscreen(unsigned n) const
{
  return conf_fullscreens[n];
}


void Config::setFullscreen(unsigned n,bool state)
{
  conf_fullscreens[n]=state;
}


QColor Config::color(unsigned n) const
{
  return conf_colors[n];
}


void Config::setColor(unsigned n,const QColor &color)
{
  conf_colors[n]=color;
}


unsigned Config::addHost(Type type,const QString &title,const QString &hostname,
			 const QString &passwd, bool autoconnect,
			 bool fullscreen,Qt::GlobalColor color)
{
  conf_types.push_back(type);
  conf_titles.push_back(title);
  conf_hostnames.push_back(hostname);
  conf_passwords.push_back(passwd);
  conf_autoconnects.push_back(autoconnect);
  conf_fullscreens.push_back(fullscreen);
  conf_colors.push_back(color);

  return conf_types.size()-1;
}


void Config::removeHost(unsigned n)
{
  conf_types.erase(conf_types.begin()+n);
  conf_titles.erase(conf_titles.begin()+n);
  conf_hostnames.erase(conf_hostnames.begin()+n);
  conf_passwords.erase(conf_passwords.begin()+n);
  conf_autoconnects.erase(conf_autoconnects.begin()+n);
  conf_fullscreens.erase(conf_fullscreens.begin()+n);
  conf_colors.erase(conf_colors.begin()+n);
}


bool Config::load()
{
  Config::Type type;
  bool ok=false;
  int count=0;
  QString section=QString().sprintf("Host%d",count+1);
  Profile *p=new Profile();
#ifdef DESKTOP
  p->setSource(conf_filename);
#endif  // DESKTOP
#ifdef EMBEDDED
  p->setSource(VPICK_CONF_FILE);
#endif  // EMBEDDED

  conf_synergy_mode=(Config::SynergyMode)p->intValue("Synergy","Mode");
  conf_synergy_screenname=
    p->stringValue("Synergy","Screenname",Config::hostName());
  conf_synergy_server_address=p->addressValue("Synergy","ServerAddress","");
  
  type=(Config::Type)p->intValue(section,"Type",(int)Config::VncPlain,&ok);
  while(ok) {
    conf_types.push_back(type);
    conf_hostnames.push_back(p->stringValue(section,"Hostname"));
    conf_passwords.push_back(p->stringValue(section,"Password"));
    conf_autoconnects.push_back(p->boolValue(section,"Autoconnect"));
    conf_fullscreens.push_back(p->boolValue(section,"Fullscreen"));
    conf_colors.push_back(p->stringValue(section,"Color"));
    conf_titles.push_back(p->stringValue(section,"Title",QString().
					 sprintf("Host %d",count+1)));
    count++;
    section=QString().sprintf("Host%d",count+1);
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
  fprintf(f,"[Synergy]\n");
  fprintf(f,"Mode=%d\n",conf_synergy_mode);
  fprintf(f,"Screenname=%s\n",(const char *)conf_synergy_screenname.toUtf8());
  fprintf(f,"ServerAddress=%s\n",
	  (const char *)conf_synergy_server_address.toString().toUtf8());
  fprintf(f,"\n");
  for(int i=0;i<conf_types.size();i++) {
    fprintf(f,"[Host%u]\n",i+1);
    fprintf(f,"Type=%u\n",conf_types[i]);
    fprintf(f,"Title=%s\n",(const char *)conf_titles[i].toUtf8());
    fprintf(f,"Hostname=%s\n",(const char *)conf_hostnames[i].toUtf8());
    fprintf(f,"Password=%s\n",(const char *)conf_passwords[i].toUtf8());
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
