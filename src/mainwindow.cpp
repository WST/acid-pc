#include "mainwindow.h"

MainWindow::MainWindow(QString title, QWidget *parent): QMainWindow(parent), roster_widget(this), roster_model(this) {
	// этот класс здесь, вообще говоря, лишь лишняя прослойка. Можно было бы всё сделать в Messenger, включая UI
	setWindowTitle(title);
	setWindowIcon(QIcon(":/acid_16.png"));
	
	roster_widget.setModel(& roster_model);
	setCentralWidget(& roster_widget);

	connect(& roster_widget, SIGNAL(showChatDialog(QString)), this, SIGNAL(showChatDialog(QString)));
	connect(& roster_widget, SIGNAL(showProfile(QString)), this, SIGNAL(showProfile(QString)));
	connect(& roster_widget, SIGNAL(removeContact(QString)), this, SIGNAL(removeContact(QString)));
}

MainWindow::~MainWindow() {
	
}

RosterItemModel *MainWindow::model() {
	return & roster_model;
}

RosterListView *MainWindow::roster() {
    return & roster_widget;
}
