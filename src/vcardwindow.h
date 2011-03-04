#ifndef VCARDWINDOW_H
#define VCARDWINDOW_H

#include <QtGui/QDialog>

#include "messenger.h"

namespace Ui {
	class VcardWindow;
}

class Messenger;

class VcardWindow: public QDialog
{
	Q_OBJECT

	public:
		explicit VcardWindow(Messenger *parent, QString bare_jid);
		~VcardWindow();

	private:
		Ui::VcardWindow *ui;
	
	private slots:
		void destroy();
};

#endif // VCARDWINDOW_H
