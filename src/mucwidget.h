#ifndef MUCWIDGET_H
#define MUCWIDGET_H

// acid
#include "tabwidget.h"

// qxmpp
#include <QXmppMessage.h>

namespace Ui {
    class MUCWidget;
}

class MUCWidget: public TabWidget
{
    Q_OBJECT

public:
    explicit MUCWidget(QString with, QWidget *parent = 0);
    ~MUCWidget();
    void insertMessage(QXmppMessage &message);
    void setOnline(bool is_online);

private:
    Ui::MUCWidget *ui;
};

#endif // MUCWIDGET_H
