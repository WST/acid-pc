#include "vcardwindow.h"
#include "ui_vcardwindow.h"

VcardWindow::VcardWindow(Messenger *parent, QString bare_jid): QDialog(parent), ui(new Ui::VcardWindow) {
	ui->setupUi(this);
	connect(this, SIGNAL(accepted()), this, SLOT(destroy()));
	// TODO: разрушать по другим сигналам, потом как-то прикрутить правку собственной vCard
}

VcardWindow::~VcardWindow() {
	delete ui;
}

void VcardWindow::destroy() {
	delete this;
}
