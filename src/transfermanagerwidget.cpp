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

    connect(job, SIGNAL(stateChanged(QXmppTransferJob::State)), this, SLOT(changeState(QXmppTransferJob::State)));
    connect(job, SIGNAL(progress(qint64, qint64)), this, SLOT(progress(qint64, qint64)));
}

void TransferManagerWidget::setOnline(bool is_online) {
    // TODO
}
