#ifndef TRAYICON_H
#define TRAYICON_H

#include <QtGui>

#include "notificationwidget.h"

class TrayIcon: public QSystemTrayIcon
{
	Q_OBJECT
	public:
		TrayIcon();
		void debugMessage(QString msg);
		void notify(const QString &title, const QString &message);
		void popup(QWidget *message, unsigned short int timeout = 5);
		void setOnline();
		void setOffline();
};

#endif
