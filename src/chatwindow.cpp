#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(MainWindow *parent): QDialog(parent), ui(new Ui::ChatWindow) {
    main = parent;
    ui->setupUi(this);
    setWindowIcon(QIcon(":/acid_16.png"));
}

ChatWindow::~ChatWindow() {
    delete ui;
}

void ChatWindow::displayMessage(QXmppMessage &message) {
    if(!isVisible()) {
	show();
    }
    TabWidget *widget;
    for(int i = 0; i < ui->tabWidget->count(); i ++) {
	if((widget = (TabWidget *) ui->tabWidget->widget(i))->getJid() == message.from()) {
	    TabWidget::Type type = widget->getType();
	    switch(type) {
		case TabWidget::MUC: {
			((MUCWidget *) widget)->insertMessage(message);
		} break;
		case TabWidget::Chat: {
			((ChatWidget *) widget)->insertMessage(message);
		} break;
	    }
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
    switch(type) {
	case TabWidget::Chat: {
		ChatWidget *widget = new ChatWidget(fulljid);
		connect(widget, SIGNAL(aboutToSend(QString,QString)), this, SIGNAL(aboutToSend(QString, QString)));
		ui->tabWidget->addTab(widget, "tab");
		widget->setOnline(online);
	} break;
	case TabWidget::MUC: {
		// TODO
		// NOTE: сюда передаётся JID вида комната@сервер/ник
	} break;
    }
}

void ChatWindow::setOnline(bool is_online) {
    online = is_online;
    TabWidget *widget;
    for(int i = 0; i < ui->tabWidget->count(); i ++) {
	widget = (TabWidget *) ui->tabWidget->widget(i);
	switch(widget->getType()) {
	    case TabWidget::Chat: {
		    ((ChatWidget *) widget)->setOnline(is_online);
	    } break;
	    case TabWidget::MUC: {
		    ((MUCWidget *) widget)->setOnline(is_online);
	    } break;
	}
    }
}
