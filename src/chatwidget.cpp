#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "version.h"

ChatWidget::ChatWidget(QString with, ChatWindow *parent, CL::ContactItem *roster_item): TabWidget(with, parent), ui(new Ui::ChatWidget) {
    ui->setupUi(this);
    ui->jid->setText(with);
    TabWidget::setType(TabWidget::Chat);
    m_item = roster_item;
    window = parent;
    nick = roster_item->getNick();
    connect(roster_item, SIGNAL(iconChanged(const QIcon &)), this, SLOT(setIcon(const QIcon &)));
    connect(roster_item, SIGNAL(nickChanged(const QString &)), this, SLOT(setNick(const QString &)));
}

ChatWidget::~ChatWidget() {
    delete ui;
}

void ChatWidget::insertMessage(QXmppMessage &message) {
	QString text = message.body();
	text.replace("&", "&amp;");
	text.replace("<", "&lt;");
	text.replace("<", "&gt;");
	text.replace("\"", "&quot;");
	text.replace("\n", "<br />");
	text.replace(HYPERLINK_REPLACE_ARGS);
    ui->chatview->append("<font color=\"#0000AA\">&lt;" + nick + "&gt;</font> " + text);
}

void ChatWidget::on_send_clicked() {
	QString text = ui->message->toPlainText();
	emit aboutToSend(jid, text);
	text.replace("&", "&amp;");
	text.replace("<", "&lt;");
	text.replace("<", "&gt;");
	text.replace("\"", "&quot;");
	text.replace("\n", "<br />");
	text.replace(HYPERLINK_REPLACE_ARGS);
    ui->message->clear();
    ui->chatview->append("<font color=\"#AA0000\">&lt;me&gt;</font> " + text);
    ui->message->setFocus();
}

void ChatWidget::on_message_textChanged() {
    ui->send->setEnabled(online && !ui->message->toPlainText().isEmpty());
}

void ChatWidget::setOnline(bool is_online) {
    ui->send->setEnabled((online = is_online) && !ui->message->toPlainText().isEmpty());
    ui->send->setToolTip(online ? tr("Send the message") : tr("This button is not available because you are not connected"));
    if(!is_online) {
        setIcon(QIcon(":/trayicon/offline-16px.png"));
    }
}

void ChatWidget::activate() {
    // просто активирует поле ввода для удобства
    ui->message->setFocus();
}

void ChatWidget::setNick(QString newnick) {
    nick = newnick;
}

void ChatWidget::setIcon(QIcon icon) {
    ui->icon->setPixmap(icon.pixmap(16));
    window->setTabIcon(this, icon);
}

void ChatWidget::appendResource(QString resource) {
    jid += "/";
    jid += resource.toLower();
    ui->jid->setText(ui->jid->text() + "/" + resource);
}

void ChatWidget::setChatGeometry(QByteArray state) {
    ui->splitter->restoreState(state);
}

void ChatWidget::on_splitter_splitterMoved(int pos, int index) {
    emit chatGeometryChanged(ui->splitter->saveState());
}
