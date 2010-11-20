#include "mainwindow.h"

MainWindow::MainWindow(QString title, QWidget *parent): QMainWindow(parent), roster(this), roster_model(this) {
	setWindowTitle(title);
	setWindowIcon(QIcon(":/acid_16.png"));
	
	roster.setModel(& roster_model);
	setCentralWidget(& roster);

	connect(& roster, SIGNAL(showChatDialog(QString)), this, SIGNAL(showChatDialog(QString)));
	connect(& roster, SIGNAL(showProfile(QString)), this, SIGNAL(showProfile(QString)));
	connect(& roster, SIGNAL(removeContact(QString)), this, SIGNAL(removeContact(QString)));
}

MainWindow::~MainWindow() {
	
}

RosterItemModel *MainWindow::model() {
	return & roster_model;
}
