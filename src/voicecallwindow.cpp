#include "voicecallwindow.h"
#include "ui_voicecallwindow.h"

#include <QAudioInput>
#include <QAudioOutput>

#include <QXmppRtpChannel.h>
#include <QXmppJingleIq.h>

VoiceCallWindow::VoiceCallWindow(QWidget *parent, QXmppCall *call): QDialog(parent), ui(new Ui::VoiceCallWindow) {
	connect(this, SIGNAL(accepted()), call, SLOT(hangup()));
    ui->setupUi(this);

	ui->call_status->setText(tr("Ready"));
	connect(call, SIGNAL(stateChanged(QXmppCall::State)), this, SLOT(callStateChanged(QXmppCall::State)));
	connect(call, SIGNAL(connected()), this, SLOT(handleConnection()));

	if(call->direction() == QXmppCall::IncomingDirection) {
		call->accept();
	} else {
		// ?
	}
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

void VoiceCallWindow::handleConnection() {
	QXmppCall *call = qobject_cast<QXmppCall*>(sender());
	Q_ASSERT(call);

	QXmppRtpAudioChannel *channel = call->audioChannel();

	// prepare audio format
	QAudioFormat format;
	format.setFrequency(channel->payloadType().clockrate());
	format.setChannels(channel->payloadType().channels());
	format.setSampleSize(16);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);
	// the size in bytes of the audio buffers to/from sound devices
	// 160 ms seems to be the minimum to work consistently on Linux/Mac/Windows
	const int buffer_size = (format.frequency() * format.channels() * (format.sampleSize() / 8) * 160) / 1000;

	// initialise audio output
	QAudioOutput *audio_output = new QAudioOutput(format, this);
	audio_output->setBufferSize(buffer_size);
	audio_output->start(channel);

	// initialise audio input
	QAudioInput *audio_input = new QAudioInput(format, this);
	audio_input->setBufferSize(buffer_size);
	audio_input->start(channel);

	qDebug() << "initialized call!";
}
