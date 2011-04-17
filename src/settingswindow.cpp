#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(Messenger *parent): QDialog(parent), ui(new Ui::SettingsWindow) {
	ui->setupUi(this);
	settings = parent->settingsManager();
	loadCurrentSettings();
}

SettingsWindow::~SettingsWindow() {
	delete ui;
}

void SettingsWindow::on_file_transfer_mode_currentIndexChanged(int index) {
	ui->file_transfer_proxy->setEnabled(index == 0);
}

void SettingsWindow::on_button_box_accepted() {
	settings->setValue("settings/automatically_accept_calls", ui->automatically_accept_calls->checkState());
	settings->setValue("settings/automatically_accept_files", ui->automatically_accept_files->checkState());
	settings->setValue("settings/automatically_open_new_tabs", ui->automatically_open_new_tabs->checkState());
	settings->setValue("settings/file_transfer_mode", ui->file_transfer_mode->currentIndex());
	settings->setValue("settings/file_transfer_proxy", ui->file_transfer_proxy->text());
	settings->setValue("settings/gui_style", ui->gui_style->currentIndex());
	settings->setValue("settings/muc_nickname", ui->muc_nickname->text());
	settings->setValue("settings/keepalive_interval", ui->keepalive_interval->value());
	settings->setValue("settings/keepalive_timeout", ui->keepalive_timeout->value());

	emit modified();
}

void SettingsWindow::loadCurrentSettings() {
	//ui->file_transfer_proxy->setEnabled(ui->file_transfer_mode->currentIndex() == 0);
	ui->gui_style->setCurrentIndex(settings->value("settings/gui_style", 0).toInt());
	ui->keepalive_interval->setValue(settings->value("settings/keepalive_interval", 60).toInt());
	ui->keepalive_timeout->setValue(settings->value("settings/keepalive_timeout", 30).toInt());
}

void SettingsWindow::on_button_box_rejected() {
	loadCurrentSettings();
}

void SettingsWindow::on_muc_nickname_textChanged(QString newtext) {
	ui->button_box->buttons().at(0)->setEnabled(!newtext.isEmpty());
	ui->button_box->buttons().at(2)->setEnabled(!newtext.isEmpty());
}
