
#include "functions.h"

QStringList parseJid(QString jid) {
	QRegExp format = QRegExp("^(([^/@]+)@([^/@]+))(/([^/@]+))?$");
	return format.exactMatch(jid) ?  format.capturedTexts() : QStringList();
}
