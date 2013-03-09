#ifndef VOICECALLWINDOW_H
#define VOICECALLWINDOW_H

#include <QtWidgets/QDialog>

#include <qxmpp/QXmppCallManager.h>

namespace Ui {
    class VoiceCallWindow;
}

class VoiceCallWindow : public QDialog
{
    Q_OBJECT

public:
	explicit VoiceCallWindow(QWidget *parent, QXmppCall *call);
    ~VoiceCallWindow();

private slots:
	void on_hangup_button_clicked();
	void callStateChanged(QXmppCall::State state);
	void handleConnection();

private:
    Ui::VoiceCallWindow *ui;
};

#endif // VOICECALLWINDOW_H
