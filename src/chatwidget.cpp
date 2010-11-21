#include "chatwidget.h"
#include "ui_chatwidget.h"

ChatWidget::ChatWidget(QString with, QWidget *parent): TabWidget(with, parent), ui(new Ui::ChatWidget) {
    ui->setupUi(this);
    ui->jid->setText(with);
    TabWidget::setType(TabWidget::Chat);
}

ChatWidget::~ChatWidget() {
    delete ui;
}

void ChatWidget::insertMessage(QXmppMessage &message) {
    ui->chatview->append("<font color=\"#AA0000\">&lt;" + message.from() + "&gt;</font>" + message.body());
}

void ChatWidget::on_send_clicked() {
    QString message = ui->message->toPlainText();
    ui->message->clear();
    ui->chatview->append("<me> " + message);
    ui->message->setFocus();
    emit aboutToSend(jid, message);
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
