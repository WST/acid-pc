#ifndef MESSENGER_H
#define MESSENGER_H

#include <QObject>

class Messenger: public QObject
{
    Q_OBJECT

public:
    Messenger(QObject *parent = 0);
};

#endif // MESSENGER_H
