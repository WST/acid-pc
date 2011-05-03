#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QtCore>

#define JID_RESOURCE_SEPARATOR "/"

QStringList parseJid(QString jid);

void split_jid(const QString &jid, QString *bare, QString *resource = NULL);

#define unless(x) if (!(x))
#define elsif else if

//// Helper class to store a pointer in QVariant
//template <class T> class VPtr
//{
//public:
//	static T *asPtr(QVariant v)
//	{
//		return static_cast<T *>(v.value<void *>());
//	}

//	static QVariant asQVariant(T* ptr)
//	{
//		return qVariantFromValue(static_cast<void *>(ptr));
//	}
//};

#endif // FUNCTIONS_H
