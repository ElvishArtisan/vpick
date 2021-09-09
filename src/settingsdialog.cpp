// settingsdialog.cpp
//
// Configure system settings
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
#include <stdlib.h>

#include <QList>
#include <QMessageBox>
#include <QStringList>

#include "config.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(Config *c,QWidget *parent)
  : QDialog(parent)
{
  setWindowTitle(tr("VPick - Settings"));

  QFont small_button_font("helvetica",10,QFont::Bold);
  small_button_font.setPixelSize(10);

  QFont label_font("helvetica",14,QFont::Bold);
  label_font.setPixelSize(14);

  QFont button_font("helvetica",16,QFont::Bold);
  button_font.setPixelSize(16);

  //
  // Config Files
  //
  set_config=c;
  set_rpiconfig=new RpiConfig();

  //
  // Dialogs
  //
  set_synergy_dialog=new SynergyDialog(set_config,this);

  //
  // Configuration Type
  //
  set_dhcp_label=new QLabel(tr("Set Address")+":",this);
  set_dhcp_label->setFont(label_font);
  set_dhcp_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  set_dhcp_box=new QComboBox(this);
  set_dhcp_box->insertItem(0,tr("via DHCP"));
  set_dhcp_box->insertItem(1,tr("Manually"));
  connect(set_dhcp_box,SIGNAL(activated(int)),this,SLOT(dhcpChangedData(int)));

  //
  // IP Address
  //
  set_ipaddress_label=new QLabel(tr("IP Address")+":",this);
  set_ipaddress_label->setFont(label_font);
  set_ipaddress_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  set_ipaddress_edit=new QLineEdit(this);

  //
  // IP Netmask
  //
  set_ipnetmask_label=new QLabel(tr("IP Netmask")+":",this);
  set_ipnetmask_label->setFont(label_font);
  set_ipnetmask_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  set_ipnetmask_edit=new QLineEdit(this);

  //
  // IP Gateway
  //
  set_ipgateway_label=new QLabel(tr("IP Gateway")+":",this);
  set_ipgateway_label->setFont(label_font);
  set_ipgateway_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  set_ipgateway_edit=new QLineEdit(this);

  //
  // DNS Servers
  //
  set_dns_label=new QLabel(tr("DNS Servers")+":",this);
  set_dns_label->setFont(label_font);
  set_dns_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  set_dns1_edit=new QLineEdit(this);
  set_dns2_edit=new QLineEdit(this);

  //
  // Resolution
  //
  set_resolution_label=new QLabel(tr("Resolution")+":",this);
  set_resolution_label->setFont(label_font);
  set_resolution_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  set_resolution_box=new ComboBox(this);
  set_resolution_box->insertItem(0,tr("Automatic (recommended)"),QSize());
  set_resolution_box->insertItem(1,tr("640x480"),QSize(640,480));
  set_resolution_box->insertItem(1,tr("720x400"),QSize(720,400));
  set_resolution_box->insertItem(1,tr("800x600"),QSize(800,600));
  set_resolution_box->insertItem(1,tr("1024x786"),QSize(1024,786));
  set_resolution_box->insertItem(1,tr("1440x900"),QSize(1440,900));
  set_resolution_box->insertItem(1,tr("1600x900"),QSize(1600,900));
  set_resolution_box->insertItem(1,tr("1280x1024"),QSize(1280,1024));
  set_resolution_box->insertItem(1,tr("1280x960"),QSize(1280,960));
  set_resolution_box->insertItem(1,tr("1280x768"),QSize(1280,768));
  set_resolution_box->insertItem(1,tr("1280x720"),QSize(1280,720));
  set_resolution_box->insertItem(1,tr("1600x1200"),QSize(1680,1050));
  set_resolution_box->insertItem(1,tr("1680x1050"),QSize(1680,1050));
  set_resolution_box->insertItem(1,tr("1920x1080"),QSize(1920,1080));

  //
  // Synergy Button
  //
  set_synergy_button=
    new QPushButton(tr("Configure")+"\n"+tr("Synergy"),this);
  set_synergy_button->setFont(small_button_font);
  connect(set_synergy_button,SIGNAL(clicked()),set_synergy_dialog,SLOT(exec()));

  //
  // Calibrate Button
  //
  set_calibrate_button=
    new QPushButton(tr("Calibrate")+"\n"+tr("Touchscreen"),this);
  set_calibrate_button->setFont(small_button_font);
  connect(set_calibrate_button,SIGNAL(clicked()),this,SLOT(calibrateData()));

  //
  // OK Button
  //
  set_ok_button=new QPushButton(tr("OK"),this);
  set_ok_button->setFont(button_font);
  connect(set_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  set_cancel_button=new QPushButton(tr("Cancel"),this);
  set_cancel_button->setFont(button_font);
  connect(set_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));

#ifdef DESKTOP
  //
  // Disable IP settings controls
  //
  set_dhcp_label->setDisabled(true);
  set_dhcp_box->setDisabled(true);

  set_resolution_label->setDisabled(true);
  set_resolution_box->setDisabled(true);

  set_calibrate_button->setDisabled(true);
#endif  // DESKTOP
}


