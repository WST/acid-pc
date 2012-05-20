#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QtGui>

class TabWidget: public QWidget
{
public:
    TabWidget(QString with, QWidget *parent = 0);
    QString &getJid();
    enum Type {Chat, MUC, ServiceDiscovery};
    Type getType();
    virtual void setOnline(bool is_online) = 0;
    void setType(Type newtype);
    //virtual void setIcon(const QString &path) = 0;
    //virtual void setIcon(QIcon &icon) = 0;

protected:
    QString jid;
    Type type;
    bool online;
};

#endif // TABWIDGET_H
