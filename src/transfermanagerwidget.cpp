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
    ui->items->setItem(rows, 3, new QTableWidgetItem("0 kbps"));
    ui->items->setItem(rows, 4, new QTableWidgetItem("0%"));

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
    int row = rowBySid(job->sid());
    switch(state) {
        case QXmppTransferJob::FinishedState: {
            ui->items->item(row, 4)->setText(tr("Finished"));
        } break;
    }
}

void TransferManagerWidget::progress(qint64 sent, qint64 total) {
    QXmppTransferJob *job = (QXmppTransferJob *) sender();
    int row = rowBySid(job->sid());
    ui->items->item(row, 3)->setText(QString::number(job->speed()));
    ui->items->item(row, 4)->setText(QString::number(trunc(((double) sent / total) * 100.0)) + QString("%"));
}

int TransferManagerWidget::rowBySid(const QString &sid) {
    int rows = ui->items->rowCount();
    for(int i = 0; i < rows; i ++) {
        if(ui->items->item(i, 0)->data(Qt::UserRole).toString() == sid) return i;
    }
    return -1;
}
