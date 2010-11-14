#include "chatwidget.h"
#include "ui_chatwidget.h"

ChatWidget::ChatWidget(QString with, QWidget *parent): TabWidget(with, parent), ui(new Ui::ChatWidget) {
    ui->setupUi(this);
    ui->jid->setText(with);
}

ChatWidget::~ChatWidget() {
    delete ui;
}

void ChatWidget::insertMessage(QXmppMessage &message) {
    ui->chatview->append("<" + message.from() + "> " + message.body());
}
