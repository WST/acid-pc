#ifndef MESSANGER_H
#define MESSANGER_H

#include <QObject>

class Messanger: public QObject
{
    Q_OBJECT

public:
    Messanger(QObject *parent = 0);
};

#endif // MESSANGER_H
