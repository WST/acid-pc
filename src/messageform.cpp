#include "messageform.h"
#include "ui_messageform.h"

MessageForm::MessageForm(QWidget *parent): QDialog(parent), ui(new Ui::MessageForm) {
    ui->setupUi(this);
}

MessageForm::~MessageForm()
{
    delete ui;
}

void MessageForm::on_pushButton_clicked() {
    emit readyToSend(this);
}

void MessageForm::on_pushButton_2_clicked() {
    delete this;
}

const QString MessageForm::jid() {
    return ui->jid->text();
}

const QString MessageForm::body() {
    return ui->body->toPlainText();
}
