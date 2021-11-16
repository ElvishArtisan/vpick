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
#include <QProcess>

#include "config.h"
#include "network.h"
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
  set_layout_dialog=new LayoutDialog(set_config,this);
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
  set_dns_edits[0]=new QLineEdit(this);
  set_dns_edits[1]=new QLineEdit(this);

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
  // Handedness
  //
  set_handedness_label=new QLabel(tr("Mouse Mode")+":",this);
  set_handedness_label->setFont(label_font);
  set_handedness_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  set_handedness_box=new QComboBox(this);
  set_handedness_box->insertItem(0,tr("Right Handed"),Config::RightHanded);
  set_handedness_box->insertItem(1,tr("Left Handed"),Config::LeftHanded);

  //
  // Layout Button
  //
  set_layout_button=
    new QPushButton(tr("Configure")+"\n"+tr("Layout"),this);
  set_layout_button->setFont(small_button_font);
  connect(set_layout_button,SIGNAL(clicked()),set_layout_dialog,SLOT(exec()));

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
  return QSize(490,299);
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
  set_dns_edits[0]->setEnabled(n);
  set_dns_edits[1]->setEnabled(n);
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
  set_dns_edits[0]->setGeometry(125,110,size().width()-135,20);
  set_dns_edits[1]->setGeometry(125,132,size().width()-135,20);

  set_resolution_label->setGeometry(10,166,110,20);
  set_resolution_box->setGeometry(125,166,size().width()-135,20);

  set_handedness_label->setGeometry(10,200,110,20);
  set_handedness_box->setGeometry(125,200,size().width()-135,20);

  set_layout_button->setGeometry(10,size().height()-60,80,50);
  set_synergy_button->setGeometry(100,size().height()-60,80,50);
  set_calibrate_button->setGeometry(190,size().height()-60,80,50);

  set_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  set_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void SettingsDialog::Load()
{
  FILE *f=NULL;
  char line[1024];

  //
  // Check for devices that require calibration
  //
  bool calb_needed=false;
  QStringList args;
  args.push_back("--list");
  QProcess *proc=new QProcess(this);
  proc->start("/usr/bin/xinput_calibrator",args);
  proc->waitForFinished();
  calb_needed=(proc->exitStatus()==QProcess::NormalExit)&&(proc->exitCode()==0);
  delete proc;
  set_calibrate_button->setEnabled(calb_needed);

  //
  // Pointer Configuration
  //
  for(int i=0;i<set_handedness_box->count();i++) {
    if((Config::Handedness)set_handedness_box->itemData(i).toUInt()==
       set_config->pointerHandedness()) {
      set_handedness_box->setCurrentIndex(i);
    }
  }

  //
  // Network Configuration
  //
#ifdef REDHAT
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
  set_dns_edits[0]->setText(set_values["DNS1"]);
  set_dns_edits[1]->setText(set_values["DNS2"]);
#endif  // REDHAT

#ifdef DEBIAN
  bool ok=false;
  set_dhcp_box->setCurrentIndex(0);
  set_ipaddress_edit->setText("");
  set_ipnetmask_edit->setText("");
  set_ipgateway_edit->setText("");
  set_dns_edits[0]->setText("");
  set_dns_edits[1]->setText("");
  if((f=fopen("/etc/dhcpcd.conf","r"))!=NULL) {
    while(fgets(line,1024,f)!=NULL) {
      QString str=QString(line).trimmed();
      if(str.left(7)=="static ") {
	QStringList f0=str.right(str.length()-7).split("=");
	if(f0.size()==2) {
	  if(f0.at(0)=="ip_address") {
	    set_dhcp_box->setCurrentIndex(1);
	    QStringList f1=f0.at(1).split("/");
	    if(f1.size()==2) {
	      QHostAddress addr(f1.at(0));
	      if(!addr.isNull()) {
		unsigned mask=f1.at(1).toUInt(&ok);
		if(ok&&(mask<=32)) {
		  set_ipaddress_edit->setText(addr.toString());
		  set_ipnetmask_edit->setText(NetmaskText(mask).toString());
		}
	      }
	    }
	  }
	  if(f0.at(0)=="routers") {
	    QHostAddress addr(f0.at(1));
	    if(!addr.isNull()) {
	      set_ipgateway_edit->setText(addr.toString());
	    }
	  }
	  if(f0.at(0)=="domain_name_servers") {
	    QStringList f1=f0.at(1).split(" ");
	    for(int i=0;i<2;i++) {
	      if(i<f1.size()) {
		QHostAddress addr(f1.at(i));
		if(!addr.isNull()) {
		  set_dns_edits[i]->setText(addr.toString());
		}
	      }
	    }
	  }
	}
      }
      else {
	if(str.left(9)!="interface") {
	  set_dhcpcd_values.push_back(str);
	}
      }
    }
    fclose(f);
  }
#endif  // DEBIAN

#ifdef DESKTOP
  set_dhcp_box->setCurrentIndex(0);
#endif  // DESKTOP

  set_rpiconfig->load();
  set_resolution_box->setCurrentItemData(set_rpiconfig->framebufferSize());
  dhcpChangedData(set_dhcp_box->currentIndex());
}


