#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "functions.h"

ChatWindow::ChatWindow(Messenger *parent): QDialog(parent), ui(new Ui::ChatWindow) {
	settings = parent->settingsManager();
	ui->setupUi(this);
	setWindowIcon(QIcon(":/acid_16.png"));
}

ChatWindow::~ChatWindow() {
	delete ui;
}

void ChatWindow::displayMessage(QXmppMessage &message, QString tab_name, CL::ContactItem *roster_item) {
    if(!isVisible()) show();

    ChatWidget *widget = getChatByJid(message.from());
    if(widget) return widget->insertMessage(message);

	QStringList jid = parseJid(message.from());

    if(((widget = getChatByJid(jid[1]))) && !jid[5].isEmpty()) {
        widget->appendResource(jid[5]);
        return displayMessage(message, tab_name, roster_item);
    }

    ChatWidget *new_widget = openChatTab(message.from(), tab_name, roster_item);
    new_widget->setNick(tab_name);
    new_widget->insertMessage(message);
}

void ChatWindow::displayMUCMessage(QXmppMessage &message) {
    QStringList jid = parseJid(message.from());
    MUCWidget *widget = getMUCByJid(jid[1]);
    if(widget) widget->insertMessage(message);
    // Если виджета нет, сообщение нахуй не надо
}

TabWidget *ChatWindow::getWidgetByIndex(int index) {
	return (TabWidget *) ui->tabWidget->widget(index);
}

TabWidget *ChatWindow::getWidgetByJid(QString jid) {
	TabWidget *widget;
	for(int i = 0; i < ui->tabWidget->count(); i ++) {
        if((widget = (TabWidget *) ui->tabWidget->widget(i))->getJid() == jid) {
			return widget;
		}
	}
	return 0;
}

ChatWidget *ChatWindow::getChatByJid(QString jid) {
    TabWidget *widget;
    for(int i = 0; i < ui->tabWidget->count(); i ++) {
        if(((widget = (TabWidget *) ui->tabWidget->widget(i))->getJid() == jid) && widget->getType() == TabWidget::Chat) {
            return (ChatWidget *) widget;
        }
    }
    return 0;
}

MUCWidget *ChatWindow::getMUCByJid(QString jid) {
    TabWidget *widget;
    for(int i = 0; i < ui->tabWidget->count(); i ++) {
        if(((widget = (MUCWidget *) ui->tabWidget->widget(i))->getJid() == jid) && widget->getType() == TabWidget::MUC) {
            return (MUCWidget *) widget;
        }
    }
    return 0;
}

bool ChatWindow::adaTabForJid(QString jid) {
	return (bool) getWidgetByJid(jid);
}

ChatWidget *ChatWindow::openChatTab(QString fulljid, QString tab_name, CL::ContactItem *roster_item) {
    if(!isVisible()) show();

    ChatWidget *widget = getChatByJid(fulljid);
    if(widget) return widget;

    widget = new ChatWidget(fulljid, roster_item);
    connect(widget, SIGNAL(aboutToSend(QString,QString)), this, SIGNAL(aboutToSend(QString, QString)));
    connect(widget, SIGNAL(chatGeometryChanged(QByteArray)), this, SLOT(chatGeometryChanged(QByteArray)));

    if(roster_item) {
        QIcon item_icon = roster_item->getIcon();
        ui->tabWidget->addTab(widget, item_icon, tab_name); // TODO update the icon
        widget->setIcon(item_icon);
    } else {
        ui->tabWidget->addTab(widget, QIcon(":/common/chat.png"), tab_name);
        widget->setIcon(QIcon(":/common/chat.png"));
    }

    ui->tabWidget->setCurrentWidget(widget);
    widget->setOnline(online);
    widget->activate();
    return widget;
}

MUCWidget *ChatWindow::openMUCTab(QXmppMucRoom *room) {
    if(!isVisible()) show();

    MUCWidget *widget = getMUCByJid(room->jid());
    if(widget) return widget;

    QStringList jid_parts = parseJid(room->jid());

    widget = new MUCWidget(room);
    connect(widget, SIGNAL(aboutToSend(QString, QString)), this, SIGNAL(aboutToSendMUC(QString, QString)));
    ui->tabWidget->addTab(widget, QIcon(":/common/users.png"), jid_parts[2]);
    ui->tabWidget->setCurrentWidget(widget);
    widget->setOnline(online);
    widget->activate();
    widget->setIcon(":/common/users.png");
    return widget;
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

void ChatWindow::on_tabWidget_tabCloseRequested(int index) {
	TabWidget *widget = getWidgetByIndex(index);
	switch(widget->getType()) {
		case TabWidget::MUC: {
			emit mucTabClosed(widget->getJid());
			delete (MUCWidget *) widget;
		} break;
		case TabWidget::Chat: {
			delete (TabWidget *) widget;
		} break;
	}

	if(ui->tabWidget->count() == 0) {
		hide();
	}
}

void ChatWindow::on_tabWidget_currentChanged(int index) {
	if(ui->tabWidget->count() == 0) {
		return;
	}

	TabWidget *widget = (TabWidget *) ui->tabWidget->widget(index);
	switch(widget->getType()) {
		case TabWidget::Chat: {
			if(settings->contains("chat/geometry")) {
				((ChatWidget *) widget)->setChatGeometry(settings->value("chat/geometry").toByteArray());
			}
			((ChatWidget *) widget)->activate();
		} break;
		case TabWidget::MUC: {
			((MUCWidget *) widget)->activate();
		} break;
	}
}

void ChatWindow::chatGeometryChanged(QByteArray geometry) {
	settings->setValue("chat/geometry", geometry);
}
