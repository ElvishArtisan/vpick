// config.h
//
// vpick(1) Host Chooser Configuration
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

#ifndef CONFIG_H
#define CONFIG_H

#include <vector>

#include <QColor>
#include <QHostAddress>
#include <QString>

#define VPICK_NETWORK_INTERFACE QString("eth0")
#ifdef EMBEDDED
#define VPICK_CONF_FILE QString("/etc/vpick.conf")
#endif  // EMBEDDED

#define VPICK_SYNERGY_CONF_FILE QString("/etc/synergy.conf")

class Config
{
 public:
  enum Type {VncPlain=0,Spice=1,LastType=2};
  enum SynergyMode {NoSynergy=0,ClientSynergy=1,ServerSynergy=2};
  Config();
  SynergyMode synergyMode() const;
  void setSynergyMode(SynergyMode mode);
  QString synergyScreenname() const;
  void setSynergyScreenname(const QString &str);
  QHostAddress synergyServerAddress() const;
  void setSynergyServerAddress(const QHostAddress &addr);
  unsigned hostQuantity() const;
  Type type(unsigned n);
  void setType(unsigned n,Type type);
  QString title(unsigned n) const;
  void setTitle(unsigned n,const QString &str);
  QString hostname(unsigned n) const;
  void setHostname(unsigned n,const QString &str);
  QString password(unsigned n) const;
  void setPassword(unsigned n,const QString &);
  bool autoconnect(unsigned n) const;
  void setAutoconnect(unsigned n,bool state);
  Qt::GlobalColor color(unsigned n) const;
  void setColor(unsigned n,Qt::GlobalColor color);
  unsigned addHost(Type type,const QString &title,const QString &hostname,
		   const QString &passwd,bool autoconnect,
		   Qt::GlobalColor color);
  void removeHost(unsigned n);
  bool load();
  bool save();
  static QString hostName();
  static QString typeString(Type type);

 private:
  SynergyMode conf_synergy_mode;
  QString conf_synergy_screenname;
  QHostAddress conf_synergy_server_address;
  std::vector<Type> conf_types;
  std::vector<QString> conf_titles;
  std::vector<QString> conf_hostnames;
  std::vector<QString> conf_passwords;
  std::vector<unsigned> conf_autoconnects;
  std::vector<Qt::GlobalColor> conf_colors;
#ifdef DESKTOP
  QString conf_filename;
#endif  // DESKTOP
};


#endif  // CONFIG_H
