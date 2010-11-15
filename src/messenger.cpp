// ACId
#include "messenger.h"
#include <version.h>

Messenger::Messenger(QObject *parent): QObject(parent) {
    settings = new QSettings(APP_COMPANY, APP_NAME, this);
    client_settings = new QXmppConfiguration();
    client = new QXmppClient(this);
    window = new MainWindow(APP_NAME);
    login = new LoginForm(APP_NAME);
    chat = new ChatWindow(window);
    about = new AboutWindow(window);
    tray = new TrayIcon();
    tray->show();

    createConnections();
    createMenus();
}

Messenger::~Messenger() {
    delete about;
    delete tray;
    delete chat;
    delete login;
    delete window;
    delete client;
    delete client_settings;
    delete settings;
}

void Messenger::createConnections() {
    // здесь соединять сигналы и слоты…
    connect(login, SIGNAL(finished()), this, SLOT(activate()));
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconClicked(QSystemTrayIcon::ActivationReason)));
    connect(chat, SIGNAL(aboutToSend(QString, QString)), client, SLOT(sendMessage(QString, QString)));

    // сигналы клиента
    connect(client, SIGNAL(connected()), this, SLOT(handleSuccessfulConnection()));
    connect(client, SIGNAL(disconnected()), this, SLOT(handleDisconnection()));
    connect(client, SIGNAL(error(QXmppClient::Error)), this, SLOT(handleConnectionError(QXmppClient::Error)));
    connect(& client->callManager(), SIGNAL(callReceived(QXmppCall *)), this, SLOT(gotVoiceCall(QXmppCall *)));
    connect(client, SIGNAL(iqReceived(QXmppIq)), this, SLOT(gotIQ(QXmppIq)));
    connect(client, SIGNAL(messageReceived(QXmppMessage)), this, SLOT(gotMessage(QXmppMessage)));
}

void Messenger::createMenus() {
    QMenu *im_menu = window->menuBar()->addMenu("Program");
    QMenu *status_menu = window->menuBar()->addMenu("Status");
    QMenu *help_menu = window->menuBar()->addMenu("Help");

    QAction *action_new_message = im_menu->addAction("New message...");
    im_menu->addSeparator();
    QMenu *style_menu = im_menu->addMenu("Style");
    QAction *action_quit = im_menu->addAction("Quit");

    QAction *action_support_room = help_menu->addAction("Support chat");
    QAction *action_about_app = help_menu->addAction(QIcon(":/acid_16.png"), "About " APP_NAME "...");
    QAction *action_about_qt = help_menu->addAction("About Qt...");

    QAction *action_status_available = status_menu->addAction(QIcon(":/trayicon/online-16px.png"), "Available");
    QAction *action_status_f4c = status_menu->addAction("Free for chat");
    QAction *action_status_away = status_menu->addAction("Away");
    QAction *action_status_xa = status_menu->addAction("Extended away");
    QAction *action_status_dnd = status_menu->addAction("Do not disturb");
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
    client_settings->setKeepAliveInterval(60);
    client_settings->setKeepAliveTimeout(30);
    client_settings->setAutoReconnectionEnabled(false);

    client->connectToServer(*client_settings);
    client->transferManager().setProxy(PROXY65_JID);
}

void Messenger::handleSuccessfulConnection() {
    // клиент сообщает нам об успешном подключении
    login->hide();
    window->show();
    tray->setOnline();
    chat->setOnline(true);
}

void Messenger::handleDisconnection() {
    // мы отключились от сервера. Успешно или в результате ошибки — об этом говорит то, был и перед этим error().
    window->hide();
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
	client->isConnected() ? window->setVisible(!window->isVisible()) : login->setVisible(!login->isVisible());
    }
}

void Messenger::createNewMessage() {
    MessageForm *message = new MessageForm(window);
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
    // Входящая станза IQ (возможно, запрос версии или last activity).
    tray->debugMessage("Got an incoming IQ");
}

void Messenger::gotMessage(QXmppMessage message) {
    // Входящее сообщение. Его по идее нужно отобразить особым значком возле элемента ростера и сохранить или,
    // если открыт чат, вхуячить туда…
    // пока что просто отобразим его.
    // TODO: приём MUC-сообщений
    if(!chat->adaTabForJid(message.from())) {
	chat->openTab(message.from(), TabWidget::Chat);
    }
    chat->displayMessage(message);
}

void Messenger::setPlastiqueStyle() {
    qApp->setStyle("plastique");
}

void Messenger::setCleanlooksStyle() {
    qApp->setStyle("cleanlooks");
}

void Messenger::joinSupportRoom() {
    chat->openTab(SUPPORT_JID "/" + login->username(), TabWidget::MUC);
}

void Messenger::showApplicationInfo() {
    about->show();
}
