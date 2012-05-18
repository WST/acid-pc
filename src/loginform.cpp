#include "loginform.h"
#include "ui_loginform.h"

LoginForm::LoginForm(QString title, QWidget *parent): QWidget(parent), ui(new Ui::LoginForm) {
	ui->setupUi(this);
	connect(ui->SignIn_pushButton, SIGNAL(clicked()), this, SIGNAL(finished()));
	connect(ui->UserName_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(validateUsername(QString)));
	connect(ui->Password_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(validatePassword(QString)));
	setWindowTitle(title);
	username_valid = false;
	password_valid = false;
	ui->SignIn_pushButton->setEnabled(false);
	setWindowIcon(QIcon(":/acid_16.png"));
}

const QString LoginForm::username() {
	return ui->UserName_lineEdit->text();
}

const QString LoginForm::password() {
	return ui->Password_lineEdit->text();
}

const QString LoginForm::domain() {
	return ui->Server_comboBox->currentText();
}

bool LoginForm::autoLogin() {
	return ui->LogInAutomatically_checkBox->isChecked();
}

bool LoginForm::savePassword() {
	return ui->SavePassword_checkBox->isChecked();
}

void LoginForm::validateUsername(QString username) {
	// В дальнейшем здесь уместно сделать более надёжную проверку, чтобы сервер не засирали тупыми именами
	username_valid = !username.isEmpty();
	refreshButton();
}

void LoginForm::validatePassword(QString password) {
	password_valid = !password.isEmpty();
	refreshButton();
}

void LoginForm::refreshButton() {
	ui->SignIn_pushButton->setEnabled(username_valid && password_valid);
}

void LoginForm::setUsername(QString username) {
	ui->UserName_lineEdit->setText(username);
}

void LoginForm::setPassword(QString password) {
	ui->Password_lineEdit->setText(password);
}

void LoginForm::setDomain(QString domain) {
	ui->Server_comboBox->setCurrentIndex(ui->Server_comboBox->findText(domain));
}

void LoginForm::setAutoLogin(bool autologin) {
	ui->LogInAutomatically_checkBox->setChecked(autologin);
}

void LoginForm::setSavePassword(bool save) {
	ui->SavePassword_checkBox->setChecked(save);
}

void LoginForm::emitFinished() {
	emit finished();
}

LoginForm::~LoginForm() {
	delete ui;
}

void LoginForm::on_settings_button_clicked() {
    emit showSettingsRequested();
}