QSize SettingsDialog::sizeHint() const
{
  return QSize(400,265);
}


void SettingsDialog::stopSynergy()
{
  set_synergy_dialog->stopSynergy();
}


int SettingsDialog::exec()
{
  Load();
  return QDialog::exec();
}


void SettingsDialog::dhcpChangedData(int n)
{
  set_ipaddress_label->setEnabled(n);
  set_ipaddress_edit->setEnabled(n);
  set_ipnetmask_label->setEnabled(n);
  set_ipnetmask_edit->setEnabled(n);
  set_ipgateway_label->setEnabled(n);
  set_ipgateway_edit->setEnabled(n);
  set_dns_label->setEnabled(n);
  set_dns1_edit->setEnabled(n);
  set_dns2_edit->setEnabled(n);
}


void SettingsDialog::calibrateData()
{
  system("/usr/bin/xinput_calibrator --misclick 0");
}


void SettingsDialog::okData()
{
  if(Save()) {
    done(0);
  }
}


void SettingsDialog::cancelData()
{
  done(-1);
}


void SettingsDialog::closeEvent(QCloseEvent *e)
{
  cancelData();
}


void SettingsDialog::resizeEvent(QResizeEvent *e)
{
  set_dhcp_label->setGeometry(10,10,110,20);
  set_dhcp_box->setGeometry(125,10,100,20);

  set_ipaddress_label->setGeometry(10,32,110,20);
  set_ipaddress_edit->setGeometry(125,32,size().width()-135,20);

  set_ipnetmask_label->setGeometry(10,54,110,20);
  set_ipnetmask_edit->setGeometry(125,54,size().width()-135,20);

  set_ipgateway_label->setGeometry(10,76,110,20);
  set_ipgateway_edit->setGeometry(125,76,size().width()-135,20);

  set_dns_label->setGeometry(10,110,110,20);
  set_dns1_edit->setGeometry(125,110,size().width()-135,20);
  set_dns2_edit->setGeometry(125,132,size().width()-135,20);

  set_resolution_label->setGeometry(10,166,110,20);
  set_resolution_box->setGeometry(125,166,size().width()-135,20);

  set_synergy_button->setGeometry(10,size().height()-60,80,50);
  set_calibrate_button->setGeometry(100,size().height()-60,80,50);
  set_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  set_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void SettingsDialog::Load()
{
  FILE *f=NULL;
  char line[1024];

  if((f=fopen(("/etc/sysconfig/network-scripts/ifcfg-"+VPICK_NETWORK_INTERFACE).
	      toUtf8(),"r"))!=NULL) {
    while(fgets(line,1024,f)!=NULL) {
      QStringList f0=QString(line).trimmed().split("=",QString::KeepEmptyParts);
      QString hdr=f0[0];
      f0.erase(f0.begin());
      set_values[hdr]=f0.join("=");
    }
    fclose(f);
  }
  set_dhcp_box->setCurrentIndex(set_values["BOOTPROTO"].toLower()!="dhcp");
  set_ipaddress_edit->setText(set_values["IPADDR"]);
  set_ipnetmask_edit->setText(set_values["NETMASK"]);
  set_ipgateway_edit->setText(set_values["GATEWAY"]);
  set_dns1_edit->setText(set_values["DNS1"]);
  set_dns2_edit->setText(set_values["DNS2"]);
#ifdef DESKTOP
  set_dhcp_box->setCurrentIndex(0);
#endif  // DESKTOP

  set_rpiconfig->load();
  set_resolution_box->setCurrentItemData(set_rpiconfig->framebufferSize());
  dhcpChangedData(set_dhcp_box->currentIndex());
}


bool SettingsDialog::Save()
{
  FILE *f=NULL;
  QList<QString> dns_servers;

  if(set_dhcp_box->currentIndex()!=0) {  // Manual Setup
    if(!ValidIp(set_ipaddress_edit->text())) {
      QMessageBox::information(this,tr("VPick - Error"),
			       tr("Invalid IP Address value!"));
      return false;
    }
    if(!ValidIp(set_ipnetmask_edit->text())) {
      QMessageBox::information(this,tr("VPick - Error"),
			       tr("Invalid IP Netmask value!"));
      return false;
    }
    if(!ValidIp(set_ipgateway_edit->text())) {
      QMessageBox::information(this,tr("VPick - Error"),
			       tr("Invalid IP Gateway value!"));
      return false;
    }
    if(!set_dns1_edit->text().isEmpty()) {
      if(!ValidIp(set_dns1_edit->text())) {
	QMessageBox::information(this,tr("VPick - Error"),
				 tr("Invalid DNS Server value!"));
	return false;
      }
      dns_servers.push_back(set_dns1_edit->text());
    }
    if(!set_dns2_edit->text().isEmpty()) {
      if(!ValidIp(set_dns2_edit->text())) {
	QMessageBox::information(this,tr("VPick - Error"),
				 tr("Invalid DNS Server value!"));
	return false;
      }
      dns_servers.push_back(set_dns2_edit->text());
    }
    set_values["BOOTPROTO"]="none";
    set_values["IPADDR"]=set_ipaddress_edit->text();
    set_values["NETMASK"]=set_ipnetmask_edit->text();
    set_values["GATEWAY"]=set_ipgateway_edit->text();
  }
  else {   // DHCP Setup
    set_values["BOOTPROTO"]="dhcp";
    set_values["IPADDR"]="";
    set_values["NETMASK"]="";
    set_values["GATEWAY"]="";
  }
  if((f=fopen(("/etc/sysconfig/network-scripts/ifcfg-"+
	       VPICK_NETWORK_INTERFACE+"-back").toUtf8(),"w"))!=NULL) {
    for(QMap<QString,QString>::const_iterator it=set_values.begin();
	it!=set_values.end();it++) {
      if(it.key().left(3)!="DNS") {
	fprintf(f,"%s=%s\n",it.key().toUtf8().constData(),
		it.value().toUtf8().constData());
      }
    }
    for(int i=0;i<dns_servers.size();i++) {
      fprintf(f,"DNS%u=%s\n",i+1,dns_servers[i].toUtf8().constData());
    }
    fclose(f);
    rename(("/etc/sysconfig/network-scripts/ifcfg-"+VPICK_NETWORK_INTERFACE+
	    "-back").toUtf8(),
	   ("/etc/sysconfig/network-scripts/ifcfg-"+VPICK_NETWORK_INTERFACE).
	   toUtf8());
  }

  set_rpiconfig->
    setFramebufferSize(set_resolution_box->currentItemData().toSize());
  if(set_rpiconfig->wasChanged()) {
    set_rpiconfig->save();
#ifdef EMBEDDED
    system("/sbin/reboot");
#endif  // EMBEDDED
  }

  return true;
}


bool SettingsDialog::ValidIp(const QString &str) const
{
  int octet;
  bool ok=false;

  QStringList f0=str.split(".");
  if(f0.size()!=4) {
    return false;
  }
  for(int i=0;i<f0.size();i++) {
    octet=f0[i].toInt(&ok);
    if((!ok)||(octet<0)||(octet>255)) {
      return false;
    }
  }
  return true;
}
