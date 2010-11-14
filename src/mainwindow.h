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

private:

};

#endif // MAINWINDOW_H
