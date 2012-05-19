#include "joinroomwindow.h"
#include "ui_joinroomwindow.h"

JoinRoomWindow::JoinRoomWindow(QWidget *parent): QDialog(parent), ui(new Ui::JoinRoomWindow) {
    ui->setupUi(this);
    ui->error_message->setVisible(false);
}

JoinRoomWindow::JoinRoomWindow(QString error_message, QString room_jid, QWidget *parent): QDialog(parent), ui(new Ui::JoinRoomWindow) {
    ui->setupUi(this);
    ui->error_message->setText(error_message);
    ui->error_message->setVisible(true);
    ui->room_link->setText(room_jid);
    ui->room_link->setEnabled(false);
}

JoinRoomWindow::~JoinRoomWindow() {
    delete ui;
}

void JoinRoomWindow::on_room_link_textChanged(QString newtext) {
	ui->conference_server->setEnabled(newtext.isEmpty());
	ui->conference_name->setEnabled(newtext.isEmpty());
	ui->conference_name->clear();
	ui->conference_server->clear();
}

void JoinRoomWindow::on_conference_server_textChanged(QString newtext) {
	ui->room_link->clear();
	ui->room_link->setEnabled(newtext.isEmpty());
}

void JoinRoomWindow::on_conference_name_textChanged(QString newtext) {
	ui->room_link->clear();
	ui->room_link->setEnabled(newtext.isEmpty());
}

void JoinRoomWindow::on_buttonBox_accepted() {
    // TODO: всякая валидация
    QString room_address("");

    if(ui->room_link->text().isEmpty()) {
        room_address = ui->conference_name->text() + "@" + ui->conference_server->text();
    } else {
        room_address = ui->room_link->text().replace("xmpp:", "").replace("?join", "");
    }

    emit joinRoomRequested(room_address);
}
