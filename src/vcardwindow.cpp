#include "vcardwindow.h"
#include "ui_vcardwindow.h"

VcardWindow::VcardWindow(Messenger *parent, const QXmppVCardIq *whose): QDialog(parent), ui(new Ui::VcardWindow) {
	ui->setupUi(this);
	connect(this, SIGNAL(accepted()), this, SLOT(destroy()));

	ui->fullname->setText(whose->fullName());
	ui->nickname->setText(whose->nickName());
    ui->about->document()->setPlainText("");
	ui->birthday->setText(whose->birthday().toString("dd.MM.yyyy"));
	ui->city->setText("");

    setWindowTitle(whose->fullName().isEmpty() ? tr("Viewing profile") : whose->fullName());
    resize(width(), minimumSizeHint().height());
}

VcardWindow::~VcardWindow() {
	delete ui;
}

void VcardWindow::destroy() {
	delete this;
}
