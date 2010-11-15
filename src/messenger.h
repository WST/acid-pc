#ifndef MESSENGER_H
#define MESSENGER_H

// Qt
#include <QObject>
#include <QSettings>

// qxmpp
#include <QXmppClient.h>
#include <QXmppTransferManager.h>
#include <QXmppCallManager.h>
#include <QXmppMessage.h>

// ACId
#include "mainwindow.h"
#include "loginform.h"
#include "trayicon.h"
#include "messageform.h"
#include "chatwindow.h"
#include "aboutwindow.h"

class Messenger: public QObject {
	Q_OBJECT
	private:
		QXmppClient *client; // клиент
		QXmppConfiguration *client_settings; // настройки клиента
		QSettings *settings; // запоминалка настроек
		MainWindow *window; // окошко с ростером
		LoginForm *login; // окошко ввода информации об учётной записи
		TrayIcon *tray; // значок приложения в трее
		ChatWindow *chat; // окошко чатов
		AboutWindow *about;
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
		void sendMessage(MessageForm *message);
		void gotVoiceCall(QXmppCall *call);
		void gotIQ(QXmppIq message);
		void gotMessage(QXmppMessage message);
		void setPlastiqueStyle();
		void setCleanlooksStyle();
		void joinSupportRoom();
		void showApplicationInfo();
};

#endif // MESSENGER_H
