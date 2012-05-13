#ifndef JOINROOMWINDOW_H
#define JOINROOMWINDOW_H

#include <QtGui/QDialog>

namespace Ui {
    class JoinRoomWindow;
}

class JoinRoomWindow : public QDialog
{
    Q_OBJECT

public:
    explicit JoinRoomWindow(QWidget *parent = 0);
    explicit JoinRoomWindow(QString error_message, QString room_jid, QWidget *parent = 0); // Если войти в комнату не удалось, показать окошко с такими параметрами
    ~JoinRoomWindow();

private slots:
	void on_room_link_textChanged(QString );
	void on_conference_server_textChanged(QString );
	void on_conference_name_textChanged(QString );

    void on_buttonBox_accepted();

signals:
    void joinRoomRequested(const QString &);

private:
    Ui::JoinRoomWindow *ui;
};

#endif // JOINROOMWINDOW_H
