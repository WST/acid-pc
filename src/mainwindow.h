#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

// ACId
#include "roster_list_view.h"
#include "roster_item_model.h"

namespace Ui {
    class MainWindow;
}

class MainWindow: public QMainWindow
{
	Q_OBJECT
	public:
		MainWindow(QString title, QWidget *parent = 0);
		~MainWindow();
		RosterItemModel *model();

	private:
		RosterListView roster;
		RosterItemModel roster_model;

	signals:
		void showChatDialog(const QString& bareJid);
		void showProfile(const QString& bareJid);
		void removeContact(const QString& bareJid);
};

#endif // MAINWINDOW_H
