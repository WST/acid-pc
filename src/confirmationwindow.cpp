#include "confirmationwindow.h"
#include "ui_confirmationwindow.h"

#include <QDesktopWidget>

ConfirmationWindow::ConfirmationWindow(QWidget *parent): QWidget(parent), ui(new Ui::ConfirmationWindow) {
    ui->setupUi(this);
	setWindowFlags(Qt::ToolTip | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Desktop);
}

ConfirmationWindow::~ConfirmationWindow() {
    delete ui;
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

void ConfirmationWindow::setEventIcon(QPixmap &icon) {
	ui->icon->setPixmap(icon);
}

void ConfirmationWindow::setType(Type newtype) {
	type = newtype;
}

void ConfirmationWindow::setPointer(QObject *newpointer) {
	pointer = newpointer;
}

void ConfirmationWindow::setTimeout(unsigned short int seconds) {
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(kill()));
	timer->setInterval(seconds * 1000);
	timer->start();
}

ConfirmationWindow *ConfirmationWindow::newMessage(QXmppMessage *message, int timeout) {
	ConfirmationWindow *window = new ConfirmationWindow();
	window->setEventTitle("New chat message");
	window->setEventDescription(message->from());
	window->setType(Message);
	window->show();
	window->setPointer((QObject *) message);
	window->setTimeout(timeout);
	return window;
}

ConfirmationWindow *ConfirmationWindow::newFile(QXmppTransferJob *job, int timeout) {
	// TODO
}

ConfirmationWindow *ConfirmationWindow::newCall(QXmppCall *call, int timeout) {
	// TODO
}

void ConfirmationWindow::on_accept_button_clicked() {
	switch(type) {
		case Message:
			emit confirmedMessage((QXmppMessage *) pointer, true);
		break;
	}
	hide();
	delete this;
}

void ConfirmationWindow::on_decline_button_clicked() {
	switch(type) {
		case Message:
			emit confirmedMessage((QXmppMessage *) pointer, false);
		break;
	}
	hide();
	delete this;
}

void ConfirmationWindow::kill() {
	hide();
	delete this;
}
