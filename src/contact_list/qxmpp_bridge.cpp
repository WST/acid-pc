#include <QDebug>

#include "qxmpp_bridge.h"

using namespace CL;

ContactItem::Status QXmppBridge::qxmpp2cl(const QXmppPresence &presence) {
	ContactItem::Status newStatus;
	newStatus.type = ContactItem::Unchanged;

	switch (presence.type()) {
	case QXmppPresence::Available:
		newStatus.type = ContactItem::Online;
		break;
	case QXmppPresence::Unavailable:
		newStatus.type = ContactItem::Offline;
		break;
	default:;
	}

	ContactItem::StatusType parsedType = qxmpp2cl(presence.status().type());
	if (parsedType != ContactItem::Unchanged)
		newStatus.type = parsedType;

	newStatus.text = presence.status().statusText();
	return newStatus;
}

ContactItem::StatusType	QXmppBridge::qxmpp2cl(const QXmppPresence::Status::Type type) {
	switch (type) {
	case QXmppPresence::Status::Online:
		return ContactItem::Online;
	case QXmppPresence::Status::Chat:
		return ContactItem::Chat;
	case QXmppPresence::Status::Away:
		return ContactItem::Away;
	case QXmppPresence::Status::XA:
		return ContactItem::XA;
	case QXmppPresence::Status::DND:
		return ContactItem::DND;
	case QXmppPresence::Status::Invisible:
	case QXmppPresence::Status::Offline:
		return ContactItem::Offline;
	}
	return ContactItem::Unchanged;
}