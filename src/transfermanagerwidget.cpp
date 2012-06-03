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

    int rows = ui->items->rowCount();
    ui->items->insertRow(rows);

    ui->items->setItem(rows, 0, new QTableWidgetItem(job->fileName()));
    ui->items->setItem(rows, 1, new QTableWidgetItem(job->direction() == QXmppTransferJob::IncomingDirection ? tr("Incoming") : tr("Outgoing")));
    ui->items->setItem(rows, 2, new QTableWidgetItem(QString::number(job->fileSize())));

    ui->items->item(rows, 0)->setData(Qt::UserRole, job->sid());

    // TODO

    connect(job, SIGNAL(stateChanged(QXmppTransferJob::State)), this, SLOT(changeState(QXmppTransferJob::State)));
    connect(job, SIGNAL(progress(qint64, qint64)), this, SLOT(progress(qint64, qint64)));
}

void TransferManagerWidget::setOnline(bool is_online) {
    // TODO
}

void TransferManagerWidget::changeState(QXmppTransferJob::State state) {
    QXmppTransferJob *job = (QXmppTransferJob *) sender();
}

void TransferManagerWidget::progress(qint64 total, qint64 sent) {
    QXmppTransferJob *job = (QXmppTransferJob *) sender();
}
