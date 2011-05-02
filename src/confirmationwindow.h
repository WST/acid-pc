#ifndef CONFIRMATIONWINDOW_H
#define CONFIRMATIONWINDOW_H

#include <QtGui>

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
	enum Type {
		File, Message, Transfer, MUCInvitation
	};

    explicit ConfirmationWindow(QWidget *parent = 0);
    ~ConfirmationWindow();
	void show();
	void setEventTitle(const QString &title);
	void setEventDescription(const QString &description);
	void setEventIcon(QPixmap icon);
	void setType(Type newtype);
	void setPointer(QObject *newpointer);
	void setTimeout(unsigned short int seconds);
	void disableDeclineButton();
	void setMessageId(const QString &new_id);

	static ConfirmationWindow *newMessage(QXmppMessage *message, int timeout = 5);
	static ConfirmationWindow *newFile(QXmppTransferJob *job, int timeout = 5);
	static ConfirmationWindow *newCall(QXmppCall *call, int timeout = 5);

private:
    Ui::ConfirmationWindow *ui;
	Type type;
	QObject *pointer;
	QTimer *timer;
	QString message_id;

signals:
	void confirmedFile(QXmppTransferJob *job, bool accepted);
	void confirmedMessage(const QString &message_id);
	void confirmedCall(QXmppCall *call, bool accepted);

private slots:
	void on_accept_button_clicked();
	void on_decline_button_clicked();
	void kill();
};

#endif // CONFIRMATIONWINDOW_H
