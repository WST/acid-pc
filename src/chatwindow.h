#ifndef CHATWINDOW_H
#define CHATWINDOW_H

// Qt
#include <QWidget>

// qxmpp
#include <QXmppMessage.h>

// ACId
#include "tabwidget.h"
#include "chatwidget.h"
#include "mucwidget.h"

namespace Ui {
    class ChatWindow;
}

class ChatWindow: public QDialog
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = 0);
    ~ChatWindow();
    void displayMessage(QXmppMessage &message);
    bool adaTabForJid(QString fulljid);
    void openTab(QString fulljid, TabWidget::Type type);

private:
    Ui::ChatWindow *ui;
    bool online;

signals:
    void aboutToSend(QString to, QString message);

public slots:
    void setOnline(bool is_online);
};

#endif // CHATWINDOW_H
