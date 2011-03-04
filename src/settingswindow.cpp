#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(Messenger *parent): QDialog(parent), ui(new Ui::SettingsWindow) {
	ui->setupUi(this);
}

SettingsWindow::~SettingsWindow() {
	delete ui;
}
