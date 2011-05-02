
#include "functions.h"

QStringList parseJid(QString jid) {
	QRegExp format = QRegExp("^(([^/@]+)@([^/@]+))(/([^/@]+))?$");
	return format.exactMatch(jid) ?  format.capturedTexts() : QStringList();
}

#define JID_RESOURCE_SEPARATOR '/'
void split_jid(const QString &jid, QString *bare, QString *resource) {
	if (bare || resource) {
		int sep = jid.indexOf(JID_RESOURCE_SEPARATOR);
		if (resource)
			*resource = sep >= 0 ? jid.mid(sep+1) : "";
		if (bare)
			*bare = jid.mid(0, sep);
	}
}
