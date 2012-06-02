#include "transfermanagerwidget.h"
#include "ui_transfermanagerwidget.h"

TransferManagerWidget::TransferManagerWidget(QWidget *parent): TabWidget("", parent), ui(new Ui::TransferManagerWidget) {
    ui->setupUi(this);
    setType(Transfers);
}

TransferManagerWidget::~TransferManagerWidget() {
    delete ui;
}

void TransferManagerWidget::newTransferJob(QXmppTransferJob *job) {
    // TODO
}

void TransferManagerWidget::setOnline(bool is_online) {
    // TODO
}
