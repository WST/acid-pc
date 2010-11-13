#include "messenger.h"

Messenger::Messenger(QObject *parent): QObject(parent) {
    settings = new QSettings("SmartCommunity", "ACId", this);
    client = new QXmppClient(this);
    login = new LoginForm();
    window = new MainWindow();
}

Messenger::~Messenger() {
    delete client;
    delete settings;
}

void Messenger::launch() {
    login->show();
}
