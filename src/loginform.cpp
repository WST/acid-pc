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

LoginForm::~LoginForm() {
    delete ui;
}
