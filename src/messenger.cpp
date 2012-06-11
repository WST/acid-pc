
// qxmpp
#include <qxmpp/QXmppRosterManager.h>
#include <qxmpp/QXmppBookmarkSet.h>

// ACId
#include "messenger.h"
#include "functions.h"
#include "confirmationwindow.h"
#include "newcontactwindow.h"
#include "joinroomwindow.h"
#include <version.h>

// Qt
#include <QHostAddress>

Messenger::Messenger(QTranslator *app_translator, QSettings *app_settings): QMainWindow(0), roster_widget(this), roster_model(this), messages() {
    translator = app_translator;

    settings = app_settings;
    client_settings = new QXmppConfiguration();
	client_presence = new QXmppPresence(QXmppPresence::Available, QXmppPresence::Status::Online);
	client = new QXmppClient(this);

	transfer_manager = new QXmppTransferManager();
	call_manager = new QXmppCallManager();
	muc_manager = new QXmppMucManager();
    vcard_manager = new QXmppVCardManager(); // странно, что его надо создавать, а обращаться через метод, но так и есть…
	version_manager = new QXmppVersionManager();
	disco_manager = new QXmppDiscoveryManager();
    bookmark_manager = new QXmppBookmarkManager();
	client->addExtension(call_manager);
	client->addExtension(muc_manager);
	client->addExtension(transfer_manager);
    client->addExtension(vcard_manager);
	client->addExtension(version_manager);
	client->addExtension(disco_manager);
    client->addExtension(bookmark_manager);

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
	rosterBridge = new CL::QXmppBridge(&roster_model, &client->rosterManager());

	createConnections();
	createMenus();
	loadSettings();

	setWindowTitle(APP_NAME);
	setWindowIcon(QIcon(":/acid_16.png"));

	roster_widget.setModel(& roster_model);
	setCentralWidget(& roster_widget);

	roster_widget.setItemDelegate(new CL::ItemDelegate());

	QDesktopServices::setUrlHandler("xmpp", this, "handleLink");

    counter = 0;
}

Messenger::~Messenger() {
	saveSettings();
}

void Messenger::saveSettings() {
	settings->setValue("main/geometry", this->saveGeometry());
	settings->setValue("login/geometry", login->saveGeometry());
	settings->setValue("chat/geometry", chat->saveGeometry());
	settings->setValue("about/geometry", about->saveGeometry());

	settings->sync();
}

QString Messenger::id() {
    return "acid_" + QString::number(++ counter);
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

    if(settings->contains("settings/stun_server")) {
        call_manager->setStunServer(QHostAddress(settings->value("settings/stun_server", STUN_ADDRESS).toString()));
    }

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

    connect(& client->rosterManager(), SIGNAL(subscriptionReceived(const QString &)), this, SLOT(handleSubscriptionRequest(const QString &)));
    connect(& client->vCardManager(), SIGNAL(vCardReceived(const QXmppVCardIq &)), this, SLOT(showProfile(const QXmppVCardIq &)));

	connect(call_manager, SIGNAL(callReceived(QXmppCall *)), this, SLOT(gotVoiceCall(QXmppCall *)));
	// This signal does not exist
//	connect(muc_manager, SIGNAL(roomParticipantChanged(QString,QString)), this, SLOT(roomParticipantChanged(QString, QString)));
	connect(transfer_manager, SIGNAL(fileReceived(QXmppTransferJob *)), this, SLOT(gotFile(QXmppTransferJob *)));
    connect(bookmark_manager, SIGNAL(bookmarksReceived(const QXmppBookmarkSet &)), this, SLOT(handleBookmarks(const QXmppBookmarkSet &)));

    connect(& roster_widget, SIGNAL(wannaShowChatDialog(CL::ContactItem *)), this, SLOT(openChat(CL::ContactItem *)));
    connect(& roster_widget, SIGNAL(wannaShowProfile(CL::ContactItem *)), this, SLOT(requestProfile(CL::ContactItem *)));
    connect(& roster_widget, SIGNAL(wannaRemoveContact(CL::ContactItem *)), this, SLOT(removeContact(CL::ContactItem *)));
    connect(& roster_widget, SIGNAL(wannaMakeVoiceCall(CL::ContactItem *)), this, SLOT(makeVoiceCall(CL::ContactItem *)));
    connect(& roster_widget, SIGNAL(wannaSendFile(CL::ContactItem *)), this, SLOT(showSendFileDialog(CL::ContactItem *)));

	connect(settings_window, SIGNAL(modified()), this, SLOT(loadSettings()));
}

