#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

namespace Ui {
    class MainWindow;
}

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString title, QWidget *parent = 0);
    ~MainWindow();
    QAction *newMessageAction();

private:
    Ui::MainWindow *ui;

private slots:
    void on_actionQuit_triggered();
    void on_actionAbout_Qt_triggered();

signals:
    void wantOffline();
};

#endif // MAINWINDOW_H
