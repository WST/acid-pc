#ifndef MESSENGER_H
#define MESSENGER_H

// Qt
#include <QObject>
#include <QSettings>

// qxmpp
#include <QXmppClient.h>

// ACId
#include "mainwindow.h"
#include "loginform.h"
#include "trayicon.h"
#include "messageform.h"

class Messenger: public QObject {
	Q_OBJECT
	private:
		QXmppClient *client; // клиент
		QXmppConfiguration *client_settings; // настройки клиента
		QSettings *settings; // запоминалка настроек
		MainWindow *window; // окошко с ростером
		LoginForm *login; // окошко ввода информации об учётной записи
		TrayIcon *tray; // значок приложения в трее
		void createConnections();
		void createMenus();

	public:
		Messenger(QObject *parent = 0);
		~Messenger();
		void launch();

	private slots:
		void activate();
		void handleSuccessfulConnection();
		void handleConnectionError(QXmppClient::Error error);
		void handleDisconnection();
		void disconnect();
		void iconClicked(QSystemTrayIcon::ActivationReason reason);
		void createNewMessage();
		void sendMessage(MessageForm *);
};

#endif // MESSENGER_H
