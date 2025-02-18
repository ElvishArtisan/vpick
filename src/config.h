// config.h
//
// vpick(1) Host Chooser Configuration
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

#ifndef CONFIG_H
#define CONFIG_H

#include <QColor>
#include <QList>
#include <QHostAddress>
#include <QPoint>
#include <QProcess>
#include <QScreen>
#include <QSize>
#include <QString>

#define VPICK_NETWORK_INTERFACE QString("eth0")
#ifdef EMBEDDED
#define VPICK_CONF_FILE QString("/etc/vpick.conf")
#endif  // EMBEDDED

#define VPICK_SYNERGY_CONF_FILE QString("/etc/synergy.conf")
#define VPICK_BUTTON_WIDTH 200
#define VPICK_BUTTON_HEIGHT 40
#define VPICK_BUTTON_MARGIN 10
#define VPICK_BUTTON_DESKTOP_FUDGE 50
#define VPICK_MIMETYPE "application/vpick-button"
class Config
{
 public:
  enum Type {VncPlain=0,Spice=1,LastType=2};
  enum SynergyMode {NoSynergy=0,ClientSynergy=1,ServerSynergy=2};
  enum Handedness {RightHanded=1,LeftHanded=2};
  Config(int screen_num);
  int screenNumber() const;
  QScreen *screen();
  QSize screenSize();
  QSize canvasSize() const;
  Handedness pointerHandedness() const;
  void setPointerHandedness(Handedness hand);
  SynergyMode synergyMode() const;
  void setSynergyMode(SynergyMode mode);
  QString synergyScreenname() const;
  void setSynergyScreenname(const QString &str);
  QHostAddress synergyServerAddress() const;
  void setSynergyServerAddress(const QHostAddress &addr);
  int hostQuantity() const;
  QPoint position(int n) const;
  void setPosition(int n,const QPoint &pos);
  void setPosition(int n,int x,int y);
  QPoint windowPosition(int n) const;
  void setWindowPosition(int n,const QPoint &win_pos);
  Type type(int n);
  void setType(int n,Type type);
  QString title(int n) const;
  void setTitle(int n,const QString &str);
  QString hostname(int n) const;
  void setHostname(int n,const QString &str);
  QString password(int n) const;
  void setPassword(int n,const QString &);
  bool autoconnect(int n) const;
  void setAutoconnect(int n,bool state);
  bool fullscreen(int n) const;
  void setFullscreen(int n,bool state);
  QColor color(int n) const;
  void setColor(int n,const QColor &color);
  QString liveWindowId(int n) const;
  QRect liveWindowGeometry(int n) const;
  void updateLiveParameters(int n);
  int addHost(Type type,const QString &title,const QString &hostname,
	      const QString &passwd,bool autoconnect,bool fullscreen,
	      const QColor &color);
  void removeHost(int n);
  bool positionIsFree(const QPoint &pt) const;
  QPoint nextFreePosition(bool *ok=NULL);
  bool hasFreePosition();
  bool load();
  bool save();
  bool fixup();
  static QString hostName();
  static QString typeString(Type type);

 private:
  int conf_screen_number;
  QSize conf_canvas_size;
  Handedness conf_handedness;
  SynergyMode conf_synergy_mode;
  QString conf_synergy_screenname;
  QHostAddress conf_synergy_server_address;
  QList<Type> conf_types;
  QList<QPoint> conf_positions;
  QList<QPoint> conf_window_positions;
  QList<QString> conf_titles;
  QList<QString> conf_hostnames;
  QList<QString> conf_passwords;
  QList<unsigned> conf_autoconnects;
  QList<unsigned> conf_fullscreens;
  QList<QColor> conf_colors;
  QList<QProcess *> conf_viewer_processes;
  QStringList conf_startup_file_names;
  QStringList conf_live_window_ids;
  QList<QRect> conf_live_window_geometries;
#ifdef DESKTOP
  QString conf_filename;
#endif  // DESKTOP
};


#endif  // CONFIG_H
