#include "item_model.h"

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
	if (parsedType != ContactItem::Unchanged) {
		newStatus.type = parsedType;
	}

	newStatus.text = presence.status().statusText();
	newStatus.priority = presence.status().priority();

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
		//case QXmppPresence::Status::Invisible:
		case QXmppPresence::Status::Offline:
			return ContactItem::Offline;
	}
	return ContactItem::Unchanged;
}

#define CHECK_PROPER_USE(field, useless) \
	{ \
		if (!field) { \
			LWARN("Improper use of the function: " #field " not set"); \
			if (useless) { return; } \
		} \
	}

QXmppBridge::QXmppBridge(ItemModel *model, QXmppRosterManager *rosterManager):
	m_model(model), m_rosterManager(rosterManager), m_mucRoom(0) {
	CHECK_PROPER_USE(m_rosterManager, true);
	connect(rosterManager, SIGNAL(itemAdded(const QString&)),
			this, SLOT(rosterEntryAdded(const QString &)));
	connect(rosterManager, SIGNAL(itemChanged(const QString&)),
			this, SLOT(rosterEntryChanged(const QString &)));
	connect(rosterManager, SIGNAL(itemRemoved(const QString&)),
			this, SLOT(rosterEntryRemoved(const QString &)));
	connect(rosterManager, SIGNAL(presenceChanged(const QString &, const QString &)),
			this, SLOT(rosterEntryPresence(const QString &, const QString &)));
	connect(rosterManager, SIGNAL(rosterReceived()),
			this, SLOT(rosterSynchronize()));
}

QXmppBridge::QXmppBridge(ItemModel *model, QXmppMucRoom *mucRoom):
	m_model(model), m_rosterManager(0), m_mucRoom(mucRoom) {
	CHECK_PROPER_USE(m_mucRoom, true);
	connect(mucRoom, SIGNAL(participantAdded(const QString&)),
			this, SLOT(mucEntryAdded(const QString &)));
	connect(mucRoom, SIGNAL(participantChanged(const QString&)),
			this, SLOT(mucEntryChanged(const QString &)));
	connect(mucRoom, SIGNAL(participantRemoved(const QString&)),
			this, SLOT(mucEntryRemoved(const QString &)));
}

void QXmppBridge::rosterEntryAdded(const QString &bareJid) {
	rosterEntryChanged(bareJid);
}

void QXmppBridge::rosterEntryChanged(const QString &bareJid) {
	CHECK_PROPER_USE(m_rosterManager, true);
	const QXmppRosterIq::Item &item = m_rosterManager->getRosterEntry(bareJid);
	m_model->updateEntry(bareJid, item.name(), item.groups());
}

void QXmppBridge::rosterEntryRemoved(const QString &bareJid) {
	CHECK_PROPER_USE(m_rosterManager, false);
	m_model->removeEntry(bareJid);
}

void QXmppBridge::rosterEntryPresence(const QString &bareJid, const QString &resource) {
	CHECK_PROPER_USE(m_rosterManager, true);

	const QMap<QString, QXmppPresence> &presences =
		m_rosterManager->getAllPresencesForBareJid(bareJid);
	CL::ContactItem::Status status;

	if (presences.contains(resource)) {
		status = CL::QXmppBridge::qxmpp2cl(presences[resource]);
	}

	m_model->setStatus(QString("%1" JID_RESOURCE_SEPARATOR "%2").arg(bareJid).arg(resource), status);
}

void QXmppBridge::rosterSynchronize() {
	CHECK_PROPER_USE(m_rosterManager, true);
	LDEBUG("synchronizing rosters");
	const QStringList &bareJids = m_rosterManager->getRosterBareJids();
	foreach (QString bareJid, bareJids) {
		rosterEntryAdded(bareJid);
	}
}

void QXmppBridge::mucEntryAdded(const QString &jid) {
	mucEntryChanged(jid);
}

void QXmppBridge::mucEntryChanged(const QString &jid) {
	CHECK_PROPER_USE(m_mucRoom, true);
	const QXmppPresence &item = m_mucRoom->participantPresence(jid);
	QSet<QString> groups;
	groups += "Participants";
	QString nick, bareJid;
	splitJid(jid, &bareJid, &nick);
	m_model->updateEntry(bareJid, nick, groups);
}

void QXmppBridge::mucEntryRemoved(const QString &jid) {
	CHECK_PROPER_USE(m_mucRoom, false);
	m_model->removeEntry(jid);
}

void QXmppBridge::mucEntryPresence(const QString &jid) {
}

