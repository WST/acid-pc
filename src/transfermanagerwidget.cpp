#include "transfermanagerwidget.h"
#include "ui_transfermanagerwidget.h"

TransferManagerWidget::TransferManagerWidget(QWidget *parent): TabWidget("", parent), ui(new Ui::TransferManagerWidget) {
    ui->setupUi(this);
}

TransferManagerWidget::~TransferManagerWidget() {
    delete ui;
}
