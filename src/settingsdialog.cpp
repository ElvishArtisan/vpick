// settingsdialog.cpp
//
// Configure system settings
//
//   (C) Copyright 2016-2025 Fred Gleason <fredg@paravelsystems.com>
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
#include <QMap>
#include <QMessageBox>
#include <QProcess>

#include "config.h"
#include "network.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(Config *c,QWidget *parent)
  : QDialog(parent)
{
  setWindowTitle(tr("Host Picker - Settings"));

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
#ifdef EMBEDDED
  set_rpiconfig=new RpiConfig();
#endif  // EMBEDDED
  
  //
  // Dialogs
  //
  set_layout_dialog=new LayoutDialog(set_config,this);
#ifdef EMBEDDED
  set_synergy_dialog=new SynergyDialog(set_config,this);
#endif  // EMBEDDED

#ifdef DESKTOP
  //
  // Viewer Button Mode
  //
  set_viewer_button_mode_label=new QLabel(tr("Viewer Button Mode")+":",this);
  set_viewer_button_mode_label->setFont(label_font);
  set_viewer_button_mode_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  set_viewer_button_mode_box=new ComboBox(this);
  set_viewer_button_mode_box->
    insertItem(0,tr("Raise Viewer"),Config::ButtonRaises);
  set_viewer_button_mode_box->
    insertItem(0,tr("Toggle Viewer"),Config::ButtonToggles);
#endif  // DESKTOP


#ifdef EMBEDDED
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
#endif  // EMBEDDED

  //
  // Layout Button
  //
  set_layout_button=
    new QPushButton(tr("Configure")+"\n"+tr("Layout"),this);
  set_layout_button->setFont(small_button_font);
  connect(set_layout_button,SIGNAL(clicked()),set_layout_dialog,SLOT(exec()));

  #ifdef EMBEDDED
  //
  // Synergy Button
  //
  set_synergy_button=
    new QPushButton(tr("Configure")+"\n"+tr("Synergy"),this);
  set_synergy_button->setFont(small_button_font);
  connect(set_synergy_button,SIGNAL(clicked()),set_synergy_dialog,SLOT(exec()));
#endif  // EMBEDDED

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
}


QSize SettingsDialog::sizeHint() const
{
#ifdef EMBEDDED
  return QSize(490,299);
#endif  // EMBEDDED
#ifdef DESKTOP
  return QSize(350,120);
#endif  // DESKTOP
}


void SettingsDialog::stopSynergy()
{
#ifdef EMBEDDED
  set_synergy_dialog->stopSynergy();
#endif  // EMBEDDED
}


int SettingsDialog::exec()
{
  Load();
  return QDialog::exec();
}


void SettingsDialog::dhcpChangedData(int n)
{
#ifdef EMBEDDED
  set_ipaddress_label->setEnabled(n);
  set_ipaddress_edit->setEnabled(n);
  set_ipnetmask_label->setEnabled(n);
  set_ipnetmask_edit->setEnabled(n);
  set_ipgateway_label->setEnabled(n);
  set_ipgateway_edit->setEnabled(n);
  set_dns_label->setEnabled(n);
  set_dns_edits[0]->setEnabled(n);
  set_dns_edits[1]->setEnabled(n);
#endif  // EMBEDDED
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
#ifdef EMBEDDED
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
#endif  // EMBEDDED

#ifdef DESKTOP
  set_viewer_button_mode_label->setGeometry(10,2,175,20);
  set_viewer_button_mode_box->setGeometry(190,2,size().width()-200,20);
#endif  // DESKTOP  
  set_layout_button->setGeometry(10,size().height()-60,80,50);

  set_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  set_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void SettingsDialog::Load()
{
#ifdef EMBEDDED
  FILE *f=NULL;
  char line[1024];
  QMap<QString,QString> confvalues;

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
      confvalues[hdr]=f0.join("=");
    }
    fclose(f);
  }
  set_dhcp_box->setCurrentIndex(confvalues["BOOTPROTO"].toLower()!="dhcp");
  set_ipaddress_edit->setText(confvalues["IPADDR"]);
  set_ipnetmask_edit->setText(confvalues["NETMASK"]);
  set_ipgateway_edit->setText(confvalues["GATEWAY"]);
  set_dns_edits[0]->setText(confvalues["DNS1"]);
  set_dns_edits[1]->setText(confvalues["DNS2"]);
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
  set_rpiconfig->load();
  set_resolution_box->setCurrentItemData(set_rpiconfig->framebufferSize());
  dhcpChangedData(set_dhcp_box->currentIndex());
#endif  // EMBEDDED

#ifdef DESKTOP
  set_viewer_button_mode_box->
    setCurrentItemData((int)set_config->viewerButtonMode());
#endif  // DESKTOP
}


