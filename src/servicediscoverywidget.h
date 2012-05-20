#ifndef SERVICEDISCOVERYWIDGET_H
#define SERVICEDISCOVERYWIDGET_H

#include <QWidget>

#include <qxmpp/QXmppDiscoveryManager.h>

namespace Ui {
class ServiceDiscoveryWidget;
}

class ServiceDiscoveryWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ServiceDiscoveryWidget(QXmppDiscoveryManager *manager, QWidget *parent = 0);
    ~ServiceDiscoveryWidget();
    
private:
    Ui::ServiceDiscoveryWidget *ui;
    QXmppDiscoveryManager *manager;
};

#endif // SERVICEDISCOVERYWIDGET_H
