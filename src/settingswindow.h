#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QtGui/QDialog>

#include "messenger.h"

namespace Ui {
	class SettingsWindow;
}

class Messenger;

class SettingsWindow: public QDialog
{
	Q_OBJECT
	public:
		explicit SettingsWindow(Messenger *parent = 0);
		~SettingsWindow();

	private:
		Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
