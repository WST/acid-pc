#ifndef MESSENGER_H
#define MESSENGER_H

// Qt
#include <QtGui>

// qxmpp
#include <QXmppClient.h>
#include <QXmppTransferManager.h>
#include <QXmppCallManager.h>
#include <QXmppMucManager.h>
#include <QXmppVCardManager.h>
#include <QXmppVersionManager.h>
#include <QXmppDiscoveryManager.h>
#include <QXmppMessage.h>

// ACId
#include "loginform.h"
#include "trayicon.h"
#include "messageform.h"
#include "chatwindow.h"
#include "settingswindow.h"
#include "aboutwindow.h"
#include "vcardwindow.h"
#include "voicecallwindow.h"
#include "contact_list/tree_view.h"
#include "contact_list/item_model.h"
#include "contact_list/item_delegate.h"

class Messenger;
class ChatWindow;
class SettingsWindow;

class Messenger: public QMainWindow {
	Q_OBJECT
	private:
		QXmppClient *client; // клиент
		QXmppConfiguration *client_settings; // настройки клиента
		QXmppPresence *client_presence; // присутствие клиента
		QSettings *settings; // запоминалка настроек
		LoginForm *login; // окошко ввода информации об учётной записи
		TrayIcon *tray; // значок приложения в трее
		ChatWindow *chat; // окошко чатов
		SettingsWindow *settings_window; // редактор настроек
		VoiceCallWindow *call_window; // Окошко голосового вызова
		AboutWindow *about;
		void createConnections();
		void createMenus();
		void saveSettings();
		QXmppCallManager *call_manager;
		QXmppMucManager *muc_manager;
		QXmppTransferManager *transfer_manager;
		QXmppVersionManager *version_manager;
		QXmppVCardManager *vcard_manager;
		QXmppDiscoveryManager *disco_manager;
	
		CL::TreeView roster_widget; // виджет ростера
		CL::ItemModel roster_model; // модель элемента ростера

		QMap<QString, QXmppMessage> messages; // буфер принятых сообщений
		QMap<QString, QXmppMucRoom *> rooms; // комнаты, в которые зашёл клиент

	public:
		Messenger(QWidget *parent = 0);
		~Messenger();
		void launch();
		QSettings *settingsManager();

	private slots:
		void activate();
		void manageSettings();
		void handleSuccessfulConnection();
		void handleConnectionError(QXmppClient::Error error);
		void handleDisconnection();
		void disconnect();
		void iconClicked(QSystemTrayIcon::ActivationReason reason);
		void createNewMessage();
		void joinNewRoom();
		void sendMessage(MessageForm *message); // слот с аргументами to, message уже есть в клиенте (qxmpp)
		void sendMUCMessage(QString, QString);
		void gotVoiceCall(QXmppCall *call);
		void gotIQ(QXmppIq message);
		void gotMessage(QXmppMessage message);
		void gotFile(QXmppTransferJob *job);
		void fileTransferFinished(QXmppTransferJob *job);
		void joinSupportRoom();
		void showApplicationInfo();
		void presenceChanged(const QString &bare_jid, const QString &resource);
		void rosterReceived();
		void rosterChanged(const QString& bare_jid);
		void openChat(const QString &full_jid);
		void joinRoom(const QString &room_jid, const QString &nick); // в qxmpp ето есть, но этот метод ещё и открывает таб
		void leaveRoom(const QString &room_jid);
		void showProfile(const QXmppVCardIq &whose);
		void loadSettings();
		void roomParticipantChanged(QString room_jid, QString nick);
		void requestProfile(const QString &bare_jid);
		void makeVoiceCall(const QString &bare_jid);
		void openOfficialSite();
		void endCall();
		void confirmedMessage(const QString &message_id);
		void confirmedFile(QXmppTransferJob *job, bool confirmed);
		void confirmedCall(QXmppCall *call, bool confirmed);
		void setOnlineStatus();
		void setAwayStatus();
		void setXAStatus();
		void setF4CStatus();
		void setDNDStatus();
};

#endif // MESSENGER_H
