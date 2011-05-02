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
    ~JoinRoomWindow();

private:
    Ui::JoinRoomWindow *ui;
};

#endif // JOINROOMWINDOW_H
