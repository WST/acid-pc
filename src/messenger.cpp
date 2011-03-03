// ACId
#include "messenger.h"
#include "functions.h"
#include <version.h>

Messenger::Messenger(QWidget *parent): QMainWindow(parent), roster_widget(this), roster_model(this) {
	settings = new QSettings(APP_COMPANY, APP_NAME, this);
	client_settings = new QXmppConfiguration();
	client = new QXmppClient(this);
	call_manager = new QXmppCallManager();
	muc_manager = new QXmppMucManager();
	client->addExtension(call_manager);
	client->addExtension(muc_manager);
	//window = new MainWindow(APP_NAME);
	login = new LoginForm(APP_NAME);
	chat = new ChatWindow(this);
	about = new AboutWindow(this);
	tray = new TrayIcon();
	tray->show();

	createConnections();
	createMenus();
	loadSettings();
	
	setWindowTitle(APP_NAME);
	setWindowIcon(QIcon(":/acid_16.png"));
	
	roster_widget.setModel(& roster_model);
	setCentralWidget(& roster_widget);
	
	RosterItemDelegate *delegate = new RosterItemDelegate();
	roster_widget.setItemDelegate(delegate);
	
	connect(& roster_widget, SIGNAL(showChatDialog(QString)), this, SIGNAL(openChat(QString)));
	//connect(& roster_widget, SIGNAL(showProfile(QString)), this, SIGNAL(showProfile(QString)));
	//connect(& roster_widget, SIGNAL(removeContact(QString)), this, SIGNAL(removeContact(QString)));
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
    if(settings->contains("login/auto")) login->setAutoLogin(settings->value("login/auto").toBool());
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
    connect(call_manager, SIGNAL(callReceived(QXmppCall *)), this, SLOT(gotVoiceCall(QXmppCall *)));
    connect(client, SIGNAL(iqReceived(QXmppIq)), this, SLOT(gotIQ(QXmppIq)));
    connect(client, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(gotMessage(QXmppMessage)));
	
	connect(& client->rosterManager(), SIGNAL(rosterReceived()), this, SLOT(rosterReceived()));
	connect(& client->rosterManager(), SIGNAL(rosterChanged(const QString&)), this, SLOT(rosterChanged(const QString&)));
	connect(& client->rosterManager(), SIGNAL(presenceChanged(const QString&, const QString&)), this, SLOT(presenceChanged(const QString&, const QString&)));

	connect(this, SIGNAL(showChatDialog(QString)), this, SLOT(openChat(QString)));
}

