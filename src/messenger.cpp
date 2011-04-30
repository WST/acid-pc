// ACId
#include "messenger.h"
#include "functions.h"
#include <version.h>

Messenger::Messenger(QWidget *parent): QMainWindow(parent), roster_widget(this), roster_model(this), messages() {
	settings = new QSettings(APP_COMPANY, APP_NAME, this);
	client_settings = new QXmppConfiguration();
	client = new QXmppClient(this);

	transfer_manager = new QXmppTransferManager();
	call_manager = new QXmppCallManager();
	muc_manager = new QXmppMucManager();
	vcard_manager = new QXmppVCardManager();
	version_manager = new QXmppVersionManager();
	disco_manager = new QXmppDiscoveryManager();
	client->addExtension(call_manager);
	client->addExtension(muc_manager);
	client->addExtension(transfer_manager);
	client->addExtension(vcard_manager);
	client->addExtension(version_manager);
	client->addExtension(disco_manager);

	version_manager->setClientName(APP_NAME);
	version_manager->setClientVersion(APP_VERSION);

	disco_manager->setClientCategory("client");
	disco_manager->setClientType("pc");
	disco_manager->setClientName(APP_NAME);
	disco_manager->setClientCapabilitiesNode(APP_CAPSNODE);

	login = new LoginForm(APP_NAME);
	chat = new ChatWindow(this);
	about = new AboutWindow(this);
	settings_window = new SettingsWindow(this);
	tray = new TrayIcon(this);

	createConnections();
	createMenus();
	loadSettings();
	
	setWindowTitle(APP_NAME);
	setWindowIcon(QIcon(":/acid_16.png"));
	
	roster_widget.setModel(& roster_model);
	setCentralWidget(& roster_widget);
	
	roster_widget.setItemDelegate(new CL::ItemDelegate());
}

Messenger::~Messenger() {
    saveSettings();
	/*
    delete about;
    delete tray;
    delete chat;
    delete login;
    delete window;
	 delete call_manager;
	delete muc_manager;
    delete client;
    delete client_settings;
    delete settings;
	*/
}

void Messenger::saveSettings() {
	settings->setValue("main/geometry", this->saveGeometry());
	settings->setValue("login/geometry", login->saveGeometry());
	settings->setValue("chat/geometry", chat->saveGeometry());
	settings->setValue("about/geometry", about->saveGeometry());

	settings->sync();
}

void Messenger::loadSettings() {
	if(settings->contains("main/geometry")) this->restoreGeometry(settings->value("main/geometry").toByteArray());
	if(settings->contains("login/geometry")) login->restoreGeometry(settings->value("login/geometry").toByteArray());
	if(settings->contains("chat/geometry")) chat->restoreGeometry(settings->value("chat/geometry").toByteArray());
	if(settings->contains("about/geometry")) about->restoreGeometry(settings->value("about/geometry").toByteArray());

	if(settings->contains("login/password")) login->setPassword(settings->value("login/password").toString());
	if(settings->contains("login/username")) login->setUsername(settings->value("login/username").toString());
	if(settings->contains("login/domain")) login->setDomain(settings->value("login/domain").toString());
	if(settings->contains("login/auto")) login->setAutoLogin(settings->value("login/auto").toBool());

	if(settings->contains("settings/roster_opacity")) setWindowOpacity(settings->value("settings/roster_opacity").toFloat() / 100);
	if(settings->value("settings/roster_on_the_top", false).toBool()) setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

	if(settings->contains("settings/gui_style")) {
		const char *styles[] = {0, "plastique", "cleanlooks"};
		const char *active_style = styles[settings->value("settings/gui_style").toInt()];
		qApp->setStyle(active_style);
	}
}

