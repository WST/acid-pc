#include "mainwindow.h"

MainWindow::MainWindow(QString title, QWidget *parent): QMainWindow(parent) {
    setWindowTitle(title);
    setWindowIcon(QIcon(":/acid_16.png"));
}

MainWindow::~MainWindow() {

}
