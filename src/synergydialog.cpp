// synergydialog.cpp
//
// Configure Synergy
//
//   (C) Copyright 2017-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <QMessageBox>

#include "synergydialog.h"

SynergyDialog::SynergyDialog(Config *c,QWidget *parent)
  : QDialog(parent)
{
  synergy_config=c;
  synergy_process=NULL;

  setWindowTitle(tr("Host Picker - Configure Synergy"));

  QFont bold_font(font().family(),font().pointSize(),QFont::Bold);

  synergy_mode_box=new ComboBox(this);
  synergy_mode_box->insertItem(-1,tr("Disable Synergy"),Config::NoSynergy);
  synergy_mode_box->insertItem(-1,tr("Act as Synergy client"),Config::ClientSynergy);
  synergy_mode_box->insertItem(-1,tr("Act as Synergy server"),Config::ServerSynergy);
  connect(synergy_mode_box,SIGNAL(activated(int)),
	  this,SLOT(modeActivatedData(int)));

  synergy_screenname_label=new QLabel(tr("Screen Name")+":",this);
  synergy_screenname_label->setFont(bold_font);
  synergy_screenname_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  synergy_screenname_edit=new QLineEdit(this);

  synergy_server_label=new QLabel(tr("Server Address")+":",this);
  synergy_server_label->setFont(bold_font);
  synergy_server_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  synergy_server_edit=new QLineEdit(this);

  synergy_grid=new SynergyGrid(this);
  connect(synergy_screenname_edit,SIGNAL(textChanged(const QString &)),
	  synergy_grid,SLOT(setServerScreenName(const QString &)));
  synergy_grid->load();

  synergy_screen_source=new ScreenSource(this);

  synergy_trashcan=new Trashcan(this);

  synergy_ok_button=new QPushButton(tr("OK"),this);
  synergy_ok_button->setFont(bold_font);
  connect(synergy_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  synergy_cancel_button=new QPushButton(tr("Cancel"),this);
  synergy_cancel_button->setFont(bold_font);
  connect(synergy_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));

  startSynergy();
}


QSize SynergyDialog::sizeHint() const
{
  return QSize(800,450);
}


void SynergyDialog::startSynergy()
{
  QStringList args;

  switch(synergy_config->synergyMode()) {
  case Config::NoSynergy:
    synergy_process=NULL;
    break;

  case Config::ClientSynergy:
    args.push_back("--no-daemon");
    args.push_back("--restart");
    args.push_back("--name");
    args.push_back(synergy_config->synergyScreenname());
    args.push_back(synergy_config->synergyServerAddress().toString());
    synergy_process=new QProcess(this);
    synergy_process->start("/usr/bin/synergyc",args);

    if(!synergy_process->waitForStarted()) {
      QMessageBox::critical(this,tr("VPick - Process Error"),
			    tr("Unable to start synergy client!"));
    }
    break;

  case Config::ServerSynergy:
    args.push_back("--no-daemon");
    args.push_back("--restart");
    args.push_back("--name");
    args.push_back(synergy_config->synergyScreenname());
    synergy_process=new QProcess(this);
    synergy_process->start("/usr/bin/synergys",args);

    if(!synergy_process->waitForStarted()) {
      QMessageBox::critical(this,tr("VPick - Process Error"),
			    tr("Unable to start synergy server!"));
    }
    break;
  }  
}


void SynergyDialog::stopSynergy()
{
  if(synergy_process!=NULL) {
    synergy_process->terminate();
    if(!synergy_process->waitForFinished(5000)) {
      synergy_process->kill();
      synergy_process->waitForFinished();
    }
    delete synergy_process;
    synergy_process=NULL;
  }
}


int SynergyDialog::exec()
{
  synergy_mode_box->setCurrentItemData(synergy_config->synergyMode());
  synergy_screenname_edit->setText(synergy_config->synergyScreenname());
  synergy_server_edit->setText(synergy_config->synergyServerAddress().toString());
  modeActivatedData(synergy_mode_box->currentIndex());
  return QDialog::exec();
}


void SynergyDialog::modeActivatedData(int index)
{
  Config::SynergyMode mode=(Config::SynergyMode)synergy_mode_box->
    itemData(index).toInt();

  switch(mode) {
  case Config::NoSynergy:
    synergy_screenname_label->setDisabled(true);
    synergy_screenname_edit->setDisabled(true);
    synergy_server_label->setDisabled(true);
    synergy_server_edit->setDisabled(true);
    synergy_grid->setDisabled(true);
    synergy_screen_source->setDisabled(true);
    synergy_trashcan->setDisabled(true);
    break;

  case Config::ClientSynergy:
    synergy_screenname_label->setEnabled(true);
    synergy_screenname_edit->setEnabled(true);
    synergy_server_label->setEnabled(true);
    synergy_server_edit->setEnabled(true);
    synergy_grid->setDisabled(true);
    synergy_screen_source->setDisabled(true);
    synergy_trashcan->setDisabled(true);
    break;

  case Config::ServerSynergy:
    synergy_screenname_label->setEnabled(true);
    synergy_screenname_edit->setEnabled(true);
    synergy_server_label->setDisabled(true);
    synergy_server_edit->setDisabled(true);
    synergy_grid->setEnabled(true);
    synergy_screen_source->setEnabled(true);
    synergy_trashcan->setEnabled(true);
    break;
  }
}


void SynergyDialog::okData()
{
  Config::SynergyMode mode=(Config::SynergyMode)synergy_mode_box->
    currentItemData().toInt();
  QHostAddress addr(synergy_server_edit->text());

  if(synergy_screenname_edit->text().isEmpty()) {
    QMessageBox::warning(this,tr("VPick - Error"),
			 tr("You must provide a screen name."));
    return;
  }

  switch(mode) {
  case Config::NoSynergy:
    break;

  case Config::ClientSynergy:
    if(addr.isNull()) {
      QMessageBox::warning(this,tr("VPick - Error"),
			   tr("Invalid server IP address!"));
      return;
    }
    break;

  case Config::ServerSynergy:
    break;
  }
  synergy_config->
    setSynergyMode((Config::SynergyMode)synergy_mode_box->currentItemData().toInt());
  synergy_config->setSynergyServerAddress(addr);
  synergy_config->setSynergyScreenname(synergy_screenname_edit->text());
  synergy_config->save();
  synergy_grid->save();

  stopSynergy();
  startSynergy();

  done(true);
}
 

void SynergyDialog::cancelData()
{
  done(false);
}


void SynergyDialog::closeEvent(QCloseEvent *e)
{
  cancelData();
}


void SynergyDialog::resizeEvent(QResizeEvent *e)
{
  synergy_mode_box->setGeometry(10,10,size().width()/2,20);

  synergy_screenname_label->setGeometry(10,40,125,20);
  synergy_screenname_edit->setGeometry(140,40,size().width()-150,20);

  synergy_server_label->setGeometry(10,62,125,20);
  synergy_server_edit->setGeometry(140,62,150,20);

  synergy_grid->setGeometry(10,84,size().width()-20,size().height()-150);

  synergy_screen_source->setGeometry(20,size().height()-55,44,42);
  synergy_trashcan->setGeometry(90,size().height()-50,32,32);

  synergy_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);

  synergy_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}
