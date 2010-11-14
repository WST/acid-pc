#ifndef TRAYICON_H
#define TRAYICON_H

#include <QtGui>

class TrayIcon: public QSystemTrayIcon
{
	Q_OBJECT
	public:
		TrayIcon();
		void debugMessage(QString msg);
		void setOnline();
		void setOffline();
};

#endif
