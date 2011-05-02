#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QtGui/QDialog>

#include "messenger.h"
#include "version.h"

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
		QSettings *settings;
		void loadCurrentSettings();

private slots:
	void on_muc_nickname_textChanged(QString );
 void on_button_box_rejected();
 void on_button_box_accepted();
 void on_file_transfer_mode_currentIndexChanged(int index);
 void on_savepath_browse_button_clicked();

 void on_select_savepath_everytime_clicked(bool checked);

signals:
		void modified();
};

#endif // SETTINGSWINDOW_H
