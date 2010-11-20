#include "mainwindow.h"

MainWindow::MainWindow(QString title, QWidget *parent): QMainWindow(parent), roster_model(this), roster(this) {
	setWindowTitle(title);
	setWindowIcon(QIcon(":/acid_16.png"));
	
	roster.setModel(& roster_model);
	setCentralWidget(& roster);
}

MainWindow::~MainWindow() {
	
}

RosterItemModel *MainWindow::model() {
	return & roster_model;
}
