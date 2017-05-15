// synergynodedialog.h
//
// Configure a Synergy node.
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

#ifndef SYNERGYNODEDIALOG_H
#define SYNERGYNODEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class SynergyNodeDialog : public QDialog
{
 Q_OBJECT;
 public:
  SynergyNodeDialog(QWidget *parent=0);
  QSize sizeHint() const;

 public slots:
  int exec(QString *screenname);

 private slots:
  void okData();
  void cancelData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *synergy_screenname_label;
  QLineEdit *synergy_screenname_edit;
  QPushButton *synergy_ok_button;
  QPushButton *synergy_cancel_button;
  QString *synergy_screenname;
};


#endif  // SYNERGYNODEDIALOG_H
