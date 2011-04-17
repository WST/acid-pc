
#include "trayicon.h"

#ifdef WIN32
#define TRAYICON_OFFLINE QIcon(":/trayicon/offline-16px.png")
#define TRAYICON_ONLINE QIcon(":/trayicon/online-16px.png")
#else
#define TRAYICON_OFFLINE QIcon(":/trayicon/offline-22px.png")
#define TRAYICON_ONLINE QIcon(":/trayicon/online-22px.png")
#endif

TrayIcon::TrayIcon(): QSystemTrayIcon(TRAYICON_OFFLINE) {
	setVisible(true);
}

void TrayIcon::debugMessage(QString msg) {
	showMessage("Debug message", msg);
}

void TrayIcon::notify(const QString &title, const QString &message) {
	showMessage(title, message);
}

void TrayIcon::popup(QWidget *message, unsigned short int timeout) {
	NotificationWidget *widget = new NotificationWidget(0, timeout);
	widget->showMessage(QPixmap(), message, true);
	connect(widget, SIGNAL(hidden()), widget, SLOT(deleteLater()));
	connect(widget, SIGNAL(hidden()), message, SLOT(deleteLater()));
}

void TrayIcon::setOnline() {
	setIcon(TRAYICON_ONLINE);
}

void TrayIcon::setOffline() {
	setIcon(TRAYICON_OFFLINE);
}
