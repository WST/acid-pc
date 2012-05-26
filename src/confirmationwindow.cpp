#include "confirmationwindow.h"
#include "ui_confirmationwindow.h"

#include <QDesktopWidget>

ConfirmationWindow::ConfirmationWindow(QWidget *parent): QWidget(parent), ui(new Ui::ConfirmationWindow) {
    ui->setupUi(this);
	setWindowFlags(Qt::ToolTip | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Desktop);
    timer = 0;
    ui->actions_button->setVisible(false); // По умолчанию не нужна
}

ConfirmationWindow::~ConfirmationWindow() {
    delete ui;
    delete timer; // NOTE: таймер не используется при передачах файлов
}

void ConfirmationWindow::show() {
	// TODO: указать позицию
	setGeometry(qApp->desktop()->availableGeometry().right() - width(), qApp->desktop()->availableGeometry().bottom() - height(), width(), height());
	QWidget::show();
}

void ConfirmationWindow::setEventTitle(const QString &title) {
	ui->event_title->setText(title);
}

void ConfirmationWindow::setEventDescription(const QString &description) {
	ui->event_description->setText(description);
}

void ConfirmationWindow::setEventIcon(QPixmap icon) {
	ui->icon->setPixmap(icon);
}

void ConfirmationWindow::setType(Type newtype) {
	type = newtype;
}

void ConfirmationWindow::setPointer(QObject *newpointer) {
	pointer = newpointer;
}

void ConfirmationWindow::setString(const QString &some_value) {
    string_data = some_value;
}

void ConfirmationWindow::setTimeout(unsigned short int seconds) {
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(kill()));
	timer->setInterval(seconds * 1000);
	timer->start();
}

void ConfirmationWindow::setMessageInfo(const QString &new_id, const QString &new_from) {
	message_id = new_id;
    message_from = new_from;
}

void ConfirmationWindow::disableDeclineButton() {
	ui->decline_button->setVisible(false);
}

ConfirmationWindow *ConfirmationWindow::newMessage(QXmppMessage *message, int timeout) {
	ConfirmationWindow *window = new ConfirmationWindow();
	window->setEventTitle("New chat message");
	window->setEventDescription(message->from());
	window->setType(Message);
	window->setEventIcon(QPixmap(":/notifications/message.png"));
	window->show();
    window->setMessageInfo(message->id(), message->from());
	window->setTimeout(timeout);
	window->disableDeclineButton(); // неактуально для сообщений, т/к они «уже» пришли
	return window;
}

ConfirmationWindow *ConfirmationWindow::newFile(QXmppTransferJob *job, int timeout) {
	ConfirmationWindow *window = new ConfirmationWindow();
	window->setEventTitle("New incoming file");
	window->setEventDescription(QString("Name: ") + job->fileName() + QString(", size: ") + QString::number(job->fileSize()));
	window->setType(Transfer);
	window->setEventIcon(QPixmap(":/notifications/file.png"));
	window->setPointer((QObject *) job);
	window->show();
	Q_UNUSED(timeout);
	return window;
}

ConfirmationWindow *ConfirmationWindow::newCall(QXmppCall *call, int timeout) {
	ConfirmationWindow *window = new ConfirmationWindow();
	window->setEventTitle("New incoming voice call");
	window->setEventDescription("foo bar");
	window->setType(VoiceCall);
	window->setEventIcon(QPixmap(":/notifications/call.png"));
	window->setPointer((QObject *) call);
	window->show();
	//window->setTimeout(timeout);
	Q_UNUSED(timeout);
	return window;
}

ConfirmationWindow *ConfirmationWindow::confirmRegistration() {
    ConfirmationWindow *window = new ConfirmationWindow();
    window->setEventTitle("Failed to log in. Register this account?");
    window->setEventDescription("If you click <accept> here, an attempt to register new account will be made");
    window->setType(Registration);
    window->setEventIcon(QPixmap(":/notifications/information.png"));
    window->setPointer(0);
    window->show();

    return window;
}

ConfirmationWindow *ConfirmationWindow::confirmSubscriptionRequest(const QString &jid) {
    ConfirmationWindow *window = new ConfirmationWindow();
    window->setEventTitle(jid);
    window->setEventDescription("This user wants to see your online status. Accept?");
    window->setType(Subscription);
    window->setEventIcon(QPixmap(":/notifications/information.png"));
    window->setString(jid);
    window->show();

    return window;
}

void ConfirmationWindow::on_accept_button_clicked() {
	switch(type) {
        case Message: emit confirmedMessage(message_from); break;
        case Transfer: emit confirmedFile((QXmppTransferJob *) pointer, true); break;
        case MUCInvitation: break; // TODO
        case VoiceCall: emit confirmedCall((QXmppCall *) pointer, false); break;
        case Registration: emit confirmedRegistration(true); break;
        case Subscription: emit confirmedSubscription(string_data, true); break;
	}
	hide();
	delete this;
}

void ConfirmationWindow::on_decline_button_clicked() {
	switch(type) {
		case Message: break;
        case Transfer: emit confirmedFile((QXmppTransferJob *) pointer, false); break;  
        case MUCInvitation: break;
        case VoiceCall: emit confirmedCall((QXmppCall *) pointer, false); break;
        case Registration: emit confirmedRegistration(false); break;
        case Subscription: emit confirmedSubscription(string_data, false); break;
	}
	hide();
	delete this;
}

void ConfirmationWindow::kill() {
	hide();
	delete this;
}
