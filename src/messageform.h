#ifndef MESSAGEFORM_H
#define MESSAGEFORM_H

#include <QDialog>

namespace Ui {
    class MessageForm;
}

class MessageForm : public QDialog
{
    Q_OBJECT

public:
    explicit MessageForm(QWidget *parent = 0);
    ~MessageForm();
    const QString jid();
    const QString body();

private:
    Ui::MessageForm *ui;

public slots:
    void enableSendButton();
    void disableSendButton();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

signals:
    void readyToSend(MessageForm *message);
};

#endif // MESSAGEFORM_H
