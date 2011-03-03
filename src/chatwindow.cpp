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

void ChatWindow::displayMessage(QXmppMessage &message) {
	if(!isVisible()) {
		show();
	}

	TabWidget *widget = getWidgetByJid(message.from());
	if(widget) {
		return ((ChatWidget *) widget)->insertMessage(message);
	}

	QStringList jid = parseJid(message.from());

	if(((widget = getWidgetByJid(jid[1])) != 0) && !jid[5].isEmpty()) {
		((ChatWidget *) widget)->appendResource(jid[5]);
		return displayMessage(message); // NOTE: рекурсия
	}

	return openTab(message.from(), TabWidget::Chat);
}

void ChatWindow::displayMUCMessage(QXmppMessage &message) {
	QStringList jid = parseJid(message.from());
	TabWidget *widget = getWidgetByJid(jid[1]);
	if(widget && widget->getType() == TabWidget::MUC) {
		((MUCWidget *) widget)->insertMessage(message);
	}
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

bool ChatWindow::adaTabForJid(QString jid) {
	//TabWidget *widget;
	//for(int i = 0; i < ui->tabWidget->count(); i ++) {
		//if(((TabWidget *) ui->tabWidget->widget(i))->getJid() == jid) {
			return (bool) getWidgetByJid(jid);
		//}
	//}
	//return false;
}

void ChatWindow::openTab(QString fulljid, TabWidget::Type type) {
	if(!isVisible()) {
		show();
	}

	if(adaTabForJid(fulljid)) {
		// TODO: активировать требуемую вкладку, она уже открыта
		return;
	}

	switch(type) {
		case TabWidget::Chat: {
			ChatWidget *widget = new ChatWidget(fulljid);
			connect(widget, SIGNAL(aboutToSend(QString,QString)), this, SIGNAL(aboutToSend(QString, QString)));
			connect(widget, SIGNAL(chatGeometryChanged(QByteArray)), this, SLOT(chatGeometryChanged(QByteArray)));
			ui->tabWidget->addTab(widget, "tab");
			ui->tabWidget->setCurrentWidget(widget);
			widget->setOnline(online);
			widget->activate();
		} break;
		case TabWidget::MUC: {
			// NOTE: при входе в MUC fulljid является bare JID комнаты!
			MUCWidget *widget = new MUCWidget(fulljid);
			connect(widget, SIGNAL(aboutToSend(QString, QString)), this, SIGNAL(aboutToSendMUC(QString, QString)));
			ui->tabWidget->addTab(widget, "MUC");
			ui->tabWidget->setCurrentWidget(widget);
			widget->setOnline(online);
			widget->activate();
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
