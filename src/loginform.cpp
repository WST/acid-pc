#include "loginform.h"
#include "ui_loginform.h"

LoginForm::LoginForm(QWidget *parent): QWidget(parent), ui(new Ui::LoginForm) {
    ui->setupUi(this);
    connect(ui->SignIn_pushButton, SIGNAL(clicked()), this, SIGNAL(finished()));
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

LoginForm::~LoginForm()
{
    delete ui;
}
