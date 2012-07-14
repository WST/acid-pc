#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(Messenger *parent): QDialog(parent), ui(new Ui::SettingsWindow) {
	ui->setupUi(this);
	settings = parent->settingsManager();
	loadCurrentSettings();
    resize(width(), minimumSizeHint().height());
    ui->tabWidget->setCurrentIndex(0);
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
    settings->setValue("settings/language", ui->language->currentIndex());
    settings->setValue("login/auto", ui->auto_connect->checkState());

	// Второй таб
	settings->setValue("settings/keepalive_interval", ui->keepalive_interval->value());
	settings->setValue("settings/keepalive_timeout", ui->keepalive_timeout->value());
    settings->setValue("settings/stun_server", ui->stun_server->text());

	// Третий таб
	settings->setValue("settings/roster_opacity", ui->roster_opacity->value());
	settings->setValue("settings/gui_style", ui->gui_style->currentIndex());
	settings->setValue("settings/notification_display_time", ui->notification_display_time->value());
	settings->setValue("settings/roster_on_the_top", ui->roster_on_the_top->checkState());
	settings->setValue("settings/animate_roster", ui->animate_roster->checkState());
    settings->setValue("settings/hide_tab_captions", ui->hide_tab_captions->checkState());
    settings->setValue("settings/show_toolbar", ui->show_toolbar->checkState());

	// Четвёртый таб
	settings->setValue("settings/savepath", ui->savepath->text());
    settings->setValue("settings/emoticon_theme", ui->emoticon_theme->currentText());
    settings->setValue("settings/sound_thems", ui->sound_theme->currentText());
    settings->setValue("settings/enable_sound", ui->enable_sound->checkState());
    settings->setValue("settings/enable_emoticons", ui->enable_emoticons->checkState());
    // Ручное указание опций в списках тем является костылём и сделано в угоду скорости разработки

	emit modified();
}

void SettingsWindow::loadCurrentSettings() {
	// Первый таб
	//ui->file_transfer_proxy->setEnabled(ui->file_transfer_mode->currentIndex() == 0);
	ui->automatically_accept_calls->setChecked(settings->value("settings/automatically_accept_calls", false).toBool());
	ui->automatically_open_new_tabs->setChecked(settings->value("settings/automatically_open_new_tabs", false).toBool());
	ui->automatically_accept_files->setChecked(settings->value("settings/automatically_accept_files", false).toBool());
	ui->muc_nickname->setText(settings->value("settings/muc_nickname", "").toString());
    ui->auto_connect->setChecked(settings->value("login/auto", false).toBool());
    ui->language->setCurrentIndex(settings->value("settings/language", 0).toInt());

	// Второй таб
	ui->keepalive_interval->setValue(settings->value("settings/keepalive_interval", 60).toInt());
	ui->keepalive_timeout->setValue(settings->value("settings/keepalive_timeout", 30).toInt());
    ui->stun_server->setText(settings->value("settings/stun_server", STUN_ADDRESS).toString());

	// Третий таб
	ui->roster_opacity->setValue(settings->value("settings/roster_opacity", 100).toInt());
	ui->gui_style->setCurrentIndex(settings->value("settings/gui_style", 0).toInt());
	ui->roster_on_the_top->setChecked(settings->value("settings/roster_on_the_top", false).toBool());
	ui->notification_display_time->setValue(settings->value("settings/notification_display_time", 5).toInt());
	ui->animate_roster->setChecked(settings->value("settings/animate_roster", true).toBool());
    ui->hide_tab_captions->setChecked(settings->value("settings/hide_tab_captions", true).toBool());
    ui->show_toolbar->setChecked(settings->value("settings/show_toolbar", true).toBool());

	// Четвёртый таб
	ui->savepath->setText(settings->value("settings/savepath", INCOMING_FILES_STORAGE).toString());
	ui->select_savepath_everytime->setChecked(settings->value("settings/select_savepath_everytime", false).toBool());
	ui->savepath->setEnabled(!ui->select_savepath_everytime->checkState());
	ui->savepath_browse_button->setEnabled(!ui->select_savepath_everytime->checkState());
    ui->emoticon_theme->setCurrentIndex(0); // заглушка
    ui->sound_theme->setCurrentIndex(0); // заглушка
    ui->enable_sound->setChecked(settings->value("settings/enable_sound", true).toBool());
    ui->enable_emoticons->setChecked(settings->value("settings/enable_emoticons", true).toBool());
}

void SettingsWindow::on_button_box_rejected() {
	loadCurrentSettings();
}

void SettingsWindow::on_muc_nickname_textChanged(QString newtext) {
	ui->button_box->buttons().at(0)->setEnabled(!newtext.isEmpty());
	//ui->button_box->buttons().at(2)->setEnabled(!newtext.isEmpty());
}

void SettingsWindow::on_savepath_browse_button_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select a directory"));
	if(!dir.isNull()) {
		ui->savepath->setText(dir);
	}
}

void SettingsWindow::on_select_savepath_everytime_clicked(bool checked) {
	ui->savepath->setEnabled(!checked);
	ui->savepath_browse_button->setEnabled(!checked);
}
