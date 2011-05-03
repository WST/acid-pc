#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QtCore>

#define JID_RESOURCE_SEPARATOR "/"

QStringList parseJid(QString jid);

void split_jid(const QString &jid, QString *bare, QString *resource = NULL);

#define unless(x) if (!(x))
#define elsif else if

Q_DECLARE_METATYPE(const void *)

#endif // FUNCTIONS_H
