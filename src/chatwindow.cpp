#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "functions.h"

/*
В этом файле я несколько раз делаю цикл по всем табам, гораздо лучше было бы написать одну
функцию, обходящую все табы и возвращающую индекс найденного, но уже что есть то есть.
Можно оставить как TODO на этап декостылизации.
*/

ChatWindow::ChatWindow(MainWindow *parent, QSettings *settings): QDialog(parent), ui(new Ui::ChatWindow) {
    this->settings = settings;
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
	widget = (TabWidget *) ui->tabWidget->widget(i);
	switch(widget->getType()) {
	    case TabWidget::Chat: {
		    if(widget->getJid() == fulljid) {
			return true;
		    }
	    } break;
	    case TabWidget::MUC: {
		    QStringList jid = parseJid(fulljid);
		    if(widget->getJid() == jid[1]) {
			return true;
		    }
	    } break;
	}
    }
    return false;
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
		widget->setOnline(online);
		ui->tabWidget->setCurrentWidget(widget);
		widget->activate();
	} break;
	case TabWidget::MUC: {
		QStringList jid = parseJid(fulljid);
		MUCWidget *widget = new MUCWidget(jid[1]);
		// TODO: connect(widget, SIGNAL(aboutToSend(QString,QString)), this, SIGNAL(aboutToSendMUC(QString, QString)));
		ui->tabWidget->addTab(widget, "MUC");
		widget->setOnline(online);
		ui->tabWidget->setCurrentWidget(widget);
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

    if(((TabWidget *) ui->tabWidget->widget(index))->getType() == TabWidget::MUC) {
	// TODO выход из комнаты
    }

    delete ui->tabWidget->widget(index); // таб при этом удаляется сам, во как хитро

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
