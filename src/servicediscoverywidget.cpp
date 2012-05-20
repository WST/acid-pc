#include "servicediscoverywidget.h"
#include "ui_servicediscoverywidget.h"

ServiceDiscoveryWidget::ServiceDiscoveryWidget(QXmppDiscoveryManager *manager, QWidget *parent): QWidget(parent), ui(new Ui::ServiceDiscoveryWidget) {
    ui->setupUi(this);
    this->manager = manager;
}

ServiceDiscoveryWidget::~ServiceDiscoveryWidget() {
    delete ui;
}
