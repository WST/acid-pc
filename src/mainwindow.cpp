#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QString title, QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle(title);

    connect(ui->actionOffline, SIGNAL(triggered()), this, SIGNAL(wantOffline()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_Qt_triggered() {
    qApp->aboutQt();
}

void MainWindow::on_actionQuit_triggered() {
    qApp->exit();
}

QAction *MainWindow::newMessageAction() {
    return ui->actionNew_message;
}
