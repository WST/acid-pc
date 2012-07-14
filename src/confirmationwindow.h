#ifndef CONFIRMATIONWINDOW_H
#define CONFIRMATIONWINDOW_H

#include <QtGui>

#include <qxmpp/QXmppMessage.h>
#include <qxmpp/QXmppTransferManager.h>
#include <qxmpp/QXmppCallManager.h>

namespace Ui {
    class ConfirmationWindow;
}

class ConfirmationWindow: public QWidget
{
    Q_OBJECT

public:
	enum Type {
        Message, Transfer, MUCInvitation, VoiceCall, Registration, Subscription, Unsubscribed
	};

    explicit ConfirmationWindow(QWidget *parent = 0);
    ~ConfirmationWindow();
	void show();
	void setEventTitle(const QString &title);
	void setEventDescription(const QString &description);
	void setEventIcon(QPixmap icon);
	void setType(Type newtype);
	void setPointer(QObject *newpointer);
    void setString(const QString &some_value);
	void setTimeout(unsigned short int seconds);
	void disableDeclineButton();
    void setMessageInfo(const QString &new_id, const QString &new_from);

	static ConfirmationWindow *newMessage(QXmppMessage *message, int timeout = 5);
	static ConfirmationWindow *newFile(QXmppTransferJob *job, int timeout = 5);
	static ConfirmationWindow *newCall(QXmppCall *call, int timeout = 5);
    static ConfirmationWindow *confirmRegistration();
    static ConfirmationWindow *confirmSubscriptionRequest(const QString &jid);
    static ConfirmationWindow *unsubscribedByUser(const QString &jid);

private:
    Ui::ConfirmationWindow *ui;
	Type type;
	QObject *pointer;
	QTimer *timer;
	QString message_id;
    QString message_from;
    QString string_data;

signals:
	void confirmedFile(QXmppTransferJob *job, bool accepted);
	void confirmedMessage(const QString &message_id);
	void confirmedCall(QXmppCall *call, bool accepted);
    void confirmedRegistration(bool mau_register);
    void confirmedSubscription(const QString &jid, bool accepted);

private slots:
	void on_accept_button_clicked();
	void on_decline_button_clicked();
	void kill();
};

#endif // CONFIRMATIONWINDOW_H
