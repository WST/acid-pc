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
    virtual void setOnline(bool is_online) = 0;
    void setType(Type newtype);

protected:
    QString jid;
    Type type;
    bool online;
};

#endif // TABWIDGET_H
