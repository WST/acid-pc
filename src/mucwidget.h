#ifndef MUCWIDGET_H
#define MUCWIDGET_H

// acid
#include "tabwidget.h"
#include "contact_list/item_model.h"
#include "contact_list/tree_view.h"
#include "contact_list/qxmpp_bridge.h"

// qxmpp
#include <qxmpp/QXmppMessage.h>
#include <qxmpp/QXmppMucManager.h>

namespace Ui {
    class MUCWidget;
}

class MUCWidget: public TabWidget
{
    Q_OBJECT

public:
    explicit MUCWidget(QXmppMucRoom *room, QWidget *parent = 0);
    ~MUCWidget();
    void insertMessage(QXmppMessage &message);
    void setOnline(bool is_online);
    void activate();
    void setIcon(const QString &path);

signals:
    void aboutToSend(QString to, QString message);

private:
    Ui::MUCWidget *ui;
    QXmppMucRoom *m_room;
	
	CL::QXmppBridge *mucBridge;
	CL::ItemModel participantsModel;

private slots:
    void on_send_clicked();
    void on_message_textChanged();
	void openLink(const QUrl &link);
};

#endif // MUCWIDGET_H