bool SettingsDialog::Save()
{
#ifdef EMBEDDED
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
    if((!ValidIp(set_ipgateway_edit->text()))&&
       (!set_ipgateway_edit->text().trimmed().isEmpty())) {
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
  QString connfile=QString("/etc/sysconfig/network-scripts/ifcfg-")+
    VPICK_NETWORK_INTERFACE;
  QString tempfile=connfile+"-TEMP";
  if(set_dhcp_box->currentIndex()==0) {  // DHCP
    if((f=fopen(tempfile.toUtf8(),"w"))!=NULL) {
      fprintf(f,"AUTOCONNECT_PRIORITY=-999\n");
      fprintf(f,"BOOTPROTO=dhcp\n");
      fprintf(f,"BROWSER_ONLY=no\n");
      fprintf(f,"DEFROUTE=yes\n");
      fprintf(f,"DEVICE=%s\n",VPICK_NETWORK_INTERFACE.toUtf8().constData());
      fprintf(f,"NAME=%s\n",VPICK_NETWORK_INTERFACE.toUtf8().constData());
      fprintf(f,"ONBOOT=yes\n");
      fprintf(f,"TYPE=Ethernet\n");
      fprintf(f,"IPV4_FAILURE_FATAL=no\n");
      fprintf(f,"IPV6INIT=yes\n");
      fprintf(f,"IPV6_ADDR_GEN_MODE=eui64\n");
      fprintf(f,"IPV6_AUTOCONF=yes\n");
      fprintf(f,"IPV6_DEFROUTE=yes\n");
      fprintf(f,"IPV6_FAILURE_FATAL=no\n");
      fprintf(f,"PROXY_METHOD=no\n");
      fclose(f);
      rename(tempfile.toUtf8(),connfile.toUtf8());
    }
  }
  else {  // Static
    if((f=fopen(tempfile.toUtf8(),"w"))!=NULL) {
      fprintf(f,"AUTOCONNECT_PRIORITY=-999\n");
      fprintf(f,"BOOTPROTO=none\n");
      fprintf(f,"BROWSER_ONLY=no\n");
      if(set_ipgateway_edit->text().trimmed().isEmpty()) {
	fprintf(f,"DEFROUTE=no\n");
      }
      else {
	fprintf(f,"DEFROUTE=yes\n");
	fprintf(f,"GATEWAY=%s\n",
		set_ipgateway_edit->text().trimmed().toUtf8().constData());
      }
      fprintf(f,"DEVICE=%s\n",VPICK_NETWORK_INTERFACE.toUtf8().constData());
      fprintf(f,"IPADDR=%s\n",
	      set_ipaddress_edit->text().trimmed().toUtf8().constData());
      fprintf(f,"IPV4_FAILURE_FATAL=no\n");
      fprintf(f,"IPV6INIT=yes\n");
      fprintf(f,"IPV6_ADDR_GEN_MODE=eui64\n");
      fprintf(f,"IPV6_AUTOCONF=yes\n");
      fprintf(f,"IPV6_DEFROUTE=yes\n");
      fprintf(f,"IPV6_FAILURE_FATAL=no\n");
      fprintf(f,"NAME=%s\n",VPICK_NETWORK_INTERFACE.toUtf8().constData());
      fprintf(f,"NETMASK=%s\n",
	      set_ipnetmask_edit->text().toUtf8().constData());
      fprintf(f,"ONBOOT=yes\n");
      fprintf(f,"PROXY_METHOD=none\n");
      fprintf(f,"TYPE=Ethernet\n");
      for(int i=0;i<2;i++) {
	if(!set_dns_edits[i]->text().trimmed().isEmpty()) {
	  fprintf(f,"DNS%d=%s\n",i+1,
		  set_dns_edits[i]->text().trimmed().toUtf8().constData());
	}
      }
      fclose(f);
      if(rename(tempfile.toUtf8(),connfile.toUtf8())!=0) {
	QMessageBox::critical(this,tr("System Error"),
			      tr("Unable to finalize network configuration!")+"\n"+
			      strerror(errno));
      }
    }
    else {
      QMessageBox::critical(this,tr("System Error"),
			    tr("Unable to save network configuration!")+"\n"+
			    strerror(errno));
    }		    
  }
  
  //
  // Activate Changes
  //
  args.clear();
  args.push_back("con");
  args.push_back("load");
  args.push_back(connfile);
  proc=new QProcess(this);
  proc->start("/bin/nmcli",args);
  proc->waitForFinished();
  delete proc;

  args.clear();
  args.push_back("con");
  args.push_back("up");
  args.push_back(VPICK_NETWORK_INTERFACE);
  proc=new QProcess(this);
  proc->start("/bin/nmcli",args);
  proc->waitForFinished();
  delete proc;
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
    system("/sbin/reboot");
  }
#endif  // EMBEDDED

#ifdef DESKTOP
  set_config->setViewerButtonMode((Config::ViewerButtonMode)
	      set_viewer_button_mode_box->currentItemData().toInt());
#endif  // DESKTOP
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
