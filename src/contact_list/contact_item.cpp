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
	setNotified(false);
}

ContactItem::ResourceStatus ContactItem::getResource(const QString &resource) const {
	ResourceStatus rs;
	if (resource.isEmpty()) {
		if (m_resources.size() == 1) {
			rs.resourceName = m_resources.constBegin().key();
			rs.status = m_resources.constBegin().value();
		} else {
			rs.status = NULL;
			for (QMap<QString, Status *>::const_iterator i = m_resources.constBegin();
					i != m_resources.constEnd(); ++i) {
				if (!rs.status || *rs.status < *i.value()) {
					rs.resourceName = i.key();
					rs.status = i.value();
				}
			}
		}
	} else {
		rs.resourceName = resource;
		rs.status = m_resources.value(resource);
	}

	return rs;
}

bool ContactItem::operator<(const ContactItem &_other) const {
	if (isOnline() == _other.isOnline()) {
		if (isOnline()) {
			// Both are online, compare statuses
			const Status *resource_status = getResource().status,
					*other_resource_status = _other.getResource().status;
			if (resource_status->type < other_resource_status->type) {
				return true;
			} else if (other_resource_status->type < resource_status->type) {
				return false;
			}
		}

		// Compare displayed text
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
	foreach (GroupItem *item, m_groups) {
		if (item->getGroupName() == group_name) {
			found = item;
			break;
		}
	}
	return found && removeFromGroup(found);
}

void ContactItem::setOffline() {
	foreach (Status *status, m_resources) {
		delete status;
	}
	m_resources.clear();
	changeStatus();
	updateIcon();
}

bool ContactItem::removeFromGroup(GroupItem *group) {
	if (m_groups.removeOne(group)) {
		return group->removeContact(this);
	}
	return false;
}

void ContactItem::remove() {
	while (!m_groups.isEmpty()) {
		removeFromGroup(m_groups.first());
	}
	emit removed();
}

QString ContactItem::getText() const {
	QString base_text = getNick();
	return m_resources.size() > 1 ? (base_text + " (%1)").arg(m_resources.size()) : base_text;
}

void ContactItem::setResourceStatus(const QString &resource, const Status &_value) {
	LDEBUG("setting resource status for %s to %d", qPrintable(resource), (int)_value.type);
	if (_value.type != Unchanged) {
		Status *rcptr = m_resources.value(resource);
		if (!rcptr && _value.type != Offline) {
			LDEBUG("this resource did not exist, and the status is not Offline. Updating");
			m_resources[resource] = rcptr = new Status();
		}
		if (_value.type == Offline) {
			LDEBUG("removing existing resource");
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
	foreach (GroupItem *group, m_groups) {
		group->statusChanged(this);
	}
}

void ContactItem::updateIcon() {
	QString new_icon_name = isOnline() ? statusString[getResource().status->type] :
										 statusString[Offline];
	LDEBUG("%s -> %s", qPrintable(icon_name), qPrintable(new_icon_name));
	if (icon_name != new_icon_name) {
		icon_name = new_icon_name;
		m_icon = QIcon(":/trayicon/"+icon_name+"-16px.png");
		owner->contactChanged(this);
		emit iconChanged(m_icon);
	}
}

void ContactItem::setNick(const QString &_value) {
	m_nick = _value;
	owner->contactChanged(this);
	emit nickChanged(_value);
}

QString ContactItem::getSubText() const {
	const ResourceStatus &rs = getResource();
	const QString statusTemplate =
		"<br />%1 %2 <img src=':/trayicon/%3-16px.png' /> <span color='%4'>%5</span>";
	const QString selectedStatusTemplate =
		QString("<b>%1</b>").arg(statusTemplate);

	QString baseText = QString("%1").arg(m_bareJid);
	if (rs.status) {
		for (QMap<QString, Status *>::const_iterator i = m_resources.constBegin();
				i != m_resources.constEnd(); ++i) {
			baseText += QString(
					(i.value() == rs.status) ? selectedStatusTemplate : statusTemplate).
					arg(i.key()).
					arg(QString("(%1)").arg(i.value()->priority)).
					arg(statusString[i.value()->type]).
					arg(statusColor[i.value()->type]).
					arg(i.value()->text);
		}
	} else {
		baseText += QString(statusTemplate).
			arg(QString()).
			arg(QString()).
			arg(statusString[Offline]).
			arg(statusColor[Offline]).
			arg(tr("offline"));
	}
	if (m_notified && !m_notification.isEmpty()) {
		baseText += "<br />" + m_notification;
	}
	return baseText;
}

void ContactItem::setNotified(bool notified, const QString &text) {
	if (m_notified != notified) {
		LDEBUG("changing notification status for %s from %d to %d: %s",
				qPrintable(m_bareJid), m_notified, notified, qPrintable(text));
		m_notified = notified;
		if (notified) {
			m_notification = text;
		} else {
			m_notification = QString();
		}
		owner->contactChanged(this);
	}
}
