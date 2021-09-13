// layoutdialog.h
//
// Manage button layout
//
//   (C) Copyright 2021 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef LAYOUTDIALOG_H
#define LAYOUTDIALOG_H

#include <QDialog>
#include <QList>

#include "config.h"
#include "hostbutton.h"

class LayoutDialog : public QDialog
{
 Q_OBJECT;
 public:
  LayoutDialog(Config *config,QWidget *parent=0);
  QSize sizeHint() const;

 public slots:
  int exec();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);
  void paintEvent(QPaintEvent *e);
  void dragEnterEvent(QDragEnterEvent *e);
  void dropEvent(QDropEvent *e);

 private:
  QList<HostButton *> d_buttons;
  Config *d_config;
};


#endif  // LAYOUTDIALOG_H
