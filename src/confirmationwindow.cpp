#include "confirmationwindow.h"
#include "ui_confirmationwindow.h"

ConfirmationWindow::ConfirmationWindow(QWidget *parent): QWidget(parent), ui(new Ui::ConfirmationWindow) {
    ui->setupUi(this);
}

ConfirmationWindow::~ConfirmationWindow() {
    delete ui;
}

void ConfirmationWindow::show() {
	// TODO: указать позицию
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

ConfirmationWindow *ConfirmationWindow::newMessage(QXmppMessage *message) {
	ConfirmationWindow *window = new ConfirmationWindow();
	window->setEventTitle("New chat message");
	window->setEventDescription(message->from());
	return window;
}

ConfirmationWindow *ConfirmationWindow::newFile(QXmppTransferJob *job) {
	// TODO
}

ConfirmationWindow *ConfirmationWindow::newCall(QXmppCall *call) {
	// TODO
}
