
// qxmpp
#include "qxmpp/QXmppRosterManager.h"

// ACId
#include "messenger.h"
#include "functions.h"
#include "confirmationwindow.h"
#include "joinroomwindow.h"
#include "contact_list/qxmpp_bridge.h"
#include <version.h>

Messenger::Messenger(QWidget *parent): QMainWindow(parent), roster_widget(this), roster_model(this), messages() {
	settings = new QSettings(APP_COMPANY, APP_NAME, this);
	client_settings = new QXmppConfiguration();
	client_presence = new QXmppPresence(QXmppPresence::Available, QXmppPresence::Status::Online);
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
	call_window = 0;
	tray = new TrayIcon(this);

	createConnections();
	createMenus();
	loadSettings();

	setWindowTitle(APP_NAME);
	setWindowIcon(QIcon(":/acid_16.png"));

	roster_widget.setModel(& roster_model);
	setCentralWidget(& roster_widget);

	roster_widget.setItemDelegate(new CL::ItemDelegate());

	QDesktopServices::setUrlHandler("xmpp", this, "handleLink");
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
	roster_widget.setAnimated(settings->value("settings/animate_roster", true).toBool());
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
    connect(login, SIGNAL(showSettingsRequested()), this, SLOT(manageSettings()));
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
	connect(transfer_manager, SIGNAL(fileReceived(QXmppTransferJob *)), this, SLOT(gotFile(QXmppTransferJob *)));
	// Следующая строка — реальный кошмар. Почему одни менеджеры надо вызывать напрямую, а этот через метод?
        connect(& client->vCardManager(), SIGNAL(vCardReceived(const QXmppVCardIq &)), this, SLOT(showProfile(const QXmppVCardIq &)));

	connect(& roster_widget, SIGNAL(showChatDialog(const QString &, const QString &)), this, SLOT(openChat(const QString &, const QString &)));
	connect(& roster_widget, SIGNAL(showProfile(const QString &)), this, SLOT(requestProfile(const QString &)));
	connect(& roster_widget, SIGNAL(removeContact(const QString &)), this, SLOT(removeContact(const QString &)));
	connect(& roster_widget, SIGNAL(makeVoiceCall(const QString &)), this, SLOT(makeVoiceCall(const QString &)));

	connect(settings_window, SIGNAL(modified()), this, SLOT(loadSettings()));
}

