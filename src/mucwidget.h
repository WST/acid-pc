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
    void activate();

private:
    Ui::MUCWidget *ui;

private slots:
    void on_message_textChanged();
};

#endif // MUCWIDGET_H
