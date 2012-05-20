#include "newcontactwindow.h"
#include "ui_newcontactwindow.h"

NewContactWindow::NewContactWindow(QWidget *parent): QDialog(parent), ui(new Ui::NewContactWindow) {
    ui->setupUi(this);
}

NewContactWindow::~NewContactWindow() {
    delete ui;
}

void NewContactWindow::on_buttonBox_accepted() {
    emit newContactAdditionRequested(ui->contact_jid->text(), ui->contact_name->text());
}

void NewContactWindow::on_contact_jid_textChanged(const QString &arg1) {
    // TODO
}

void NewContactWindow::on_contact_name_textChanged(const QString &arg1) {
    // TODO
}
