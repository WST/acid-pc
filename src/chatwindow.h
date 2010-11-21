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
    explicit ChatWindow(MainWindow *parent, QSettings *settings);
    ~ChatWindow();
    void displayMessage(QXmppMessage &message);
    void displayMUCMessage(QXmppMessage &message);
    bool adaTabForJid(QString fulljid);
    void openTab(QString fulljid, TabWidget::Type type);
    void reloadGeometry(QSettings *settings);
    TabWidget *getWidgetByJid(QString jid);

private:
    Ui::ChatWindow *ui;
    bool online;
    MainWindow *main;
    QSettings *settings;

signals:
    void aboutToSend(QString to, QString message);
    void aboutToSendMUC(QString room, QString message);

public slots:
    void setOnline(bool is_online);

private slots:
    void on_tabWidget_currentChanged(int index);
    void on_tabWidget_tabCloseRequested(int index);
    void chatGeometryChanged(QByteArray);
};

#endif // CHATWINDOW_H