void Messenger::createConnections() {
	// здесь соединять сигналы и слоты…
	connect(login, SIGNAL(finished()), this, SLOT(activate()));
	connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconClicked(QSystemTrayIcon::ActivationReason)));
	connect(chat, SIGNAL(aboutToSend(QString, QString)), client, SLOT(sendMessage(QString, QString)));
	connect(chat, SIGNAL(aboutToSendMUC(QString, QString)), this, SLOT(sendMUCMessage(QString, QString)));
	connect(chat, SIGNAL(mucTabClosed(QString)), this, SLOT(leaveRoom(QString)));

	// сигналы клиента
	connect(client, SIGNAL(connected()), this, SLOT(handleSuccessfulConnection()));
	connect(client, SIGNAL(disconnected()), this, SLOT(handleDisconnection()));
	connect(client, SIGNAL(error(QXmppClient::Error)), this, SLOT(handleConnectionError(QXmppClient::Error)));
	connect(client, SIGNAL(iqReceived(QXmppIq)), this, SLOT(gotIQ(QXmppIq)));
	connect(client, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(gotMessage(QXmppMessage)));
	
	connect(& client->rosterManager(), SIGNAL(rosterReceived()), this, SLOT(rosterReceived()));
	connect(& client->rosterManager(), SIGNAL(rosterChanged(const QString&)), this, SLOT(rosterChanged(const QString&)));
	connect(& client->rosterManager(), SIGNAL(presenceChanged(const QString&, const QString&)), this, SLOT(presenceChanged(const QString&, const QString&)));

	connect(call_manager, SIGNAL(callReceived(QXmppCall *)), this, SLOT(gotVoiceCall(QXmppCall *)));
	connect(muc_manager, SIGNAL(roomParticipantChanged(QString,QString)), this, SLOT(roomParticipantChanged(QString, QString)));
	// Следующая строка — реальный кошмар. Почему одни менеджеры надо вызывать напрямую, а этот через метод?
	connect(& client->vCardManager(), SIGNAL(vCardReceived(const QXmppVCardIq &)), this, SLOT(showProfile(const QXmppVCardIq &)));

	connect(this, SIGNAL(showChatDialog(QString)), this, SLOT(openChat(QString)));
	
	connect(& roster_widget, SIGNAL(showChatDialog(const QString &)), this, SLOT(openChat(const QString &)));
	connect(& roster_widget, SIGNAL(showProfile(const QString &)), this, SLOT(requestProfile(const QString &)));
	connect(& roster_widget, SIGNAL(removeContact(const QString &)), this, SLOT(removeContact(const QString &)));
	connect(& roster_widget, SIGNAL(makeVoiceCall(const QString &)), this, SLOT(makeVoiceCall(const QString &)));

	connect(settings_window, SIGNAL(modified()), this, SLOT(loadSettings()));
}

