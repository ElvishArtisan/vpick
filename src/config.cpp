// config.cpp
//
// vpick(1) Host Chooser Configuration
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

#include "config.h"
#include "profile.h"

Config::Config()
{
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


unsigned Config::addHost(Type type,const QString &title,const QString &hostname,
			 const QString &passwd)
{
  conf_types.push_back(type);
  conf_titles.push_back(title);
  conf_hostnames.push_back(hostname);
  conf_passwords.push_back(passwd);

  return conf_types.size()-1;
}


void Config::removeHost(unsigned n)
{
  conf_types.erase(conf_types.begin()+n);
  conf_titles.erase(conf_titles.begin()+n);
  conf_hostnames.erase(conf_hostnames.begin()+n);
  conf_passwords.erase(conf_passwords.begin()+n);
}


bool Config::load()
{
  Config::Type type;
  bool ok=false;
  int count=0;
  QString section=QString().sprintf("Host%d",count+1);
  Profile *p=new Profile();
  p->setSource(VPICK_CONF_FILE);
  
  type=(Config::Type)p->intValue(section,"Type",(int)Config::VncPlain,&ok);
  while(ok) {
    conf_types.push_back(type);
    conf_hostnames.push_back(p->stringValue(section,"Hostname"));
    conf_passwords.push_back(p->stringValue(section,"Password"));
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

  if((f=fopen((QString(VPICK_CONF_FILE)+"-back").toUtf8(),"w"))==NULL) {
    return false;
  }
  for(unsigned i=0;i<conf_types.size();i++) {
    fprintf(f,"[Host%u]\n",i+1);
    fprintf(f,"Title=%s\n",(const char *)conf_titles[i].toUtf8());
    fprintf(f,"Hostname=%s\n",(const char *)conf_hostnames[i].toUtf8());
    fprintf(f,"Password=%s\n",(const char *)conf_passwords[i].toUtf8());
    fprintf(f,"\n");
  }
  fclose(f);
  rename((QString(VPICK_CONF_FILE)+"-back").toUtf8(),VPICK_CONF_FILE);

  return true;
}
