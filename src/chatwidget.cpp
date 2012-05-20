#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "version.h"

ChatWidget::ChatWidget(QString with, CL::ContactItem *roster_item, QWidget *parent): nick(""), TabWidget(with, parent), ui(new Ui::ChatWidget) {
    ui->setupUi(this);
    ui->jid->setText(with);
    TabWidget::setType(TabWidget::Chat);
    m_item = roster_item;
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
    ui->chatview->append("<font color=\"#AA0000\">&lt;" + nick + "&gt;</font> " + text);
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
    ui->send->setToolTip(online ? "Send the message" : "This button is not available because you are not connected");
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
}

void ChatWidget::appendResource(QString resource) {
    jid += "/";
    jid += resource;
    ui->jid->setText(ui->jid->text() + "/" + resource);
}

void ChatWidget::setChatGeometry(QByteArray state) {
    ui->splitter->restoreState(state);
}

void ChatWidget::on_splitter_splitterMoved(int pos, int index) {
    emit chatGeometryChanged(ui->splitter->saveState());
}