void Messenger::createMenus() {
	// TODO: расположить нормально, соответственно реальной структуре меню
	QMenu *im_menu = menuBar()->addMenu("Program");
		QAction *action_new_message = im_menu->addAction(QIcon(":/menu/document.png"), "New message...");
		im_menu->addSeparator();
		QAction *action_settings = im_menu->addAction(QIcon(":/menu/document.png"), "Settings");
		im_menu->addSeparator();
		QAction *action_quit = im_menu->addAction("Quit");
	
	QMenu *status_menu = menuBar()->addMenu("Status");
		QAction *action_status_available = status_menu->addAction(QIcon(":/trayicon/online-16px.png"), "Available");
		QAction *action_status_f4c = status_menu->addAction(QIcon(":/trayicon/online-16px.png"), "Free for chat");
		QAction *action_status_away = status_menu->addAction(QIcon(":/trayicon/online-16px.png"), "Away");
		QAction *action_status_xa = status_menu->addAction(QIcon(":/trayicon/online-16px.png"), "Extended away");
		QAction *action_status_dnd = status_menu->addAction(QIcon(":/trayicon/online-16px.png"), "Do not disturb");
		status_menu->addSeparator();
		QAction *action_status_dc = status_menu->addAction(QIcon(":/trayicon/offline-16px.png"), "Offline");
	
	QMenu *help_menu = menuBar()->addMenu("Help");
		QAction *action_support_room = help_menu->addAction(QIcon(":/menu/users.png"), "Support chat");
		help_menu->addSeparator();
		QAction *action_official_site = help_menu->addAction(QIcon(":/menu/smartcomm.png"), "SmartCommunity site");
		QAction *action_official_forum = help_menu->addAction(QIcon(":/menu/smartcomm.png"), "Support forum");
		help_menu->addSeparator();
		QAction *action_about_app = help_menu->addAction(QIcon(":/acid_16.png"), "About " APP_NAME "...");
		QAction *action_about_qt = help_menu->addAction("About Qt...");

	connect(action_quit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(action_settings, SIGNAL(triggered()), this, SLOT(manageSettings()));
	connect(action_support_room, SIGNAL(triggered()), this, SLOT(joinSupportRoom()));
	connect(action_about_qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(action_about_app, SIGNAL(triggered()), this, SLOT(showApplicationInfo()));
	connect(action_new_message, SIGNAL(triggered()), this, SLOT(createNewMessage()));
	connect(action_status_dc, SIGNAL(triggered()), this, SLOT(disconnect()));

	QMenu *traymenu = new QMenu();
	traymenu->insertAction(0, action_new_message);
	traymenu->insertAction(0, action_quit);
	traymenu->insertSeparator(action_quit);
	tray->setContextMenu(traymenu);
}

void Messenger::launch() {
	// Загрузить имя пользователя, сервер и пароль (если пароль сохранён из настроек) и показать окошко входа
	login->show();
	if(settings->value("login/auto", false).toBool()) {
		login->emitFinished();
	}
}

void Messenger::activate() {
	// получен сигнал об успешном вводе информации об учётной записи, можно пытаться подключить клиента…
	// если подключение успешно — скрыть окошко входа и отобразить главное окошко.
	login->setEnabled(false);

	if(!settings->contains("settings/muc_nickname")) settings->setValue("settings/muc_nickname", login->username()); // умолчальный ник в MUC — левая часть Jabber ID

	client_settings->setDomain(login->domain());
	client_settings->setUser(login->username());
	client_settings->setPassword(login->password());
	client_settings->setResource(APP_NAME);
	client_settings->setIgnoreSslErrors(true);
	client_settings->setKeepAliveInterval(settings->value("settings/keepalive_interval", KEEPALIVE_INTERVAL).toInt()); // пинговать раз в минуту — чтобы в случае косяка быстрее спалить.
	client_settings->setKeepAliveTimeout(settings->value("settings/keepalive_timeout", KEEPALIVE_TIMEOUT).toInt()); // таймаут пинга 30 секунд. Если ответ не пришёл, переходить в оффлайн.
	client_settings->setAutoReconnectionEnabled(false);

	transfer_manager->setProxy(settings->value("settings/file_transfer_proxy", PROXY65_JID).toString());
	transfer_manager->setProxyOnly(false);

	// Сохранить пароль, если нужно
	// TODO: сохранять и восстанавливать сервер (домен)
	if(login->savePassword()) {
		settings->setValue("login/password", login->password());
	} else {
		settings->remove("login/password");
	}
	settings->setValue("login/auto", login->autoLogin());
	settings->setValue("login/username", login->username());

	client->connectToServer(*client_settings);
}

void Messenger::handleSuccessfulConnection() {
	// клиент сообщает нам об успешном подключении
	login->hide();
	this->show();
	tray->setOnline();
	chat->setOnline(true);
}

void Messenger::handleDisconnection() {
	// мы отключились от сервера. Успешно или в результате ошибки — об этом говорит то, был и перед этим error().
	this->hide();
	tray->setOffline();
	login->setEnabled(true);
	login->show();
	chat->setOnline(false);
}

void Messenger::handleConnectionError(QXmppClient::Error error) {
	// получена ошибка. После этого сигнала будет тутже испущен disconnected().
	// тут можно передать окошку входа сообщение об ошибке для отображения в нём
	// но можно сделать это и в трее… Также желательно смотреть, что за ошибка произошла.
	tray->debugMessage("Failed to connect to the server. Ensure that you’ve entered valid username/password.");
}

void Messenger::disconnect() {
	// Могли бы и сделать этот метод слотом в клиенте… :can't see:
	client->disconnectFromServer();
}

void Messenger::iconClicked(QSystemTrayIcon::ActivationReason reason) {
	if(reason == QSystemTrayIcon::Trigger) {
		client->isConnected() ? this->setVisible(!this->isVisible()) : login->setVisible(!login->isVisible());
	}
}

void Messenger::createNewMessage() {
	MessageForm *message = new MessageForm(this);
	if(!client->isConnected()) {
		// если клиент не подключён, выключить кнопку отправки сообщения…
		message->disableSendButton();
	}
	connect(message, SIGNAL(readyToSend(MessageForm *)), this, SLOT(sendMessage(MessageForm *)));
	connect(client, SIGNAL(disconnected()), message, SLOT(disableSendButton()));
	connect(client, SIGNAL(connected()), message, SLOT(enableSendButton()));
	message->show();
}

void Messenger::sendMessage(MessageForm *message) {
	client->sendMessage(message->jid(), message->body());
	delete message;
}

void Messenger::gotVoiceCall(QXmppCall *call) {
	// входящий голосовой вызов.
	// знаю, что начинать надо с текстовой функциональности, но голосовая просто гораздо проще.
	// пока примем вызов как есть, потом надо сделать подтверждение… (TODO)
	tray->debugMessage("Got a voice call, accepting");
	call->accept();
}

void Messenger::gotIQ(QXmppIq iq) {
	// Входящая станза IQ. TODO.
}

void Messenger::gotMessage(QXmppMessage message) {
	// Входящее сообщение. Его по идее нужно отобразить особым значком возле элемента ростера и сохранить или,
	// если открыт чат, вхуячить туда…
	// пока что просто отобразим его.
	// TODO: сохранение сообщений до тех пор, пока пользователь сам не решит прочитать
	switch(message.type()) {
		case QXmppMessage::GroupChat: {
			chat->displayMUCMessage(message);
		} break;
		case QXmppMessage::Composing: break;
		case QXmppMessage::Chat:
		default:
			if(message.body().isEmpty()) {
				return;
			}
			if(settings->value("settings/automatically_open_new_tabs", false).toBool()) {
				chat->displayMessage(message);
				return;
			}
			if(!chat->adaTabForJid(message.from())) {
				// Сюда мы попадаем, если надо сохранить сообщение и показать уведомление…
				// TODO
				QMessageBox *notifier = new QMessageBox(QMessageBox::Question, "foo", "some message", QMessageBox::Ok | QMessageBox::Cancel);
				// connect(notifier, SIGNAL(accepted(QString)), this, SLOT(acceptConversation(QString)));
				messages[message.id()] = message;
				tray->popup(notifier, 5);
			} else {
				chat->displayMessage(message);
			}
		break;
	}
}

void Messenger::joinSupportRoom() {
	joinRoom(SUPPORT_JID, settings->value("settings/muc_nickname", login->username()).toString());
}

void Messenger::showApplicationInfo() {
	about->show();
}

void Messenger::rosterChanged(const QString &bare_jid) {
	roster_model.setEntry(bare_jid, client->rosterManager().getRosterEntry(bare_jid));
}

void Messenger::rosterReceived() {
	QStringList list = client->rosterManager().getRosterBareJids();
	QString bare_jid;
	foreach(bare_jid, list) {
		rosterChanged(bare_jid);
	}
}

void Messenger::presenceChanged(const QString &bare_jid, const QString &resource) {
	if(bare_jid == client_settings->jidBare()) {
		return;
	}

/*	if(!roster_model.getRosterItemFromBareJid(bare_jid)) {
		return;
	}*/

	QMap<QString, QXmppPresence> presences = client->rosterManager().getAllPresencesForBareJid(bare_jid);
	roster_model.setPresence(QString("%1/%2").arg(bare_jid).arg(resource), presences[resource]);
}

void Messenger::openChat(const QString &full_jid) {
	chat->openTab(full_jid, TabWidget::Chat);
}

void Messenger::joinRoom(const QString &room_jid, const QString &nick) {
	QXmppMucRoom *room = muc_manager->addRoom(room_jid);
	room->setNickName(nick);
	room->join();
	rooms[room_jid] = room;
	chat->openTab(room_jid, TabWidget::MUC);
}

void Messenger::leaveRoom(const QString &room_jid) {
	rooms[room_jid]->leave();
}

void Messenger::roomParticipantChanged(QString room_jid, QString nick) {
	// TODO: при получении чьего-то presence уведомить об этом таб; при получении кика закрыть таб
	// не закрыть таб (это бесит), а подсветить его как offline
	MUCWidget *widget = (MUCWidget *) chat->getWidgetByJid(room_jid);
	//widget->presenceFrom(nick);
}

void Messenger::sendMUCMessage(QString room, QString message) {
	rooms[room]->sendMessage(message);
}

QSettings *Messenger::settingsManager() {
	return settings;
}

void Messenger::manageSettings() {
	settings_window->show();
}

void Messenger::showProfile(const QXmppVCardIq &whose) {
	(new VcardWindow(this, & whose))->show();
}

void Messenger::requestProfile(const QString &bare_jid) {
	vcard_manager->requestVCard(bare_jid); // Могли бы и слотом сделать, странные какие-то эти ваши девелоперы qxmpp…
}

void Messenger::makeVoiceCall(const QString &bare_jid) {
	QXmppCall *call = call_manager->call(bare_jid);
	// TODO: окошко со статусом вызова
}
