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
    online = is_online;
}