void Messenger::createMenus() {
	QMenu *im_menu = menuBar()->addMenu("Program");
		QAction *action_new_message = im_menu->addAction(QIcon(":/menu/document.png"), "New message");
		QMenu *join_room_menu = im_menu->addMenu(QIcon(":/menu/users.png"), "Join a room");
			QAction *action_join_new_room = join_room_menu->addAction(QIcon(":/menu/users.png"), "Join new room");
			QMenu *action_room_bookmarks = join_room_menu->addMenu(QIcon(":/menu/bookmarks.png"), "Bookmarks");
		QAction *action_new_contact = im_menu->addAction(QIcon(":/menu/plus.png"), "Add contact");
		QMenu *connect_service_menu = im_menu->addMenu(QIcon(":/menu/plug.png"), "Connect service");
			QAction *action_connect_yahoo = connect_service_menu->addAction(QIcon(":/menu/yahoo.png"), "Yahoo! Messenger");
			QAction *action_connect_msn = connect_service_menu->addAction(QIcon(":/menu/msn.png"), "MSN (Windows Live)");
                        QAction *action_connect_icq = connect_service_menu->addAction(QIcon(":/menu/icq.png"), "ICQ");
			connect_service_menu->addSeparator();
			QAction *action_connect_lopbox = connect_service_menu->addAction(QIcon(), "Lopbox microblog");
                        QAction *action_connect_rss = connect_service_menu->addAction(QIcon(":/menu/rss.png"), "Add an RSS feed");
			connect_service_menu->addSeparator();
			QAction *action_browse_services = connect_service_menu->addAction(QIcon(":/menu/plug.png"), "Service discovery");
		im_menu->addSeparator();
		QAction *action_settings = im_menu->addAction(QIcon(":/menu/toolbox.png"), "Settings");
		im_menu->addSeparator();
		QAction *action_quit = im_menu->addAction(QIcon(":/menu/door.png"), "Quit");

	QMenu *status_menu = menuBar()->addMenu("Status");
		QAction *action_status_available = status_menu->addAction(QIcon(":/trayicon/online-16px.png"), "Available");
		QAction *action_status_f4c = status_menu->addAction(QIcon(":/trayicon/f4c-16px.png"), "Free for chat");
		QAction *action_status_away = status_menu->addAction(QIcon(":/trayicon/away-16px.png"), "Away");
		QAction *action_status_xa = status_menu->addAction(QIcon(":/trayicon/xa-16px.png"), "Extended away");
		QAction *action_status_dnd = status_menu->addAction(QIcon(":/trayicon/dnd-16px.png"), "Do not disturb");
		status_menu->addSeparator();
		QAction *action_status_dc = status_menu->addAction(QIcon(":/trayicon/offline-16px.png"), "Offline");

	QMenu *help_menu = menuBar()->addMenu("Help");
		QAction *action_support_room = help_menu->addAction(QIcon(":/menu/question.png"), "Support chat");
		QAction *action_official_site = help_menu->addAction(QIcon(":/menu/smartcomm.png"), "Official site");
		help_menu->addSeparator();
		QAction *action_about_app = help_menu->addAction(QIcon(":/acid_16.png"), "About " APP_NAME);
		QAction *action_about_qt = help_menu->addAction("About Qt");

	// Главное меню
	connect(action_quit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(action_settings, SIGNAL(triggered()), this, SLOT(manageSettings()));
	connect(action_new_message, SIGNAL(triggered()), this, SLOT(createNewMessage()));
	connect(action_join_new_room, SIGNAL(triggered()), this, SLOT(joinNewRoom()));

	// Меню статуса
	connect(action_status_available, SIGNAL(triggered()), this, SLOT(setOnlineStatus()));
	connect(action_status_f4c, SIGNAL(triggered()), this, SLOT(setF4CStatus()));
	connect(action_status_away, SIGNAL(triggered()), this, SLOT(setAwayStatus()));
	connect(action_status_xa, SIGNAL(triggered()), this, SLOT(setXAStatus()));
	connect(action_status_dnd, SIGNAL(triggered()), this, SLOT(setDNDStatus()));
	connect(action_status_dc, SIGNAL(triggered()), this, SLOT(disconnect()));

	// Меню Help
	connect(action_official_site, SIGNAL(triggered()), this, SLOT(openOfficialSite()));
	connect(action_about_qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(action_about_app, SIGNAL(triggered()), this, SLOT(showApplicationInfo()));
	connect(action_support_room, SIGNAL(triggered()), this, SLOT(joinSupportRoom()));

	QMenu *traymenu = new QMenu();
	traymenu->insertAction(0, action_new_message);
	traymenu->insertAction(0, action_quit);
	traymenu->insertSeparator(action_quit);
	tray->setContextMenu(traymenu);
}

void Messenger::setOnlineStatus() {
	client_presence->setStatus(QXmppPresence::Status::Online);
	tray->setStatus(QXmppPresence::Status::Online);
	client->setClientPresence(* client_presence);
}

void Messenger::setDNDStatus() {
	client_presence->setStatus(QXmppPresence::Status::DND);
	tray->setStatus(QXmppPresence::Status::DND);
	client->setClientPresence(* client_presence);
}

void Messenger::setAwayStatus() {
	client_presence->setStatus(QXmppPresence::Status::Away);
	tray->setStatus(QXmppPresence::Status::Away);
	client->setClientPresence(* client_presence);
}

void Messenger::setXAStatus() {
	client_presence->setStatus(QXmppPresence::Status::XA);
	tray->setStatus(QXmppPresence::Status::XA);
	client->setClientPresence(* client_presence);
}

void Messenger::setF4CStatus() {
	client_presence->setStatus(QXmppPresence::Status::Chat);
	tray->setStatus(QXmppPresence::Status::Chat);
	client->setClientPresence(* client_presence);
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

	// debug:
	client_settings->setStreamSecurityMode(QXmppConfiguration::TLSDisabled);

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
	settings->setValue("login/domain", login->domain());

	client->connectToServer(*client_settings);
}

void Messenger::handleSuccessfulConnection() {
	// клиент сообщает нам об успешном подключении
	login->hide();
	this->show();
	tray->setStatus(QXmppPresence::Status::Online);
	chat->setOnline(true);
	client->setClientPresence(* client_presence);
}

void Messenger::handleDisconnection() {
	// мы отключились от сервера. Успешно или в результате ошибки — об этом говорит то, был и перед этим error().
	this->hide();
	tray->setStatus(QXmppPresence::Status::Offline);
	login->setEnabled(true);
	login->show();
	chat->setOnline(false);
}

void Messenger::handleConnectionError(QXmppClient::Error error) {
	// получена ошибка. После этого сигнала будет тутже испущен disconnected().
	// тут можно передать окошку входа сообщение об ошибке для отображения в нём
	// но можно сделать это и в трее… Также желательно смотреть, что за ошибка произошла.
	Q_UNUSED(error);
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
	if(settings->value("settings/automatically_accept_calls", false).toBool()) {
		confirmedCall(call, true);
	} else {
		connect(ConfirmationWindow::newCall(call, settings->value("settings/notification_display_time", 5).toInt()), SIGNAL(confirmedCall(QXmppCall *, bool)), this, SLOT(confirmedCall(QXmppCall *, bool)));
	}
}

void Messenger::confirmedCall(QXmppCall *call, bool confirmed) {
	if(!confirmed) {
		call->hangup();
		return;
	}
	call_window = new VoiceCallWindow(this, call);
	connect(call_window, SIGNAL(accepted()), this, SLOT(endCall()));
	call_window->show();
}

void Messenger::gotFile(QXmppTransferJob *job) {
	if(settings->value("settings/automatically_accept_files", false).toBool()) {
		confirmedFile(job, true);
	} else {
		connect(ConfirmationWindow::newFile(job, settings->value("settings/notification_display_time", 5).toInt()), SIGNAL(confirmedFile(QXmppTransferJob *, bool)), this, SLOT(confirmedFile(QXmppTransferJob *, bool)));
	}
}

void Messenger::confirmedFile(QXmppTransferJob *job, bool confirmed) {
	if(!confirmed) {
		job->abort();
		return;
	}
	QFile *target = new QFile(settings->value("settings/savepath", INCOMING_FILES_STORAGE).toString() + "/" + job->fileInfo().name());
	if(target->open(QIODevice::ReadWrite)) {
		job->accept(target);
	} else {
		tray->debugMessage("Cannot write to the target file! Check permissions.");
		job->abort();
	}
}

void Messenger::fileTransferFinished(QXmppTransferJob *job) {
	QXmppTransferJob::Error error = job->error();
	switch(error) {
		case QXmppTransferJob::NoError:
			tray->debugMessage("File has been received successfully");
		break;
		case QXmppTransferJob::AbortError:
			tray->debugMessage("File transfer has been aborted");
		break;
		case QXmppTransferJob::FileAccessError:
			tray->debugMessage("Failed to write to the result file");
		break;
		case QXmppTransferJob::FileCorruptError:
			tray->debugMessage("File transfer seems to have failed: checksum mismatch");
		break;
		case QXmppTransferJob::ProtocolError:
			tray->debugMessage("File transfer failed: protocol error");
		break;
	}

	job->deleteLater();
	// QIODevice закрывает и удаляет сам QXmppTransferJob. Использование deleteLater() диктовано документацией.
}

void Messenger::gotIQ(QXmppIq iq) {
	// Входящая станза IQ. TODO.
	Q_UNUSED(iq);
}

void Messenger::gotMessage(QXmppMessage message) {
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
                // В настройках включено автоматическое отображение новых сообщений
				chat->displayMessage(message, "fooooooo");
				return;
			}
			if(!chat->adaTabForJid(message.from())) {
                // Сохраним текст сообщения
                messages[message.from()][message.id()] = message;
                connect(ConfirmationWindow::newMessage(& message, settings->value("settings/notification_display_time", 5).toInt()), SIGNAL(confirmedMessage(const QString &)), this, SLOT(confirmedMessage(const QString &)));
			} else {
				chat->displayMessage(message, "foo");
			}
		break;
	}
}

