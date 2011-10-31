#ifndef CHATWINDOW_H
#define CHATWINDOW_H

// Qt
#include <QtGui>

// qxmpp
#include <QXmppMessage.h>

// ACId
#include "tabwidget.h"
#include "chatwidget.h"
#include "mucwidget.h"
#include "messenger.h"

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
	void displayMessage(QXmppMessage &message, QString tab_name);
    void displayMUCMessage(QXmppMessage &message);
    bool adaTabForJid(QString fulljid);
	TabWidget *openTab(QString fulljid, QString tab_name, TabWidget::Type type);
    void reloadGeometry(QSettings *settings);
    TabWidget *getWidgetByJid(QString jid);
    TabWidget *getWidgetByIndex(int index);

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
