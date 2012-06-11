#ifndef MESSENGER_H
#define MESSENGER_H

// Qt
#include <QtGui>

// qxmpp
#include <qxmpp/QXmppClient.h>
#include <qxmpp/QXmppTransferManager.h>
#include <qxmpp/QXmppCallManager.h>
#include <qxmpp/QXmppMucManager.h>
#include <qxmpp/QXmppVCardManager.h>
#include <qxmpp/QXmppVersionManager.h>
#include <qxmpp/QXmppBookmarkManager.h>
#include <qxmpp/QXmppDiscoveryManager.h>
#include <qxmpp/QXmppMessage.h>

// ACId
#include "loginform.h"
#include "trayicon.h"
#include "messageform.h"
#include "chatwindow.h"
#include "settingswindow.h"
#include "aboutwindow.h"
#include "vcardwindow.h"
#include "voicecallwindow.h"
#include "contact_list/qxmpp_bridge.h"
#include "contact_list/tree_view.h"
#include "contact_list/item_model.h"
#include "contact_list/item_delegate.h"
#include "contact_list/contact_item.h"

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
        QXmppBookmarkManager *bookmark_manager;
        QMenu *room_bookmarks_menu; // меню с закладками на MUC
        QAction *action_edit_bookmarks;

		CL::QXmppBridge *rosterBridge; // мост между QXmpp и CL ростером
		CL::TreeView roster_widget; // виджет ростера
		CL::ItemModel roster_model; // модель элемента ростера

        QMap<QString, QMap<QString, QXmppMessage> > messages; // буфер принятых сообщений
        QTranslator *translator; // Транслятор UI приложения

	public:
        Messenger(QTranslator *app_translator, QSettings *app_settings);
		~Messenger();
		void launch();
		QSettings *settingsManager();
        QXmppMucRoom *getRoomByJid(const QString &jid);
        unsigned long int counter;
        QString id();

	private slots:
		void activate();
		void manageSettings();
		void handleSuccessfulConnection();
		void handleConnectionError(QXmppClient::Error error);
		void handleDisconnection();
		void disconnect();
		void iconClicked(QSystemTrayIcon::ActivationReason reason);
		void createNewMessage();
        void showNewContactWindow();
        void joinNewRoom();
        void rejoinRoom(QString error_message, QString room_jid);
		void sendMessage(MessageForm *message); // слот с аргументами to, message уже есть в клиенте (qxmpp)
		void sendMUCMessage(QString, QString);
		void gotVoiceCall(QXmppCall *call);
		void gotIQ(QXmppIq message);
		void gotMessage(QXmppMessage message);
		void gotFile(QXmppTransferJob *job);
		void fileTransferFinished(QXmppTransferJob *job);
		void joinSupportRoom();
		void showApplicationInfo();
        void openChat(const CL::ContactItem *item);
		void joinRoom(const QString &room_jid, const QString &nick); // в qxmpp ето есть, но этот метод ещё и открывает таб
        void joinedRoom();
        void leftRoom();
        void kickedFromRoom(const QString &jid, const QString &reason);
        void processJoinRequest(const QString &room_jid);
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
        void addNewContact(const QString &jid, const QString &nick);
        void removeContact(const QString &jid);
        void openServiceBrowser();
        void inBandRegister(bool);
        void handleSubscriptionRequest(const QString &jid);
        void answerSubscriptionRequest(const QString &jid, bool accepted);
        void handleBookmarks(const QXmppBookmarkSet &bookmarks);
        void processBookmarkClick();
        void showBookmarkManager();
        void showSendFileDialog(const QString &jid);
};

#endif // MESSENGER_H