bool SettingsDialog::Save()
{
  QList<QString> dns_servers;
  FILE *f=NULL;
  QProcess *proc=NULL;
  QStringList args;

  //
  // Sanity Check
  //
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
    if(!set_dns_edits[0]->text().isEmpty()) {
      if(!ValidIp(set_dns_edits[0]->text())) {
	QMessageBox::information(this,tr("VPick - Error"),
				 tr("Invalid DNS Server value!"));
	return false;
      }
      dns_servers.push_back(set_dns_edits[0]->text());
    }
    if(!set_dns_edits[1]->text().isEmpty()) {
      if(!ValidIp(set_dns_edits[1]->text())) {
	QMessageBox::information(this,tr("VPick - Error"),
				 tr("Invalid DNS Server value!"));
	return false;
      }
      dns_servers.push_back(set_dns_edits[1]->text());
    }
  }

  //
  // Pointer Settings
  //
  set_config->setPointerHandedness((Config::Handedness)
				   set_handedness_box->currentData().toUInt());

  //
  // Network Settings
  //
#ifdef REDHAT
  if(set_dhcp_box->currentIndex()==0) {  // DHCP
    //
    // Enable DHCP
    //
    args.clear();
    args.push_back("con");
    args.push_back("mod");
    args.push_back(VPICK_NETWORK_INTERFACE);
    args.push_back("ipv4.method");
    args.push_back("auto");
    proc=new QProcess(this);
    proc->start("/bin/nmcli",args);
    proc->waitForFinished();
    delete proc;
  }
  else {  // Manual Setup
    //
    // Disable DHCP
    //
    args.clear();
    args.push_back("con");
    args.push_back("mod");
    args.push_back(VPICK_NETWORK_INTERFACE);
    args.push_back("ipv4.method");
    args.push_back("manual");
    proc=new QProcess(this);
    proc->start("/bin/nmcli",args);
    proc->waitForFinished();
    delete proc;

    //
    // IP Address / Netmask
    //
    QHostAddress netmask(set_ipnetmask_edit->text());
    unsigned masksize=0;
    for(unsigned i=0;i<32;i++) {
      if((netmask.toIPv4Address()&(1<<i))!=0) {
	masksize++;
      }
    }
    args.clear();
    args.push_back("con");
    args.push_back("mod");
    args.push_back(VPICK_NETWORK_INTERFACE);
    args.push_back("ipv4.addresses");
    args.push_back(set_ipaddress_edit->text()+
		   QString::asprintf("/%u",masksize));
    proc=new QProcess(this);
    proc->start("/bin/nmcli",args);
    proc->waitForFinished();
    delete proc;

    //
    // Default Gateway
    //
    args.clear();
    args.push_back("con");
    args.push_back("mod");
    args.push_back(VPICK_NETWORK_INTERFACE);
    args.push_back("ipv4.gateway");
    args.push_back(set_ipgateway_edit->text());
    proc=new QProcess(this);
    proc->start("/bin/nmcli",args);
    proc->waitForFinished();
    delete proc;

    //
    // DNS
    //
    bool used=false;
    for(int i=0;i<2;i++) {
      if(!set_dns_edits[i]->text().isEmpty()) {
	args.clear();
	args.push_back("con");
	args.push_back("mod");
	args.push_back(VPICK_NETWORK_INTERFACE);
	if(used) {
	  args.push_back("+ipv4.dns");
	}
	else {
	  args.push_back("ipv4.dns");
	}
	args.push_back(set_dns_edits[i]->text());
	proc=new QProcess(this);
	proc->start("/bin/nmcli",args);
	proc->waitForFinished();
	delete proc;
	used=true;
      }
    }

    //
    // Activate
    //
    args.clear();
    args.push_back("con");
    args.push_back("up");
    args.push_back(VPICK_NETWORK_INTERFACE);
    proc=new QProcess(this);
    proc->start("/bin/nmcli",args);
    proc->waitForFinished();
    delete proc;

    //
    // Make Persistent
    //
    args.clear();
    args.push_back("con");
    args.push_back("mod");
    args.push_back(VPICK_NETWORK_INTERFACE);
    args.push_back("autoconnect");
    args.push_back("yes");
    proc=new QProcess(this);
    proc->start("/bin/nmcli",args);
    proc->waitForFinished();
    delete proc;
  }
#endif  // REDHAT

#ifdef DEBIAN
  if((f=fopen("/etc/dhcpcd.conf-TEMP","w"))!=NULL) {
    for(int i=0;i<set_dhcpcd_values.size();i++) {
      fprintf(f,"%s\n",set_dhcpcd_values.at(i).toUtf8().constData());
    }
    if(set_dhcp_box->currentIndex()>0) {
      fprintf(f,"interface %s\n",VPICK_NETWORK_INTERFACE.toUtf8().constData());
      fprintf(f,"static ip_address=%s/%d\n",
	      set_ipaddress_edit->text().toUtf8().constData(),
	      NetmaskValue(QHostAddress(set_ipnetmask_edit->text())));
      fprintf(f,"static routers=%s\n",
	      set_ipgateway_edit->text().toUtf8().constData());
      fprintf(f,"static domain_name_servers=");
      for(int i=0;i<2;i++) {
	QHostAddress addr(set_dns_edits[i]->text());
	if(!addr.isNull()) {
	  fprintf(f,"%s ",addr.toString().toUtf8().constData());
	}
      }
      fprintf(f,"\n");
    }
    fclose(f);
    rename("/etc/dhcpcd.conf-TEMP","/etc/dhcpcd.conf");

    args.clear();
    args.push_back("restart");
    args.push_back("dhcpcd.service");
    proc=new QProcess(this);
    proc->start("/bin/systemctl",args);
    proc->waitForFinished();
    delete proc;
  }  
#endif  // DEBIAN

  //
  // Firmware Settings
  //
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
