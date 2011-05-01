#ifndef CONFIRMATIONWINDOW_H
#define CONFIRMATIONWINDOW_H

#include <QtGui/QWidget>

#include <QXmppMessage.h>
#include <QXmppTransferManager.h>
#include <QXmppCallManager.h>

namespace Ui {
    class ConfirmationWindow;
}

class ConfirmationWindow: public QWidget
{
    Q_OBJECT

public:
    explicit ConfirmationWindow(QWidget *parent = 0);
    ~ConfirmationWindow();
	void show();
	void setEventTitle(const QString &title);
	void setEventDescription(const QString &description);
	void setEventIcon(QPixmap &icon);

	static ConfirmationWindow *newMessage(QXmppMessage *message);
	static ConfirmationWindow *newFile(QXmppTransferJob *job);
	static ConfirmationWindow *newCall(QXmppCall *call);

private:
    Ui::ConfirmationWindow *ui;

signals:
	void confirmedFile(QXmppTransferJob *job, bool accepted);
	void confirmedMessage(QXmppMessage *message, bool accepted);
	void confirmedCall(QXmppCall *call, bool accepted);
	//void confirmedMUCInvitation(const QString &message_id, bool accepted);

};

#endif // CONFIRMATIONWINDOW_H
