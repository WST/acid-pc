
#include "trayicon.h"

#ifdef WIN32
#define TRAYICON_OFFLINE QIcon(":/trayicon/offline-16px.png")
#define TRAYICON_ONLINE QIcon(":/trayicon/online-16px.png")
#define TRAYICON_AWAY QIcon(":/trayicon/away-16px.png")
#define TRAYICON_XA QIcon(":/trayicon/xa-16px.png")
#define TRAYICON_DND QIcon(":/trayicon/dnd-16px.png")
#define TRAYICON_F4C QIcon(":/trayicon/f4c-16px.png")
#else
#define TRAYICON_OFFLINE QIcon(":/trayicon/offline-22px.png")
#define TRAYICON_ONLINE QIcon(":/trayicon/online-22px.png")
#define TRAYICON_AWAY QIcon(":/trayicon/away-22px.png")
#define TRAYICON_XA QIcon(":/trayicon/xa-22px.png")
#define TRAYICON_DND QIcon(":/trayicon/dnd-22px.png")
#define TRAYICON_F4C QIcon(":/trayicon/f4c-22px.png")
#endif

TrayIcon::TrayIcon(QWidget *parent): QSystemTrayIcon(TRAYICON_OFFLINE, parent) {
	setVisible(true);
}

void TrayIcon::debugMessage(QString msg) {
	showMessage("Debug message", msg);
}

void TrayIcon::notify(const QString &title, const QString &message) {
	showMessage(title, message);
}

void TrayIcon::setStatus(QXmppPresence::Status::Type type) {
	switch(type) {
		case QXmppPresence::Status::Away: setIcon(TRAYICON_AWAY); break;
		//case 0: setIcon(TRAYICON_OFFLINE); break;
		case QXmppPresence::Status::Online: setIcon(TRAYICON_ONLINE); break;
		case QXmppPresence::Status::DND: setIcon(TRAYICON_DND); break;
		case QXmppPresence::Status::XA: setIcon(TRAYICON_XA); break;
		case QXmppPresence::Status::Chat: setIcon(TRAYICON_F4C); break;
	}
}
