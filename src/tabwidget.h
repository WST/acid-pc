#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QtGui>

class TabWidget: public QWidget
{
public:
    TabWidget(QString with, QWidget *parent = 0);
    QString &getJid();
    enum Type {Chat, MUC};
    Type getType();
    void setType(Type newtype);

private:
    QString jid;
    Type type;
};

#endif // TABWIDGET_H
