#include "messenger.h"
#include "version.h"

Messenger::Messenger(QObject *parent): QObject(parent) {
    settings = new QSettings(APP_COMPANY, APP_NAME, this);
    client = new QXmppClient(this);
    window = new MainWindow();
    login = new LoginForm();

    createConnections();
}

Messenger::~Messenger() {
    delete login;
    delete window;
    delete client;
    delete settings;
}

void Messenger::createConnections() {
    // здесь соединять сигналы и слоты…
    connect(login, SIGNAL(finished()), this, SLOT(activate()));
}

void Messenger::launch() {
    // Загрузить имя пользователя, сервер и пароль (если пароль сохранён из настроек) и показать окошко входа
    login->show();
}

void Messenger::activate() {
    // получен сигнал об успешном вводе информации об учётной записи, можно пытаться подключить клиента…
    // если подключение успешно — скрыть окошко входа и отобразить главное окошко.
    login->setEnabled(false);
    client->connectToServer(login->domain(), login->username(), login->password(), login->domain());
}
