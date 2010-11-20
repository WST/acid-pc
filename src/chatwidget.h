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
    void setOnline(bool is_online);
    void activate();
    void setChatGeometry(QByteArray geometry);

private:
    Ui::ChatWidget *ui;

signals:
    void aboutToSend(QString to, QString message);
    void chatGeometryChanged(QByteArray state);

private slots:
    void on_splitter_splitterMoved(int pos, int index);
    void on_message_textChanged();
    void on_send_clicked();
};

#endif // CHATWIDGET_H
