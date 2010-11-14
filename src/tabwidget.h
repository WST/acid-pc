#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QtGui>

class TabWidget: public QWidget
{
private:
    QString jid;

public:
    TabWidget(QString with, QWidget *parent = 0);
    QString &getJid();
};

#endif // TABWIDGET_H
