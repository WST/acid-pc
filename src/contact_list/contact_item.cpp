#include <QImage>

#include "../functions.h"

#include "contact_item.h"

using namespace CL;

ContactItem::ContactItem(const QString &jid)
{
    setStatusType(QXmppPresence::Status::Offline);
	setStatusText(QString());
	addJid(jid);
}

bool ContactItem::addToGroup(GroupItem *group) {
	if (m_groups.indexOf(group) < 0) {
		m_groups << group;
		return group->addContact(this);
	}
	return false;
}

bool ContactItem::removeFromGroup(const QString &group_name) {
	GroupItem *found = NULL;
	foreach (GroupItem *item, m_groups)
		if (item->groupName() == group_name) {
			found = item;
			break;
		}
	if (found)
		return removeFromGroup(found);
	else
		return false;
}

bool ContactItem::removeFromGroup(GroupItem *group) {
	if (m_groups.removeOne(group))
		return group->removeContact(this);
	return false;
}

QString ContactItem::getText() const {
	QString base_text = getNick();
	if (base_text.isEmpty())
		base_text = getBareJid();
	return m_resources.size() > 1 ? (base_text + " (%1)").arg(m_resources.size()) : base_text;
}

void ContactItem::addJid(const QString &jid)
{
	QString bare, resource;
	split_jid(jid, &bare, &resource);
	m_bareJid = bare;
	unless (resource.isEmpty())
		m_resources.insert(resource);
}

void ContactItem::dropJid(const QString &jid) {
	QString bare, resource;
	split_jid(jid, &bare, &resource);
	m_resources.remove(resource);
}

void ContactItem::setStatusType(QXmppPresence::Status::Type _value)
{
	m_statusType = _value;
    QString icon;
	switch(_value)
    {
    case QXmppPresence::Status::Online:
    case QXmppPresence::Status::Chat:
    case QXmppPresence::Status::Away:
    case QXmppPresence::Status::XA:
    case QXmppPresence::Status::DND:
		icon = "online";
        break;
    case QXmppPresence::Status::Invisible:
    case QXmppPresence::Status::Offline:
		icon = "offline";
        break;
    }

	unless (icon.isEmpty())
		m_icon = QIcon(":/trayicon/"+icon+"-16px.png");
}

void ContactItem::setPresenceType(QXmppPresence::Type _value)
{
	m_presenceType = _value;
    QString icon;
	switch(_value)
    {
    case QXmppPresence::Available:
        break;
    case QXmppPresence::Unavailable:
		icon = "offline";
        break;
    case QXmppPresence::Error:
    case QXmppPresence::Subscribe:
    case QXmppPresence::Subscribed:
    case QXmppPresence::Unsubscribe:
    case QXmppPresence::Unsubscribed:
    case QXmppPresence::Probe:
        break;
    }

	unless (icon.isEmpty())
		m_icon = QIcon(":/trayicon/"+icon+"-16px.png");
}
