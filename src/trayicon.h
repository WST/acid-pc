#ifndef TRAYICON_H
#define TRAYICON_H

#include <QtWidgets>

#include <qxmpp/QXmppPresence.h>

class TrayIcon: public QSystemTrayIcon
{
	Q_OBJECT
	public:
                TrayIcon(QWidget *parent = 0);
		void debugMessage(QString msg);
		void notify(const QString &title, const QString &message);
		void setStatus(QXmppPresence::Status::Type type);
};

#endif
