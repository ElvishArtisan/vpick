// synergygrid.cpp
//
// Synergy server configuration grid
//
//   (C) Copyright 2017 Fred Gleason <fredg@paravelsystems.com>
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

#include <QList>
#include <QPainter>
#include <QStringList>

#include "config.h"
#include "synergygrid.h"

SynergyScreen::SynergyScreen(const QString &name)
{
  screen_name=name;
}


QString SynergyScreen::screenName() const
{
  return screen_name;
}


QString SynergyScreen::up() const
{
  return screen_up;
}


void SynergyScreen::setUp(const QString &str)
{
  screen_up=str;
}


QString SynergyScreen::down() const
{
  return screen_down;
}


void SynergyScreen::setDown(const QString &str)
{
  screen_down=str;
}


QString SynergyScreen::left() const
{
  return screen_left;
}


void SynergyScreen::setLeft(const QString &str)
{
  screen_left=str;
}


QString SynergyScreen::right() const
{
  return screen_right;
}


void SynergyScreen::setRight(const QString &str)
{
  screen_right=str;
}




SynergyGrid::SynergyGrid(QWidget *parent)
  : QWidget(parent)
{
  for(int i=0;i<SYNERGYGRID_Y_QUAN;i++) {
    for(int j=0;j<SYNERGYGRID_X_QUAN;j++) {
      grid_nodes[j][i]=new SynergyNode(this,(i==1)&&(j==2));
    }
  }
}


QSize SynergyGrid::sizeHint() const
{
  return QSize(800,600);
}


void SynergyGrid::load()
{
  FILE *f=NULL;
  QString section="";
  QString node="";
  char data[1024];
  QList<SynergyScreen *> screens;
  SynergyScreen *scrn=NULL;

  //
  // Load the nodes
  //
  if((f=fopen(VPICK_SYNERGY_CONF_FILE.toUtf8(),"r"))!=NULL) {
    while(fgets(data,1024,f)!=NULL) {
      QString line=QString(data).trimmed();
      if((!line.isEmpty())&&(line.at(0)!=QChar('#'))) {
	if(section.isEmpty()) {
	  QStringList f0=line.split(":");
	  if((f0.size()==2)&&(f0.at(0).trimmed()=="section")) {
	    section=f0.at(1).trimmed();
	    continue;
	  }
	}
	if(section=="screens") {
	  if(line=="end") {
	    section="";
	    continue;
	  }
	  if(line.right(1).at(0)==QChar(':')) {
	    screens.push_back(new SynergyScreen(line.left(line.length()-1)));
	  }
	}
	if(section=="links") {
	  if(line=="end") {
	    section="";
	    continue;
	  }
	  if(line.right(1)==":") {
	    scrn=NULL;
	    for(int i=0;i<screens.size();i++) {
	      if(screens.at(i)->screenName()==line.left(line.length()-1)) {
		scrn=screens.at(i);
		break;
	      }
	    }
	  }
	  else {
	    if(scrn!=NULL) {
	      QStringList f0=line.split("=");
	      if(f0.size()==2) {
		if(f0.at(0).trimmed()=="right") {
		  scrn->setRight(f0.at(1).trimmed());
		}
		if(f0.at(0).trimmed()=="left") {
		  scrn->setLeft(f0.at(1).trimmed());
		}
		if(f0.at(0).trimmed()=="up") {
		  scrn->setUp(f0.at(1).trimmed());
		}
		if(f0.at(0).trimmed()=="down") {
		  scrn->setDown(f0.at(1).trimmed());
		}
	      }
	    }
	  }
	}
      }
    }
    fclose(f);

    //
    // Place the root element
    //
    if(screens.size()>0) {
      grid_nodes[2][1]->setScreenName(screens.at(0)->screenName());
      grid_nodes[2][1]->setActive(true);
    }
    else {
      grid_nodes[2][1]->setScreenName(Config::hostName());
      grid_nodes[2][1]->setActive(true);
    }

    //
    // Build node array
    //
    bool placed=true;
    while(placed) {
      placed=false;
      for(int i=0;i<SYNERGYGRID_Y_QUAN;i++) {
	for(int j=0;j<SYNERGYGRID_X_QUAN;j++) {
	  SynergyNode *node=grid_nodes[j][i];
	  if((!node->screenName().isEmpty())&&(!node->placed())) {
	    if((scrn=GetScreen(&screens,node->screenName()))==NULL) {
	      fprintf(stderr,"unknown screen \"%s\"\n",
		      (const char *)node->screenName().toUtf8());
	    }
	    else {
	      if((!scrn->left().isEmpty())&&(j>0)) {
		grid_nodes[j-1][i]->setScreenName(scrn->left());
		grid_nodes[j-1][i]->setActive(true);
	      }
	      if((!scrn->right().isEmpty())&&(j<(SYNERGYGRID_X_QUAN-1))) {
		grid_nodes[j+1][i]->setScreenName(scrn->right());
		grid_nodes[j+1][i]->setActive(true);
	      }
	      if((!scrn->up().isEmpty())&&(i>0)) {
		grid_nodes[j][i-1]->setScreenName(scrn->up());
		grid_nodes[j][i-1]->setActive(true);
	      }
	      if((!scrn->down().isEmpty())&&(i<(SYNERGYGRID_Y_QUAN-1))) {
		grid_nodes[j][i+1]->setScreenName(scrn->down());
		grid_nodes[j][i+1]->setActive(true);
	      }
	    }
	    node->setPlaced(true);
	    placed=true;
	  }
	}
      }
    }
  }
}


