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
    ui->chatview->append("<" + message.from() + "> " + message.body());
}

void ChatWidget::on_send_clicked() {
    QString message = ui->message->toPlainText();
    ui->message->clear();
    ui->chatview->append("<me> " + message);
    emit aboutToSend(jid, message);
}

void ChatWidget::on_message_textChanged() {
    ui->send->setEnabled(online && !ui->message->toPlainText().isEmpty());
}

void ChatWidget::setOnline(bool is_online) {
    ui->send->setEnabled(online = is_online);
}
