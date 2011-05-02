#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(Messenger *parent): QDialog(parent), ui(new Ui::SettingsWindow) {
	ui->setupUi(this);
	settings = parent->settingsManager();
	loadCurrentSettings();
	setWindowIcon(QIcon(":/acid_16.png"));
}

SettingsWindow::~SettingsWindow() {
	delete ui;
}

void SettingsWindow::on_file_transfer_mode_currentIndexChanged(int index) {
	ui->file_transfer_proxy->setEnabled(index == 0);
}

void SettingsWindow::on_button_box_accepted() {
	// Первый таб
	settings->setValue("settings/automatically_accept_calls", ui->automatically_accept_calls->checkState());
	settings->setValue("settings/automatically_accept_files", ui->automatically_accept_files->checkState());
	settings->setValue("settings/automatically_open_new_tabs", ui->automatically_open_new_tabs->checkState());
	settings->setValue("settings/file_transfer_mode", ui->file_transfer_mode->currentIndex());
	settings->setValue("settings/file_transfer_proxy", ui->file_transfer_proxy->text());
	settings->setValue("settings/muc_nickname", ui->muc_nickname->text());

	// Второй таб
	settings->setValue("settings/keepalive_interval", ui->keepalive_interval->value());
	settings->setValue("settings/keepalive_timeout", ui->keepalive_timeout->value());

	// Третий таб
	settings->setValue("settings/roster_opacity", ui->roster_opacity->value());
	settings->setValue("settings/gui_style", ui->gui_style->currentIndex());
	settings->setValue("settings/notification_display_time", ui->notification_display_time->value());
	settings->setValue("settings/roster_on_the_top", ui->roster_on_the_top->checkState());

	// Четвёртый таб
	settings->setValue("settings/savepath", ui->savepath->text());

	emit modified();
}

void SettingsWindow::loadCurrentSettings() {
	// Первый таб
	//ui->file_transfer_proxy->setEnabled(ui->file_transfer_mode->currentIndex() == 0);
	ui->automatically_accept_calls->setChecked(settings->value("settings/automatically_accept_calls", false).toBool());
	ui->automatically_open_new_tabs->setChecked(settings->value("settings/automatically_open_new_tabs", false).toBool());
	ui->automatically_accept_files->setChecked(settings->value("settings/automatically_accept_files", false).toBool());
	ui->muc_nickname->setText(settings->value("settings/muc_nickname", "").toString());

	// Второй таб
	ui->keepalive_interval->setValue(settings->value("settings/keepalive_interval", 60).toInt());
	ui->keepalive_timeout->setValue(settings->value("settings/keepalive_timeout", 30).toInt());

	// Третий таб
	ui->roster_opacity->setValue(settings->value("settings/roster_opacity", 100).toInt());
	ui->gui_style->setCurrentIndex(settings->value("settings/gui_style", 0).toInt());
	ui->roster_on_the_top->setChecked(settings->value("settings/roster_on_the_top", false).toBool());
	ui->notification_display_time->setValue(settings->value("settings/notification_display_time", 5).toInt());

	// Четвёртый таб
	ui->savepath->setText(settings->value("settings/savepath", INCOMING_FILES_STORAGE).toString());
	ui->select_savepath_everytime->setChecked(settings->value("settings/select_savepath_everytime", false).toBool());
	ui->savepath->setEnabled(!ui->select_savepath_everytime->checkState());
	ui->savepath_browse_button->setEnabled(!ui->select_savepath_everytime->checkState());
}

void SettingsWindow::on_button_box_rejected() {
	loadCurrentSettings();
}

void SettingsWindow::on_muc_nickname_textChanged(QString newtext) {
	ui->button_box->buttons().at(0)->setEnabled(!newtext.isEmpty());
	//ui->button_box->buttons().at(2)->setEnabled(!newtext.isEmpty());
}

void SettingsWindow::on_savepath_browse_button_clicked() {
	QString dir = QFileDialog::getExistingDirectory(this, "Select a directory");
	if(!dir.isNull()) {
		ui->savepath->setText(dir);
	}
}

void SettingsWindow::on_select_savepath_everytime_clicked(bool checked) {
	ui->savepath->setEnabled(!checked);
	ui->savepath_browse_button->setEnabled(!checked);
}
