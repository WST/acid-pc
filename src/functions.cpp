
#include "functions.h"

QStringList parseJid(QString jid) {
	QRegExp format = QRegExp("^(([^/@]+)@([^/@]+))(/([^/@]+))?$");
	return format.exactMatch(jid) ?  format.capturedTexts() : QStringList();
}

#define JID_RESOURCE_SEPARATOR '/'
void split_jid(const QString &jid, QString *bare, QString *resource) {
	int sep = jid.indexOf(JID_RESOURCE_SEPARATOR);
	if (sep >= 0) {
		if (resource)
			resource->operator=(QString(jid.mid(sep + 1)));
	} else {
		sep = -1;
		if (resource)
			resource->operator=(QString());
	}
	bare->operator=(QString(jid.mid(0, sep)));
}
