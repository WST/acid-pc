#include <QImage>

#include "../functions.h"

#include "contact_item.h"

using namespace CL;

ContactItem::ContactItem(const QString &jid) {
	split_jid(jid, &m_bareJid);
	setOffline();
}

const ContactItem::Status *ContactItem::getResource(const QString &name) const {
	if (name.isEmpty()) {
		if (m_resources.size() == 1) {
			return m_resources.constBegin().value();
		} else {
			const Status *highest = NULL;
			foreach (const Status *r, m_resources)
				if (!highest || r->priority > highest->priority)
					highest = r;
			return highest;
		}
	} else {
		return m_resources[name];
	}
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

void ContactItem::setOffline() {
	m_resources.clear();
	updateIcon();
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

void ContactItem::updatePresence(const QString &resource, const Status &status) {
	if (status.type != Unchanged) {
		Status *rcptr = m_resources[resource];
		unless (rcptr || status.type == Offline)
			m_resources[resource] = rcptr = new Status();
		if (status.type == Offline) {
			if (rcptr) {
				delete rcptr;
				m_resources.remove(resource);
			}
		} else {
			*rcptr = status;
		}
		updateIcon();
	}
}

void ContactItem::updateIcon() {
	QString icon = isOnline() ? "online" : "offline";
	m_icon = QIcon(":/trayicon/"+icon+"-16px.png");
}
