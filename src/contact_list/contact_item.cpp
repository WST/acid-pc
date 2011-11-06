#include <QImage>

#include "../functions.h"
#include "group_item.h"
#include "item_model.h"

#include "contact_item.h"

using namespace CL;

const char *ContactItem::statusString[] = { "offline", "dnd", "xa",     "away",   "online", "f4c" };
const char *ContactItem::statusColor[] =  { "gray",    "red", "orange", "yellow", "green",  "green" };

ContactItem::ContactItem(ItemModel *_owner, const QString &_jid): owner(_owner) {
	splitJid(_jid, &m_bareJid);
	setOffline();
}

ContactItem::ResourceStatus ContactItem::getResource(const QString &resource) const {
	if (resource.isEmpty()) {
		if (m_resources.size() == 1) {
			return ResourceStatus(m_resources.constBegin().key(), m_resources.constBegin().value());
		} else {
			QString highestName;
			const Status *highestStatus = NULL;
			for (QMap<QString, Status *>::const_iterator i = m_resources.constBegin(); i != m_resources.constEnd(); ++i)
				if (!highestStatus || *highestStatus < *i.value()) {
					highestName = i.key();
					highestStatus = i.value();
				}
			return ResourceStatus(highestName, highestStatus);
		}
	} else {
		return ResourceStatus(resource, m_resources.value(resource));
	}
}

bool ContactItem::operator<(const ContactItem &_other) const {
	if (isOnline() == _other.isOnline()) {
		if (isOnline()) {
			// Both are online, compare statuses
			const Status *resource_status = getResource().second,
					*other_resource_status = _other.getResource().second;
			if (resource_status->type < other_resource_status->type)
				return true;
			else if (other_resource_status->type < resource_status->type)
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
	changeStatus();
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
	if (!_value.type == Unchanged) {
		Status *rcptr = m_resources.value(resource);
		if (!rcptr && _value.type != Offline)
			m_resources[resource] = rcptr = new Status();
		if (_value.type == Offline) {
			if (rcptr) {
				delete rcptr;
				m_resources.remove(resource);
			}
		} else {
			*rcptr = _value;
		}

		changeStatus();
		updateIcon();
	}
}

void ContactItem::changeStatus() {
	foreach (GroupItem *group, m_groups)
		group->statusChanged(this);
}

void ContactItem::updateIcon() {
	QString new_icon_name = isOnline() ? statusString[getResource().second->type] :
										 statusString[Offline];
	if (icon_name != new_icon_name) {
		icon_name = new_icon_name;
		m_icon = QIcon(":/trayicon/"+icon_name+"-16px.png");
		owner->contactChanged(this);
	}
}

void ContactItem::setNick(const QString &_value) {
	m_nick = _value;
	owner->contactChanged(this);
}

QString ContactItem::getSubText() const {
	const ResourceStatus &rs = getResource();
	QString baseText;
	if (rs.second)
		baseText = QString("<span color='%1'>%2</span>\n").arg(statusColor[rs.second->type]).arg(rs.second->text);
	else
		baseText = QString("<span color='grey'>offline</span>");
	for (QMap<QString, Status *>::const_iterator i = m_resources.constBegin(); i != m_resources.constEnd(); ++i)
		baseText += QString("%1 <img src=':/trayicon/%2-16px.png' /> <span color='%3'>%4</span>\n").
				arg(i.key()).
				arg(statusString[i.value()->type]).
				arg(statusColor[i.value()->type]).
				arg(i.value()->text);
	return baseText;
}
