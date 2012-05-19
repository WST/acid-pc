#ifndef CHATWINDOW_H
#define CHATWINDOW_H

// Qt
#include <QtGui>

// qxmpp
#include <qxmpp/QXmppMessage.h>

// ACId
#include "tabwidget.h"
#include "chatwidget.h"
#include "mucwidget.h"
#include "messenger.h"
#include "contact_list/contact_item.h"

namespace Ui {
	class ChatWindow;
}

class Messenger;

class ChatWindow: public QDialog
{
    Q_OBJECT

public:
    explicit ChatWindow(Messenger *parent);
    ~ChatWindow();
    void displayMessage(QXmppMessage &message, QString tab_name, CL::ContactItem *roster_item);
    void displayMUCMessage(QXmppMessage &message);
    bool adaTabForJid(QString fulljid);

    ChatWidget *openChatTab(QString fulljid, QString tab_name, CL::ContactItem *roster_item = 0);
    MUCWidget *openMUCTab(QXmppMucRoom *room);

    // Deprecated © WST
    //TabWidget *openTab(QString fulljid, QString tab_name, TabWidget::Type type, CL::ContactItem *roster_item = 0, QXmppMucRoom *room = 0);
    void reloadGeometry(QSettings *settings);
    TabWidget *getWidgetByJid(QString jid);
    TabWidget *getWidgetByIndex(int index);
    ChatWidget *getChatByJid(QString jid);
    MUCWidget *getMUCByJid(QString jid);

private:
    Ui::ChatWindow *ui;
    bool online;
    QSettings *settings;

signals:
    void aboutToSend(QString to, QString message);
    void aboutToSendMUC(QString room, QString message);
    void mucTabClosed(QString room);

public slots:
    void setOnline(bool is_online);

private slots:
    void on_tabWidget_currentChanged(int index);
    void on_tabWidget_tabCloseRequested(int index);
    void chatGeometryChanged(QByteArray);
};

#endif // CHATWINDOW_H
