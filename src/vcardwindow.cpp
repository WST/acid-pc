#include "vcardwindow.h"
#include "ui_vcardwindow.h"

VcardWindow::VcardWindow(Messenger *parent, const QXmppVCard *whose): QDialog(parent), ui(new Ui::VcardWindow) {
	ui->setupUi(this);
	connect(this, SIGNAL(accepted()), this, SLOT(destroy()));

	ui->fullname->setText(whose->fullName());
}

VcardWindow::~VcardWindow() {
	delete ui;
}

void VcardWindow::destroy() {
	delete this;
}
