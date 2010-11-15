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
#include "mainwindow.h"

namespace Ui {
    class ChatWindow;
}

class ChatWindow: public QDialog
{
    Q_OBJECT

public:
    explicit ChatWindow(MainWindow *parent);
    ~ChatWindow();
    void displayMessage(QXmppMessage &message);
    bool adaTabForJid(QString fulljid);
    void openTab(QString fulljid, TabWidget::Type type);

private:
    Ui::ChatWindow *ui;
    bool online;
    MainWindow *main;

signals:
    void aboutToSend(QString to, QString message);

public slots:
    void setOnline(bool is_online);
};

#endif // CHATWINDOW_H