void Messenger::createMenus() {
    QMenu *im_menu = menuBar()->addMenu(tr("Program"));
        QAction *action_new_message = im_menu->addAction(QIcon(":/menu/document.png"), tr("New message"));
        QMenu *join_room_menu = im_menu->addMenu(QIcon(":/menu/users.png"), tr("Join a room"));
            QAction *action_join_new_room = join_room_menu->addAction(QIcon(":/menu/users.png"), tr("Join new room"));
            action_edit_bookmarks = join_room_menu->addAction(QIcon(":/menu/bookmarks.png"), tr("Manage bookmarks"));
            room_bookmarks_menu = join_room_menu->addMenu(QIcon(":/menu/bookmarks.png"), tr("Bookmarks"));
        QAction *action_new_contact = im_menu->addAction(QIcon(":/menu/plus.png"), tr("Add contact"));
        QMenu *roster_menu = im_menu->addMenu(QIcon(":/menu/user-black.png"), tr("Roster"));
            QAction *action_hide_offline_contacts = roster_menu->addAction(QIcon(":/menu/user-silhouette.png"), tr("Hide offline items"));
            QAction *action_hide_services = roster_menu->addAction(QIcon(":/menu/plug.png"), tr("Hide gateways"));
            action_hide_offline_contacts->setCheckable(true);
            action_hide_services->setCheckable(true);
        QMenu *connect_service_menu = im_menu->addMenu(QIcon(":/menu/plug.png"), tr("Connect service"));
            QAction *action_connect_yahoo = connect_service_menu->addAction(QIcon(":/menu/yahoo.png"), "Yahoo! Messenger");
            QAction *action_connect_msn = connect_service_menu->addAction(QIcon(":/menu/msn.png"), "MSN (Windows Live)");
                        QAction *action_connect_icq = connect_service_menu->addAction(QIcon(":/menu/icq.png"), "ICQ");
			connect_service_menu->addSeparator();
            QAction *action_connect_lopbox = connect_service_menu->addAction(QIcon(), tr("Lopbox microblog"));
                        QAction *action_connect_rss = connect_service_menu->addAction(QIcon(":/menu/rss.png"), tr("Add an RSS feed"));
			connect_service_menu->addSeparator();
            QAction *action_browse_services = connect_service_menu->addAction(QIcon(":/menu/plug.png"), tr("Service discovery"));
		im_menu->addSeparator();
        QAction *action_settings = im_menu->addAction(QIcon(":/menu/toolbox.png"), tr("Settings"));
		im_menu->addSeparator();
        QAction *action_quit = im_menu->addAction(QIcon(":/menu/door.png"), tr("Quit"));

    QMenu *status_menu = menuBar()->addMenu(tr("Status"));
        QAction *action_status_available = status_menu->addAction(QIcon(":/trayicon/online-16px.png"), tr("Available"));
        QAction *action_status_f4c = status_menu->addAction(QIcon(":/trayicon/f4c-16px.png"), tr("Free for chat"));
        QAction *action_status_away = status_menu->addAction(QIcon(":/trayicon/away-16px.png"), tr("Away"));
        QAction *action_status_xa = status_menu->addAction(QIcon(":/trayicon/xa-16px.png"), tr("Extended away"));
        QAction *action_status_dnd = status_menu->addAction(QIcon(":/trayicon/dnd-16px.png"), tr("Do not disturb"));
		status_menu->addSeparator();
        QAction *action_status_dc = status_menu->addAction(QIcon(":/trayicon/offline-16px.png"), tr("Offline"));

    QMenu *help_menu = menuBar()->addMenu(tr("Help"));
        QAction *action_support_room = help_menu->addAction(QIcon(":/menu/question.png"), tr("Support chat"));
        QAction *action_official_site = help_menu->addAction(QIcon(":/menu/smartcomm.png"), tr("Official site"));
		help_menu->addSeparator();
        QAction *action_about_app = help_menu->addAction(QIcon(":/acid_16.png"), tr("About"));
        //QAction *action_about_qt = help_menu->addAction(tr("About Qt"));

	// Главное меню
	connect(action_quit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(action_settings, SIGNAL(triggered()), this, SLOT(manageSettings()));
	connect(action_new_message, SIGNAL(triggered()), this, SLOT(createNewMessage()));
	connect(action_join_new_room, SIGNAL(triggered()), this, SLOT(joinNewRoom()));
    connect(action_new_contact, SIGNAL(triggered()), this, SLOT(showNewContactWindow()));
    connect(action_hide_offline_contacts, SIGNAL(triggered(bool)), & roster_widget, SLOT(setHideOfflineItems(bool)));
    connect(action_hide_services, SIGNAL(triggered(bool)), & roster_widget, SLOT(setHideServices(bool)));
    connect(action_browse_services, SIGNAL(triggered()), this, SLOT(openServiceBrowser()));
    connect(action_edit_bookmarks, SIGNAL(triggered()), this, SLOT(showBookmarkManager()));
    room_bookmarks_menu->setEnabled(false);

	// Меню статуса
	connect(action_status_available, SIGNAL(triggered()), this, SLOT(setOnlineStatus()));
	connect(action_status_f4c, SIGNAL(triggered()), this, SLOT(setF4CStatus()));
	connect(action_status_away, SIGNAL(triggered()), this, SLOT(setAwayStatus()));
	connect(action_status_xa, SIGNAL(triggered()), this, SLOT(setXAStatus()));
	connect(action_status_dnd, SIGNAL(triggered()), this, SLOT(setDNDStatus()));
	connect(action_status_dc, SIGNAL(triggered()), this, SLOT(disconnect()));

	// Меню Help
	connect(action_official_site, SIGNAL(triggered()), this, SLOT(openOfficialSite()));
    //connect(action_about_qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
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
    client_settings->setAutoAcceptSubscriptions(false);

	// debug:
	client_settings->setStreamSecurityMode(QXmppConfiguration::TLSDisabled);

	transfer_manager->setProxy(settings->value("settings/file_transfer_proxy", PROXY65_JID).toString());
    transfer_manager->setProxyOnly(true);

	// Сохранить пароль, если нужно
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
    Q_UNUSED(error);
    connect(ConfirmationWindow::confirmRegistration(), SIGNAL(confirmedRegistration(bool)), this, SLOT(inBandRegister(bool)));
}

void Messenger::inBandRegister(bool confirmed) {
    if(confirmed) {
        // Смотри http://dev.jsmart.web.id/issues/166
        QDesktopServices::openUrl(QUrl(REGISTER_URL));
        handleDisconnection();
    } else {
        handleDisconnection();
    }
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

void Messenger::showNewContactWindow() {
    NewContactWindow *window = new NewContactWindow(this);
    connect(window, SIGNAL(newContactAdditionRequested(const QString &, const QString &)), this, SLOT(addNewContact(const QString &, const QString &)));
    // NOTE: объекты window не удаляются, а как бы их покороче удалять?
    window->show();
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
        chat->openTransferManager(job);
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
        case QXmppMessage::Error: break;
		case QXmppMessage::Chat:
        case QXmppMessage::Normal:
		default:
			if(message.body().isEmpty()) {
				return;
			}
            // Если контакт есть в ростере, получим указатель на него (иначе ноль)
            CL::ContactItem *roster_item = roster_model.getContact(message.from());
            QString nick = roster_item ? roster_item->getNick() : parseJid(message.from())[2];
            QString bare_jid = parseJid(message.from())[1];
			if(settings->value("settings/automatically_open_new_tabs", false).toBool()) {
                // В настройках включено автоматическое отображение новых сообщений
                chat->displayMessage(message, nick, roster_model.getContact(message.from()));
				return;
			}
			if(!chat->adaTabForJid(message.from())) {
                // Сохраним текст сообщения
                messages[bare_jid][message.id()] = message;
				if (roster_item) {
	                roster_item->setNotified(true, tr("There are unread messages"));
				}
                connect(ConfirmationWindow::newMessage(& message, settings->value("settings/notification_display_time", 5).toInt()), SIGNAL(confirmedMessage(const QString &)), this, SLOT(confirmedMessage(const QString &)));
			} else {
                // Здесь может оказаться, что это сообщение от комнаты (например капча)
                if(getRoomByJid(message.from())) {
                    // TODO
                } else {
                    chat->displayMessage(message, nick, roster_model.getContact(message.from()));
                }
			}
		break;
	}
}

void Messenger::confirmedMessage(const QString &message_from) {
    QStringList jid = parseJid(message_from);

    if(!messages.contains(jid[1])) {
        // От данного JID ничего не приходило?
        return;
    }

    // Если контакт есть в ростере, получим указатель на него (иначе ноль)
    CL::ContactItem *roster_item = roster_model.getContact(message_from);
    QString nick = roster_item ? roster_item->getNick() : jid[2];

    QMap<QString, QXmppMessage> list = messages[jid[1]];

    for(QMap<QString, QXmppMessage>::iterator i = list.begin(); i != list.end(); ++ i) {
        chat->displayMessage(i.value(), nick, roster_item);
    }

    messages.erase(messages.find(jid[1]));
    if(roster_item) roster_item->setNotified(false);
}

void Messenger::joinNewRoom() {
    JoinRoomWindow *window = new JoinRoomWindow(this);
    connect(window, SIGNAL(joinRoomRequested(const QString &)), this, SLOT(processJoinRequest(const QString &)));
	window->show();
}

void Messenger::rejoinRoom(QString error_message, QString room_jid) {
    JoinRoomWindow *window = new JoinRoomWindow(error_message, room_jid, this);
    connect(window, SIGNAL(joinRoomRequested(const QString &)), this, SLOT(processJoinRequest(const QString &)));
    window->show();
}

void Messenger::joinSupportRoom() {
	joinRoom(SUPPORT_JID, settings->value("settings/muc_nickname", login->username()).toString());
}

void Messenger::showApplicationInfo() {
	about->show();
}

void Messenger::openChat(CL::ContactItem *item) {
    chat->openChatTab(item->getBareJid(), item->getNick(), item);

    QMap<QString, QXmppMessage> list = messages[item->getBareJid()];
    for(QMap<QString, QXmppMessage>::iterator i = list.begin(); i != list.end(); ++ i) {
        chat->displayMessage(i.value(), item->getNick(), item);
    }

    messages.erase(messages.find(item->getBareJid()));
    item->setNotified(false);
}

void Messenger::processJoinRequest(const QString &room_jid) {
    return joinRoom(room_jid, settings->value("settings/muc_nickname", login->username()).toString());
}

// Подготовка входа в конференцию
void Messenger::joinRoom(const QString &room_jid, const QString &nick) {
	QStringList jid = parseJid(room_jid);
	QXmppMucRoom *room = muc_manager->addRoom(room_jid);
	room->setNickName(nick);
	room->join();

    // Мы зашли в комнату и получили объект QXmppMucRoom. Этот объект является источником всех сигналов, связанных с комнатой.
    connect(room, SIGNAL(joined()), this, SLOT(joinedRoom()));
    connect(room, SIGNAL(left()), this, SLOT(leftRoom()));
    connect(room, SIGNAL(kicked(const QString &, const QString &)), this, SLOT(kickedFromRoom(const QString &, const QString &)));

    // Остальные события мы обработаем в табе комнаты
}

// Эта функция вызывается, когда вход в комнату не выполнен или выполнен выход из комнаты
void Messenger::leftRoom() {
    QXmppMucRoom *room = (QXmppMucRoom *) sender();
    delete room;
}

// Эта функция вызывается, когда пользователя выгнали из комнаты
void Messenger::kickedFromRoom(const QString &jid, const QString &reason) {
    QXmppMucRoom *room = (QXmppMucRoom *) sender();
    rejoinRoom(tr("You have been kicked: ") + reason, room->jid());
    delete room;
    // TODO: удалять таб или делать его неактивным
}

// Эта функция вызывается, когда вход в комнату выполнен успешно
void Messenger::joinedRoom() {
    chat->openMUCTab((QXmppMucRoom *) sender());
}

void Messenger::leaveRoom(const QString &room_jid) {
    QXmppMucRoom *room = getRoomByJid(room_jid);
    if(room) room->leave();
}

void Messenger::roomParticipantChanged(QString room_jid, QString nick) {
	// TODO: при получении чьего-то presence уведомить об этом таб; при получении кика закрыть таб
	// не закрыть таб (это бесит), а подсветить его как offline
}

QXmppMucRoom *Messenger::getRoomByJid(const QString &jid) {
    QListIterator<QXmppMucRoom *> iterator(muc_manager->rooms());
     while (iterator.hasNext()) {
         QXmppMucRoom *next = iterator.next();
         if(next->jid() == jid) return next;
     }
     return 0;
}

void Messenger::sendMUCMessage(QString jid, QString message) {
    QXmppMucRoom *room = getRoomByJid(jid);
    if(room) room->sendMessage(message);
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

void Messenger::requestProfile(CL::ContactItem *item) {
    vcard_manager->requestVCard(item->getBareJid());
}

void Messenger::makeVoiceCall(CL::ContactItem *item) {
	if(call_window) {
		call_window->activateWindow();
		return;
	}

    call_window = new VoiceCallWindow(this, call_manager->call(item->fullJid()));
	connect(call_window, SIGNAL(accepted()), this, SLOT(endCall()));
	call_window->show();
}

void Messenger::addNewContact(const QString &jid, const QString &nick) {
    client->rosterManager().addItem(jid, nick);
    client->rosterManager().subscribe(jid);
}

void Messenger::removeContact(CL::ContactItem *item) {
    client->rosterManager().unsubscribe(item->getBareJid());
    client->rosterManager().removeItem(item->getBareJid());
}

void Messenger::endCall() {
	delete call_window;
	call_window = 0;
}

void Messenger::openOfficialSite() {
    QDesktopServices::openUrl(QUrl(APP_SITE));
}

void Messenger::openServiceBrowser() {
    chat->openDiscoTab(disco_manager, login->domain());
}

void Messenger::handleSubscriptionRequest(const QString &jid) {
    ConfirmationWindow *window = ConfirmationWindow::confirmSubscriptionRequest(jid);
    connect(window, SIGNAL(confirmedSubscription(const QString &, bool)), this, SLOT(answerSubscriptionRequest(const QString &, bool)));
}

void Messenger::answerSubscriptionRequest(const QString &jid, bool accepted) {
    accepted ? client->rosterManager().acceptSubscription(jid) : client->rosterManager().refuseSubscription(jid);
}

void Messenger::handleBookmarks(const QXmppBookmarkSet &bookmarks) {
    room_bookmarks_menu->setEnabled(true);
    room_bookmarks_menu->clear();
    QListIterator<QXmppBookmarkConference> iterator(bookmarks.conferences());
    QAction *action;
    while(iterator.hasNext()) {
        QXmppBookmarkConference room = iterator.next();
        QString nickname = room.nickName().isEmpty() ? settings->value("settings/muc_nickname", login->username()).toString() : room.nickName();

        if(room.autoJoin()) {
            // Автовход в конференцию
            joinRoom(room.jid(), nickname);
        }

        action = room_bookmarks_menu->addAction(QIcon(":/menu/bookmarks.png"), tr("Join ") + room.name());
        action->setData(room.jid() + "/" + nickname);
        connect(action, SIGNAL(triggered()), this, SLOT(processBookmarkClick()));
    }
}

void Messenger::showBookmarkManager() {
    chat->openBookmarksEditor(bookmark_manager);
}

void Messenger::processBookmarkClick() {
    QAction *action = (QAction *) sender();
    QStringList jid = parseJid(action->data().toString());
    joinRoom(jid[1], jid[5]);
}

void Messenger::showSendFileDialog(CL::ContactItem *item) {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open a file"));
    if(!filename.isEmpty() && QFile::exists(filename)) {
        chat->openTransferManager(transfer_manager->sendFile(item->fullJid(), filename, id()));
    }
}
