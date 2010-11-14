#ifndef CHATWIDGET_H
#define CHATWIDGET_H

// Qt
#include <QWidget>

// qxmpp
#include <QXmppMessage.h>

// ACId
#include "tabwidget.h"

namespace Ui {
    class ChatWidget;
}

class ChatWidget: public TabWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QString with, QWidget *parent = 0);
    ~ChatWidget();
    void insertMessage(QXmppMessage &message);

private:
    Ui::ChatWidget *ui;
};

#endif // CHATWIDGET_H
