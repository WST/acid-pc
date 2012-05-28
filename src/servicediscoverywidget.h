#ifndef SERVICEDISCOVERYWIDGET_H
#define SERVICEDISCOVERYWIDGET_H

// ACId
#include "tabwidget.h"

// qxmpp
#include <qxmpp/QXmppDiscoveryManager.h>

namespace Ui {
class ServiceDiscoveryWidget;
}

class ServiceDiscoveryWidget : public TabWidget
{
    Q_OBJECT
    
public:
    explicit ServiceDiscoveryWidget(QXmppDiscoveryManager *manager, QWidget *parent = 0);
    ~ServiceDiscoveryWidget();
    void setOnline(bool is_online);
    
private:
    Ui::ServiceDiscoveryWidget *ui;
    QXmppDiscoveryManager *manager;
};

#endif // SERVICEDISCOVERYWIDGET_H
