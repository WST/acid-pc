#ifndef MESSENGER_H
#define MESSENGER_H

// Qt
#include <QObject>
#include <QSettings>

// qxmpp
#include <QXmppClient.h>

// ACId
#include "mainwindow.h"
#include "loginform.h"

class Messenger: public QObject {
	Q_OBJECT
	private:
		QXmppClient *client; // клиент
		QSettings *settings; // запоминалка настроек
		MainWindow *window; // окошко с ростером
		LoginForm *login; // окошко ввода информации об учётной записи

	public:
		Messenger(QObject *parent = 0);
		~Messenger();
		void launch();
};

#endif // MESSENGER_H