void Messenger::confirmedMessage(const QString &message_from) {
    if(!messages.contains(message_from)) {
        // От данного JID ничего не приходило?
		return;
	}

    QMap<QString, QXmppMessage> list = messages[message_from];

    for(QMap<QString, QXmppMessage>::iterator i = list.begin(); i != list.end(); ++ i) {
        chat->displayMessage(i.value(), "foooo");
    }

    messages.erase(messages.find(message_from));
}

void Messenger::joinNewRoom() {
	JoinRoomWindow *window = new JoinRoomWindow(this);
    connect(window, SIGNAL(joinRoomRequested(const QString &)), this, SLOT(processJoinRequest(const QString &)));
	window->show();
}

void Messenger::joinSupportRoom() {
	joinRoom(SUPPORT_JID, settings->value("settings/muc_nickname", login->username()).toString());
}

void Messenger::showApplicationInfo() {
	about->show();
}

void Messenger::rosterChanged(const QString &bare_jid) {
	QXmppRosterIq::Item item = client->rosterManager().getRosterEntry(bare_jid);
	roster_model.updateEntry(bare_jid, item.name(), item.groups());
}

void Messenger::rosterReceived() {
	QStringList list = client->rosterManager().getRosterBareJids();
	foreach(QString bare_jid, list) {
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

	const QMap<QString, QXmppPresence> &presences = client->rosterManager().getAllPresencesForBareJid(bare_jid);
	CL::ContactItem::Status status;
	if (presences.contains(resource))
		status = CL::QXmppBridge::qxmpp2cl(presences[resource]);
	roster_model.setStatus(QString("%1" JID_RESOURCE_SEPARATOR "%2").arg(bare_jid).arg(resource), status);
}

void Messenger::openChat(const QString &full_jid, const QString &nick) {
	chat->openTab(full_jid, nick, TabWidget::Chat);
}

void Messenger::processJoinRequest(const QString &room_jid) {
    return joinRoom(room_jid, settings->value("settings/muc_nickname", login->username()).toString());
}

void Messenger::joinRoom(const QString &room_jid, const QString &nick) {
	QStringList jid = parseJid(room_jid);
	QXmppMucRoom *room = muc_manager->addRoom(room_jid);
	room->setNickName(nick);
	room->join();
	rooms[room_jid] = room;
	chat->openTab(room_jid, jid[2], TabWidget::MUC);
}

void Messenger::leaveRoom(const QString &room_jid) {
	rooms[room_jid]->leave();
}

void Messenger::roomParticipantChanged(QString room_jid, QString nick) {
	// TODO: при получении чьего-то presence уведомить об этом таб; при получении кика закрыть таб
	// не закрыть таб (это бесит), а подсветить его как offline
	Q_UNUSED(nick);
	MUCWidget *widget = (MUCWidget *) chat->getWidgetByJid(room_jid);
	Q_UNUSED(widget);
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

void Messenger::makeVoiceCall(const QString &full_jid) {
	if(call_window) {
		call_window->activateWindow();
		return;
	}

	qDebug() << "calling" << full_jid;

	call_window = new VoiceCallWindow(this, call_manager->call(full_jid));
	connect(call_window, SIGNAL(accepted()), this, SLOT(endCall()));
	call_window->show();
}

void Messenger::endCall() {
	delete call_window;
	call_window = 0;
}

void Messenger::openOfficialSite() {
	QDesktopServices::openUrl(QUrl("http://acid-im.net/"));
}
