#include "joinroomwindow.h"
#include "ui_joinroomwindow.h"

JoinRoomWindow::JoinRoomWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JoinRoomWindow)
{
    ui->setupUi(this);
}

JoinRoomWindow::~JoinRoomWindow()
{
    delete ui;
}
