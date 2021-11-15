// rpiconfig.cpp
//
// Raspberry Pi Low-Level Configuration
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

#include "rpiconfig.h"

RpiConfig::RpiConfig()
{
  rpi_was_changed=false;
}


QSize RpiConfig::framebufferSize() const
{
  return rpi_framebuffer_size;
}


void RpiConfig::setFramebufferSize(const QSize &size)
{
  rpi_was_changed=rpi_was_changed||(size!=rpi_framebuffer_size);
  rpi_framebuffer_size=size;
}


bool RpiConfig::wasChanged() const
{
  return rpi_was_changed;
}

  
void RpiConfig::load()
{
  FILE *f=NULL;
  char line[1024];
  QString qline;

  if((f=fopen(VPICK_RPICONF_FILE,"r"))!=NULL) {
    rpi_lines.clear();
    while(fgets(line,1024,f)!=NULL) {
      qline=QString(line).trimmed();
      //      printf("LINE: %s\n",(const char *)qline.toUtf8());
      if(qline.left(1)=="#") {
	rpi_lines.push_back(qline);
      }
      else {
	bool used=false;
	QStringList f0=qline.split("=");
	if(f0.size()>1) {
	  if(f0[0]=="framebuffer_width") {
	    rpi_framebuffer_size.setWidth(f0[1].toInt(&used));
	  }
	  if(f0[0]=="framebuffer_height") {
	    rpi_framebuffer_size.setHeight(f0[1].toInt(&used));
	  }
	}
	if(!used) {
	  rpi_lines.push_back(qline);
	}
      }
    }
    fclose(f);
  }
  rpi_was_changed=false;
}


void RpiConfig::save()
{
  FILE *f=NULL;

  if((f=fopen(VPICK_RPICONF_FILE,"w"))!=NULL) {
    for(int i=0;i<rpi_lines.size();i++) {
      fprintf(f,"%s\n",(const char *)rpi_lines[i].toUtf8());
    }
    if((rpi_framebuffer_size.width()>0)&&(rpi_framebuffer_size.height()>0)) {
      fprintf(f,"framebuffer_width=%d\n",rpi_framebuffer_size.width());
      fprintf(f,"framebuffer_height=%d\n",rpi_framebuffer_size.height());
    }
    fclose(f);
  }
}
