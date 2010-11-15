#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent): QDialog(parent), ui(new Ui::ChatWindow) {
    ui->setupUi(this);
}

ChatWindow::~ChatWindow() {
    delete ui;
}

void ChatWindow::displayMessage(QXmppMessage &message) {
    TabWidget *widget;
    for(int i = 0; i < ui->tabWidget->count(); i ++) {
	if((widget = (TabWidget *) ui->tabWidget->widget(i))->getJid() == message.from()) {
	    // дальше зависит от типа виджета, который можно сохранять в некоторой переменной класса TabWidget
	    // Это как бэ тоже TODO.
	    ((ChatWidget *) widget)->insertMessage(message);
	}
    }
}

bool ChatWindow::adaTabForJid(QString fulljid) {
    TabWidget *widget;
    for(int i = 0; i < ui->tabWidget->count(); i ++) {
	if((widget = (TabWidget *) ui->tabWidget->widget(i))->getJid() == fulljid) {
	    return true;
	}
    }
    return false;
}

void ChatWindow::openTab(QString fulljid, TabWidget::Type type) {
    if(!isVisible()) {
	show();
    }
    switch(type) {
	case TabWidget::Chat: {
		ChatWidget *widget = new ChatWidget(fulljid);
		connect(widget, SIGNAL(aboutToSend(QString,QString)), this, SIGNAL(aboutToSend(QString, QString)));
		ui->tabWidget->addTab(widget, "tab");
	} break;
	case TabWidget::MUC: {
		// TODO
	} break;
    }
}