void Messenger::createMenus() {
    // TODO: расположить нормально, соответственно реальной структуре меню
    QMenu *im_menu = menuBar()->addMenu("Program");
    QMenu *status_menu = menuBar()->addMenu("Status");
    QMenu *help_menu = menuBar()->addMenu("Help");

    QAction *action_new_message = im_menu->addAction(QIcon(":/menu/document.png"), "New message...");
    im_menu->addSeparator();
    QMenu *style_menu = im_menu->addMenu(QIcon(":/menu/t-shirt.png"), "Style");
    QAction *action_quit = im_menu->addAction("Quit");

    QAction *action_support_room = help_menu->addAction(QIcon(":/menu/users.png"), "Support chat");
    help_menu->addSeparator();
    QAction *action_official_site = help_menu->addAction(QIcon(":/menu/smartcomm.png"), "SmartCommunity site");
    QAction *action_official_forum = help_menu->addAction(QIcon(":/menu/smartcomm.png"), "Support forum");
    help_menu->addSeparator();
    QAction *action_about_app = help_menu->addAction(QIcon(":/acid_16.png"), "About " APP_NAME "...");
    QAction *action_about_qt = help_menu->addAction("About Qt...");

    QAction *action_status_available = status_menu->addAction(QIcon(":/trayicon/online-16px.png"), "Available");
    QAction *action_status_f4c = status_menu->addAction(QIcon(":/trayicon/online-16px.png"), "Free for chat");
    QAction *action_status_away = status_menu->addAction(QIcon(":/trayicon/online-16px.png"), "Away");
    QAction *action_status_xa = status_menu->addAction(QIcon(":/trayicon/online-16px.png"), "Extended away");
    QAction *action_status_dnd = status_menu->addAction(QIcon(":/trayicon/online-16px.png"), "Do not disturb");
    status_menu->addSeparator();
    QAction *action_status_dc = status_menu->addAction(QIcon(":/trayicon/offline-16px.png"), "Offline");

    QAction *action_plastique_style = style_menu->addAction("Plastique");
    QAction *action_cleanlooks_style = style_menu->addAction("Cleanlooks");
    // TODO: отображать все доступные стили

    connect(action_quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(action_support_room, SIGNAL(triggered()), this, SLOT(joinSupportRoom()));
    connect(action_about_qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(action_about_app, SIGNAL(triggered()), this, SLOT(showApplicationInfo()));
    connect(action_new_message, SIGNAL(triggered()), this, SLOT(createNewMessage()));
    connect(action_status_dc, SIGNAL(triggered()), this, SLOT(disconnect()));

    connect(action_plastique_style, SIGNAL(triggered()), this, SLOT(setPlastiqueStyle()));
    connect(action_cleanlooks_style, SIGNAL(triggered()), this, SLOT(setCleanlooksStyle()));

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

    client_settings->setDomain(login->domain());
    client_settings->setUser(login->username());
    client_settings->setPassword(login->password());
    client_settings->setResource(APP_NAME);
    client_settings->setIgnoreSslErrors(true);
    client_settings->setKeepAliveInterval(60); // пинговать раз в минуту — чтобы в случае косяка быстрее спалить.
    client_settings->setKeepAliveTimeout(30); // таймаут пинга 30 секунд. Если ответ не пришёл, переходить в оффлайн.
    client_settings->setAutoReconnectionEnabled(false);

    // Сохранить пароль, если нужно
    if(login->savePassword()) {
	settings->setValue("login/password", login->password());
    } else {
	settings->remove("login/password");
    }
    settings->setValue("login/auto", login->autoLogin());
    settings->setValue("login/username", login->username());

    client->connectToServer(*client_settings);
    //client->transferManager().setProxy(PROXY65_JID);
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
    // Входящая станза IQ (возможно, запрос версии или last activity). TODO.

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
	    chat->displayMessage(message);
	break;
    }
}

void Messenger::setPlastiqueStyle() {
    qApp->setStyle("plastique");
}

void Messenger::setCleanlooksStyle() {
    qApp->setStyle("cleanlooks");
}

void Messenger::joinSupportRoom() {
    joinRoom(SUPPORT_JID, login->username());
}

void Messenger::showApplicationInfo() {
    about->show();
}

void Messenger::rosterChanged(const QString& bare_jid) {
	roster_model.updateRosterEntry(bare_jid, client->rosterManager().getRosterEntry(bare_jid));
}

void Messenger::rosterReceived() {
	QStringList list = client->rosterManager().getRosterBareJids();
	QString bare_jid;
	foreach(bare_jid, list) {
		rosterChanged(bare_jid);
	}
}

void Messenger::presenceChanged(const QString& bare_jid, const QString& resource) {
	if(bare_jid == client_settings->jidBare()) {
		return;
	}

	if(!roster_model.getRosterItemFromBareJid(bare_jid)) {
		return;
	}

	QString jid = bare_jid + "/" + resource;
	QMap<QString, QXmppPresence> presences = client->rosterManager().getAllPresencesForBareJid(bare_jid);
	roster_model.updatePresence(bare_jid, presences);
}

void Messenger::openChat(const QString &full_jid) {
    chat->openTab(full_jid, TabWidget::Chat);
}

void Messenger::joinRoom(const QString &room_jid, const QString &nick) {
    muc_manager->joinRoom(room_jid, nick);
    chat->openTab(room_jid, TabWidget::MUC);
}

void Messenger::leaveRoom(const QString &room_jid) {
    muc_manager->leaveRoom(room_jid);
}

void Messenger::sendMUCMessage(QString room, QString message) {
    muc_manager->sendMessage(room, message); // могли бы ето и слотом сделать…
}

QSettings *Messenger::settingsManager() {
	return settings;
}
