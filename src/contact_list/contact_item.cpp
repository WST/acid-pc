#include <QImage>

#include "../functions.h"
#include "group_item.h"

#include "contact_item.h"

using namespace CL;

const char *ContactItem::statusString[] = { "offline", "dnd", "xa", "away", "online", "f4c" };

ContactItem::ContactItem(const QString &jid) {
	split_jid(jid, &m_bareJid);
	setOffline();
}

const ContactItem::Status *ContactItem::getResourceStatus(const QString &resource) const {
	if (resource.isEmpty()) {
		if (m_resources.size() == 1) {
			return m_resources.constBegin().value();
		} else {
			const Status *highest = NULL;
			foreach (const Status *status, m_resources)
				if (!highest || *highest < *status)
					highest = status;
			return highest;
		}
	} else {
		return m_resources.value(resource);
	}
}

bool ContactItem::operator<(const ContactItem &_other) const {
	if (isOnline() == _other.isOnline()) {
		if (isOnline()) {
			// Both are online, compare statuses
			const Status *resource_status = getResourceStatus(),
					*other_resource_status = _other.getResourceStatus();
			if (*resource_status < *other_resource_status)
				return true;
			elsif (*other_resource_status < *resource_status)
				return false;
		}

		// Compare nicknames
		return getText().compare(_other.getText(), Qt::CaseInsensitive) > 0;
	} else {
		// Compare 'online' to 'offline'
		return isOnline() < _other.isOnline();
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
		if (item->getGroupName() == group_name) {
			found = item;
			break;
		}
	if (found)
		return removeFromGroup(found);
	else
		return false;
}

void ContactItem::setOffline() {
	foreach (Status *status, m_resources)
		delete status;
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

void ContactItem::setResourceStatus(const QString &resource, const Status &_value) {
	unless (_value.type == Unchanged) {
		Status *rcptr = m_resources.value(resource);
		unless (rcptr || _value.type == Offline) {
			m_resources[resource] = rcptr = new Status();
			rcptr->resource = resource;
		}
		if (_value.type == Offline) {
			if (rcptr) {
				delete rcptr;
				m_resources.remove(resource);
			}
		} else {
			*rcptr = _value;
		}

		foreach (GroupItem *group, m_groups)
			group->statusChanged(this);

		updateIcon();
	}
}

void ContactItem::updateIcon() {
	QString icon_name = isOnline() ? statusString[getResourceStatus()->type] :
									 statusString[Offline];
	m_icon = QIcon(":/trayicon/"+icon_name+"-16px.png");
}
