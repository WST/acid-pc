#include "mucwidget.h"
#include "ui_mucwidget.h"
#include "functions.h"

MUCWidget::MUCWidget(QString with, QWidget *parent): TabWidget(with, parent), ui(new Ui::MUCWidget) {
    ui->setupUi(this);
    TabWidget::setType(TabWidget::MUC);
	connect(ui->chatview, SIGNAL(anchorClicked(const QUrl &)), this, SLOT(openLink(const QUrl &)));
	ui->chatview->document()->setDefaultStyleSheet("a { text-decoration: none; color: #0000AA; }");
}

MUCWidget::~MUCWidget() {
    delete ui;
}

void MUCWidget::openLink(const QUrl &link) {
	if(link.scheme() == "talkto") {
		ui->message->setText(link.toString().remove("talkto:") + ", " + ui->message->document()->toPlainText());
		QTextCursor cursor = ui->message->textCursor();
		cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
		ui->message->setTextCursor(cursor);
		ui->message->setFocus();
	}
}

void MUCWidget::insertMessage(QXmppMessage &message) {
    if(message.body().isEmpty()) {
		return;
    }
    QStringList jid = parseJid(message.from());
    if(!jid[5].isEmpty()) {
		ui->chatview->append("<a href=\"talkto:" + jid[5] + "\">&lt;" + jid[5] + "&gt;</a> " + message.body());
    } else {
		ui->chatview->append(message.body());
    }
}

void MUCWidget::setOnline(bool is_online) {
    ui->send->setEnabled((online = is_online) && !ui->message->toPlainText().isEmpty());
    ui->send->setToolTip(online ? "Send the message" : "This button is not available because you are not connected");
}

void MUCWidget::on_message_textChanged() {
    ui->send->setEnabled(online && !ui->message->toPlainText().isEmpty());
}

void MUCWidget::activate() {
    ui->message->setFocus();
}


void MUCWidget::on_send_clicked() {
    QString message = ui->message->toPlainText();
    ui->message->clear();
    ui->message->setFocus();
    emit aboutToSend(jid, message);
}
