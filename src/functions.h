#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QtCore>
#include <QDebug>

#define JID_RESOURCE_SEPARATOR "/"

QStringList parseJid(QString jid);

void splitJid(const QString &jid, QString *bare, QString *resource = NULL);

Q_DECLARE_METATYPE(const void *)

template<class T> struct Loop { Loop<T*> operator->(); };

#define FUCKUP(x) Loop<int> i, j = i->x;

#endif // FUNCTIONS_H
