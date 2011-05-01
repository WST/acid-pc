#include "voicecallwindow.h"
#include "ui_voicecallwindow.h"

VoiceCallWindow::VoiceCallWindow(QWidget *parent, QXmppCall *call): QDialog(parent), ui(new Ui::VoiceCallWindow) {
	connect(this, SIGNAL(accepted()), call, SLOT(hangup()));
    ui->setupUi(this);

	ui->call_status->setText(tr("Ready"));
	connect(call, SIGNAL(stateChanged(QXmppCall::State)), this, SLOT(callStateChanged(QXmppCall::State)));
}

VoiceCallWindow::~VoiceCallWindow() {
    delete ui;
}


void VoiceCallWindow::on_hangup_button_clicked() {
	emit accepted();
}

void VoiceCallWindow::callStateChanged(QXmppCall::State state) {
	switch(state) {
		case QXmppCall::ConnectingState:
			ui->call_status->setText(tr("Calling"));
		break;
		case QXmppCall::ActiveState:
			ui->call_status->setText(tr("Call in progress"));
		break;
		case QXmppCall::DisconnectingState:
			ui->call_status->setText(tr("Disconnecting"));
		break;
		case QXmppCall::FinishedState:
			ui->call_status->setText("Finished");
			ui->hangup_button->setText(tr("Close"));
		break;
	}
}