void SynergyGrid::save()
{
  FILE *f=NULL;

  //
  // Screens section
  //
  if((f=fopen((VPICK_SYNERGY_CONF_FILE+"-new").toUtf8(),"w"))!=NULL) {
    fprintf(f,"section: screens\n");
    if(grid_nodes[2][1]->isActive()) {  // First entry should be the center
      fprintf(f,"\t%s:\n",
	      (const char *)grid_nodes[2][1]->screenName().toUtf8());
    }
    for(int i=0;i<SYNERGYGRID_Y_QUAN;i++) {
      for(int j=0;j<SYNERGYGRID_X_QUAN;j++) {
	if((j!=2)||(i!=1)) {
	  if(grid_nodes[j][i]->isActive()) {
	    fprintf(f,"\t%s:\n",
		    (const char *)grid_nodes[j][i]->screenName().toUtf8());
	  }
	}
      }
    }
    fprintf(f,"end\n");
    fprintf(f,"\n");

    //
    // Links section
    //
    fprintf(f,"section: links\n");
    for(int i=0;i<SYNERGYGRID_Y_QUAN;i++) {
      for(int j=0;j<SYNERGYGRID_X_QUAN;j++) {
	if(grid_nodes[j][i]->isActive()) {
	  fprintf(f,"\t%s:\n",
		  (const char *)grid_nodes[j][i]->screenName().toUtf8());
	  if((j>0)&&grid_nodes[j-1][i]->isActive()) {
	    fprintf(f,"\t\tleft = %s\n",
		    (const char *)grid_nodes[j-1][i]->screenName().toUtf8());
	  }
	  if((j<(SYNERGYGRID_X_QUAN-1))&&grid_nodes[j+1][i]->isActive()) {
	    fprintf(f,"\t\tright = %s\n",
		    (const char *)grid_nodes[j+1][i]->screenName().toUtf8());
	  }
	  if((i>0)&&grid_nodes[j][i-1]->isActive()) {
	    fprintf(f,"\t\tup = %s\n",
		    (const char *)grid_nodes[j][i-1]->screenName().toUtf8());
	  }
	  if((i<(SYNERGYGRID_Y_QUAN-1))&&grid_nodes[j][i+1]->isActive()) {
	    fprintf(f,"\t\tdown = %s\n",
		    (const char *)grid_nodes[j][i+1]->screenName().toUtf8());
	  }
	}
      }
    }
    fprintf(f,"end\n");

    fclose(f);
    rename((VPICK_SYNERGY_CONF_FILE+"-new").toUtf8(),
	   VPICK_SYNERGY_CONF_FILE.toUtf8());
  }
}


void SynergyGrid::setServerScreenName(const QString &str)
{
  grid_nodes[2][1]->setScreenName(str);
}


void SynergyGrid::paintEvent(QPaintEvent *e)
{
  QPainter *p=new QPainter(this);
  int w=size().width()-1;
  int h=size().height()-1;

  p->setPen(Qt::black);
  p->setBrush(Qt::black);
  for(int i=0;i<=SYNERGYGRID_Y_QUAN;i++) {
    p->drawLine(0,i*h/SYNERGYGRID_Y_QUAN,w,i*h/SYNERGYGRID_Y_QUAN);
  }
  for(int j=0;j<=SYNERGYGRID_X_QUAN;j++) {
    p->drawLine(j*w/SYNERGYGRID_X_QUAN,0,j*w/SYNERGYGRID_X_QUAN,h);
  }

  delete p;
}


void SynergyGrid::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();
  for(int i=0;i<SYNERGYGRID_Y_QUAN;i++) {
    for(int j=0;j<SYNERGYGRID_X_QUAN;j++) {
      grid_nodes[j][i]->setGeometry(5+j*w/SYNERGYGRID_X_QUAN,
				    5+i*h/SYNERGYGRID_Y_QUAN,
				    w/SYNERGYGRID_X_QUAN,
				    h/SYNERGYGRID_Y_QUAN);
    }
  }
}


SynergyScreen *SynergyGrid::GetScreen(QList<SynergyScreen *> *screens,
				     const QString &name) const
{
  for(int i=0;i<screens->size();i++) {
    if(screens->at(i)->screenName()==name) {
      return screens->at(i);
    }
  }
  return NULL;
}
