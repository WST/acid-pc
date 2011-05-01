#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QtCore>

QStringList parseJid(QString jid);
void split_jid(const QString &jid, QString *bare, QString *resource = NULL);

#define unless(x) if (!(x))

#endif // FUNCTIONS_H
