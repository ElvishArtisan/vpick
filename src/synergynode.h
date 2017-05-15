// synergynode.h
//
// Synergy server configuration node
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

#ifndef SYNERGYNODE_H
#define SYNERGYNODE_H

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFrame>
#include <QLabel>

#include "synergynodedialog.h"

class SynergyNode : public QWidget
{
 Q_OBJECT;
 public:
  SynergyNode(QWidget *parent=0,bool is_root=false);
  bool isRoot() const;
  bool isActive() const;
  void setActive(bool state);
  QString screenName() const;
  void setScreenName(const QString &str);
  bool placed() const;
  void setPlaced(bool state);

 protected:
  void dragEnterEvent(QDragEnterEvent *e);
  void dropEvent(QDropEvent *e);
  void mouseDoubleClickEvent(QMouseEvent *e);
  void mousePressEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  bool node_is_root;
  QLabel *node_icon_label;
  QLabel *node_screenname_label;
  bool node_active;
  bool node_placed;
  QPoint node_drag_start_pos;
  SynergyNodeDialog *node_dialog;
};


#endif  // SYNERGYNODE_H
