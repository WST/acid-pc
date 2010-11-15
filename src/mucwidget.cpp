#include "mucwidget.h"
#include "ui_mucwidget.h"

MUCWidget::MUCWidget(QString with, QWidget *parent): TabWidget(with, parent), ui(new Ui::MUCWidget) {
    ui->setupUi(this);
    TabWidget::setType(TabWidget::MUC);
}

MUCWidget::~MUCWidget() {
    delete ui;
}

void MUCWidget::insertMessage(QXmppMessage &message) {
    // TODO
}

void MUCWidget::setOnline(bool is_online) {
    ui->send->setEnabled((online = is_online) && !ui->message->toPlainText().isEmpty());
    ui->send->setToolTip(online ? "Send the message" : "This button is not available because you are not connected");
}

void MUCWidget::on_message_textChanged() {
    ui->send->setEnabled(online && !ui->message->toPlainText().isEmpty());
}
