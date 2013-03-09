#ifndef VCARDWINDOW_H
#define VCARDWINDOW_H

#include <QtWidgets/QDialog>

#include "messenger.h"

#include <qxmpp/QXmppVCardIq.h>

namespace Ui {
	class VcardWindow;
}

class Messenger;

class VcardWindow: public QDialog
{
	Q_OBJECT

	public:
		explicit VcardWindow(Messenger *parent,  const QXmppVCardIq *whose);
		~VcardWindow();

	private:
		Ui::VcardWindow *ui;
	
	private slots:
		void destroy();
};

#endif // VCARDWINDOW_H
