#include "servicediscoverywidget.h"
#include "ui_servicediscoverywidget.h"

ServiceDiscoveryWidget::ServiceDiscoveryWidget(QXmppDiscoveryManager *manager, QWidget *parent): TabWidget("", parent), ui(new Ui::ServiceDiscoveryWidget) {
    ui->setupUi(this);
    this->manager = manager;
    TabWidget::setType(TabWidget::ServiceDiscovery);
}

ServiceDiscoveryWidget::~ServiceDiscoveryWidget() {
    delete ui;
}

void ServiceDiscoveryWidget::setOnline(bool is_online) {
    // TODO
}
