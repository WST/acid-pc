#include "loginform.h"
#include "ui_loginform.h"

LoginForm::LoginForm(QWidget *parent): QWidget(parent), ui(new Ui::LoginForm) {
    ui->setupUi(this);
    connect(ui->SignIn_pushButton, SIGNAL(clicked()), this, SIGNAL(finished()));
}

LoginForm::~LoginForm()
{
    delete ui;
}
