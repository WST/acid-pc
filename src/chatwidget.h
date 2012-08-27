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
    void appendResource(QString _resource);
    void insertMessage(QXmppMessage &message);
    void setOnline(bool is_online);
    void activate();

signals:
    void aboutToSend(QString to, QString message);
    void chatGeometryChanged(QByteArray state);
    void wannaSeeProfile(const QString &jid);
    void wannaSendFile(const QString &jid);
    void wannaMakeCall(const QString &jid);

private slots:
    void on_splitter_splitterMoved(int pos, int index);
    void on_message_textChanged();
    void on_send_clicked();

    void on_view_profile_clicked();
    void on_send_file_clicked();
    void on_voice_call_clicked();
};

#endif // CHATWIDGET_H
