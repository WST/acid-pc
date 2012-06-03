#ifndef CHATWIDGET_H
#define CHATWIDGET_H

// Qt
#include <QWidget>

// qxmpp
#include <qxmpp/QXmppMessage.h>

// ACId
#include "tabwidget.h"
#include "chatwindow.h"
#include "contact_list/contact_item.h"


namespace Ui {
    class ChatWidget;
}

class ChatWindow;

class ChatWidget: public TabWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QString with, ChatWindow *parent, CL::ContactItem *roster_item = 0);
    ~ChatWidget();
    void setChatGeometry(QByteArray geometry);

private:
    Ui::ChatWidget *ui;
    QString nick;
    CL::ContactItem *m_item;
    ChatWindow *window;

public slots:
    void setNick(QString newnick);
    void setIcon(QIcon icon);
    void appendResource(QString resource);
    void insertMessage(QXmppMessage &message);
    void setOnline(bool is_online);
    void activate();

signals:
    void aboutToSend(QString to, QString message);
    void chatGeometryChanged(QByteArray state);

private slots:
    void on_splitter_splitterMoved(int pos, int index);
    void on_message_textChanged();
    void on_send_clicked();

};

#endif // CHATWIDGET_H
