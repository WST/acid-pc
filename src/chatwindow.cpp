#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "functions.h"

ChatWindow::ChatWindow(Messenger *parent): QDialog(parent), ui(new Ui::ChatWindow) {
	settings = parent->settingsManager();
	ui->setupUi(this);
}

ChatWindow::~ChatWindow() {
	delete ui;
}

void ChatWindow::displayMessage(QXmppMessage &message, QString tab_name, CL::ContactItem *roster_item) {
    if(!isVisible()) show();

    if(!isActiveWindow()) {
        qApp->alert(this, 0);
    }

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

ServiceDiscoveryWidget *ChatWindow::getServiceBrowserByJid(QString jid) {
    TabWidget *widget;
    for(int i = 0; i < ui->tabWidget->count(); i ++) {
        if(((widget = (TabWidget *) ui->tabWidget->widget(i))->getJid() == jid) && widget->getType() == TabWidget::ServiceDiscovery) {
            return (ServiceDiscoveryWidget *) widget;
        }
    }
    return 0;
}

bool ChatWindow::adaTabForJid(QString jid) {
	return (bool) getWidgetByJid(jid);
}

ServiceDiscoveryWidget *ChatWindow::openDiscoTab(QXmppDiscoveryManager *manager, const QString &jid) {
    if(!isVisible()) show();

    ServiceDiscoveryWidget *widget = getServiceBrowserByJid(jid);
    if(widget) {
        ui->tabWidget->setCurrentWidget(widget);
        return widget;
    }

    widget = new ServiceDiscoveryWidget(manager, this);
    ui->tabWidget->addTab(widget, QIcon(":/menu/plug.png"), tr("Services"));
    ui->tabWidget->setCurrentWidget(widget);
    return widget;
}

ChatWidget *ChatWindow::openChatTab(QString fulljid, QString tab_name, CL::ContactItem *roster_item) {
    if(!isVisible()) show();

    ChatWidget *widget = getChatByJid(fulljid);
    if(widget) {
        ui->tabWidget->setCurrentWidget(widget);
        return widget;
    }

    widget = new ChatWidget(fulljid, this, roster_item);
    connect(widget, SIGNAL(aboutToSend(QString,QString)), this, SIGNAL(aboutToSend(QString, QString)));
    connect(widget, SIGNAL(chatGeometryChanged(QByteArray)), this, SLOT(chatGeometryChanged(QByteArray)));

    int position = 0;

    if(roster_item) {
        QIcon item_icon = roster_item->getIcon();
        position = ui->tabWidget->addTab(widget, item_icon, tab_name);
        widget->setNumber(position);
        widget->setIcon(item_icon);
    } else {
        position = ui->tabWidget->addTab(widget, tab_name);
        widget->setNumber(position);
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
    if(widget) {
        ui->tabWidget->setCurrentWidget(widget);
        return widget;
    }

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

BookmarksWidget *ChatWindow::getBookmarksEditor() {
    TabWidget *widget;
    for(int i = 0; i < ui->tabWidget->count(); i ++) {
        if((widget = (TabWidget *) ui->tabWidget->widget(i))->getType() == TabWidget::Bookmarks) {
            return (BookmarksWidget *) widget;
        }
    }
    return 0;
}

TransferManagerWidget *ChatWindow::getTransferManager() {
    TabWidget *widget;
    for(int i = 0; i < ui->tabWidget->count(); i ++) {
        if((widget = (TabWidget *) ui->tabWidget->widget(i))->getType() == TabWidget::Transfers) {
            return (TransferManagerWidget *) widget;
        }
    }
    return 0;
}

TransferManagerWidget *ChatWindow::openTransferManager(QXmppTransferJob *job) {
    if(!isVisible()) show();

    TransferManagerWidget *widget = getTransferManager();

    if(widget) {
        ui->tabWidget->setCurrentWidget(widget);
        return widget;
    }

    widget = new TransferManagerWidget(this);
    ui->tabWidget->addTab(widget, QIcon(":/menu/disk.png"), tr("Transfers"));
    ui->tabWidget->setCurrentWidget(widget);
    widget->newTransferJob(job);

    return widget;
}

BookmarksWidget *ChatWindow::openBookmarksEditor(QXmppBookmarkManager *manager) {
    if(!isVisible()) show();

    BookmarksWidget *widget = getBookmarksEditor();

    if(widget) {
        ui->tabWidget->setCurrentWidget(widget);
        return widget;
    }

    widget = new BookmarksWidget(manager, this);
    ui->tabWidget->addTab(widget, QIcon(":/menu/bookmarks.png"), tr("Bookmarks"));
    ui->tabWidget->setCurrentWidget(widget);

    return widget;
}

void ChatWindow::setTabIcon(int position, const QIcon &icon) {
    ui->tabWidget->setTabIcon(position, icon);
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
            case TabWidget::Bookmarks: {
                ((BookmarksWidget *) widget)->setOnline(is_online);
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
            delete (ChatWidget *) widget;
		} break;
        case TabWidget::ServiceDiscovery: {
            delete (ServiceDiscoveryWidget *) widget;
        } break;
        case TabWidget::Bookmarks: {
            delete (BookmarksWidget *) widget;
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

