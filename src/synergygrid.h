// synergygrid.h
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

#ifndef SYNERGYGRID_H
#define SYNERGYGRID_H

#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include "synergynode.h"

#define SYNERGYGRID_X_QUAN 5
#define SYNERGYGRID_Y_QUAN 3

class SynergyScreen
{
 public:
  SynergyScreen(const QString &name);
  QString screenName() const;
  QString up() const;
  void setUp(const QString &str);
  QString down() const;
  void setDown(const QString &str);
  QString left() const;
  void setLeft(const QString &str);
  QString right() const;
  void setRight(const QString &str);

 private:
  QString screen_name;
  QString screen_up;
  QString screen_down;
  QString screen_left;
  QString screen_right;
};


class SynergyGrid : public QWidget
{
 Q_OBJECT;
 public:
  SynergyGrid(QWidget *parent=0);
  QSize sizeHint() const;
  void load();
  void save();

 public slots:
  void setServerScreenName(const QString &str);

 protected:
  void paintEvent(QPaintEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  SynergyScreen *GetScreen(QList<SynergyScreen *> *screens,
			  const QString &name) const;
  SynergyNode *grid_nodes[SYNERGYGRID_X_QUAN][SYNERGYGRID_Y_QUAN];
};


#endif  // SYNERGYGRID_H
