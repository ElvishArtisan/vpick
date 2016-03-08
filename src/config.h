// config.h
//
// vpick(1) Host Chooser Configuration
//
//   (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
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

#include <QString>

#define VPICK_CONF_FILE "/etc/vpick.conf"

class Config
{
 public:
  enum Type {VncPlain=0};
  Config();
  unsigned hostQuantity() const;
  Type type(unsigned n);
  void setType(unsigned n,Type type);
  QString title(unsigned n) const;
  void setTitle(unsigned n,const QString &str);
  QString hostname(unsigned n) const;
  void setHostname(unsigned n,const QString &str);
  QString password(unsigned n) const;
  void setPassword(unsigned n,const QString &);
  unsigned addHost(Type type,const QString &title,const QString &hostname,
		   const QString &passwd);
  void removeHost(unsigned n);
  bool load();
  bool save();

 private:
  std::vector<Type> conf_types;
  std::vector<QString> conf_titles;
  std::vector<QString> conf_hostnames;
  std::vector<QString> conf_passwords;
};


#endif  // CONFIG_H
