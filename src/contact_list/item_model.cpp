#include <QDebug>

#include "../functions.h"
#include "qxmpp_bridge.h"

#include "item_model.h"

using namespace CL;

int ItemModel::columnCount(const QModelIndex &parent) const {
	Q_UNUSED(parent);
	return 1;
}

int ItemModel::rowCount(const QModelIndex &parent) const {
	if (parent.isValid()) {
		if ((parent.internalId() & 0xFFFF) == 0xFFFF) {
			return m_groups.at(parent.internalId() >> 16)->childCount();
		} else {
			return 0;
		}
	} else {
		return m_groups.size();
	}
}

QVariant ItemModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid()) {
		return QVariant();
	}

	const GroupItem *group = m_groups.at(index.internalId() >> 16);
	int childID = index.internalId() & 0xFFFF;
	const Item *item = group->childCount() > childID ?
				static_cast<const Item *>(group->getContacts().at(childID)) :
				static_cast<const Item *>(group);
	switch (role) {
	case Qt::DisplayRole:
		return item->getText();
	case Qt::DecorationRole:
		return item->getIcon();
	case Qt::ToolTipRole:
		return item->getSubText();
	case ItemRole:
		return qVariantFromValue(static_cast<const void *>(item));
	case NotificationRole:
		return item->notified();
	default:
		return QVariant();
	}
}

Qt::ItemFlags ItemModel::flags(const QModelIndex &index) const {
	if (!index.isValid()) {
		return 0;
	}

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex ItemModel::index(int row, int column, const QModelIndex &parent) const {
	if (parent.isValid()) {
		// parent is a group, return contact ID
		int groupID = parent.internalId() >> 16;
		return index(row, column, groupID);
	} else {
		// parent is root, return group ID
		return index(row, column);
	}

	return QModelIndex();
}

QModelIndex ItemModel::index(int row, int column, int groupID) const {
	if (groupID >= 0) {
		if (groupID < m_groups.size()) {
			GroupItem *group = m_groups.at(groupID);
			if (group->childCount() > row)
				return createIndex(row, column, (groupID << 16) + row);
		}
	} else {
		if (row < m_groups.size())
			return createIndex(row, column, (row << 16) + 0xFFFF);
	}

	return QModelIndex();
}

QModelIndex ItemModel::parent(const QModelIndex &child) const {
	if ((child.internalId() & 0xFFFF) == 0xFFFF)
		// child is a group, its parent is root
		return QModelIndex();
	else
		// child is a contact, its parent is a group
		return createIndex(child.internalId() >> 16, 0, static_cast<int>(child.internalId()) | 0xFFFF);
}

void ItemModel::setStatus(const QString &jid, const ContactItem::Status &_value) {
	QString resource;
	splitJid(jid, NULL, &resource);
	ContactItem *item = getContact(jid);

	if (!item) {
		LDEBUG("got presence from the not-in-roster item");
		// Status from not-in-roster
		if (throttleNotInRoster) {
			LDEBUG("configured for throttling them");
			return;
		}
		QSet<QString> notInRosterGroups;
		notInRosterGroups << notInRosterGroupName;
		item = updateEntry(jid, QString(), notInRosterGroups);
	}

	item->setResourceStatus(resource, _value);
}

void ItemModel::removeEntry(const QString &jid) {
	LDEBUG("removing entry %s from the contact list", qPrintable(jid));

	ContactItem *item = getContact(jid);
	if (item) {
		item->remove();
		delete item;
	}

	// XXX: remove empty groups. Optimization: only check groups the item belonged to.
}

ContactItem *ItemModel::updateEntry(const QString &jid, const QString &nick, QSet<QString> groups) {
	ContactItem *item = getContact(jid);
	if (!item) {
		item = new ContactItem(this, jid);
		QString bare_jid;
		splitJid(jid, &bare_jid);
		m_contacts[bare_jid] = item;
	}

	if (item->getNick() != nick) {
		item->setNick(nick);
	}

	// Synchronize groups
	const QList<GroupItem *> &current_groups = item->getGroups();
	for (int i = 0; i < current_groups.size(); ++i) {
		if (!groups.contains(current_groups[i]->getGroupName())) {
			item->removeFromGroup(current_groups[i--]);
		} else {
			groups.remove(current_groups[i]->getGroupName());
		}
	}

	foreach (const QString &group, groups) {
		item->addToGroup(getGroup(group));
	}

	// Item is out of all groups
	if (current_groups.empty()) {
		item->addToGroup(getGroup(noGroupName));
	}

	return item;
}

GroupItem *ItemModel::getGroup(const QString &name) {
	foreach (GroupItem *item, m_groups) {
		if (item->getGroupName() == name) {
			return item;
		}
	}

	// Create a new group
	GroupItem *new_item = new GroupItem(this, name);
	int i;
	for (i = 0; i < m_groups.size(); ++i) {
		if (m_groups[i]->getGroupName().compare(name, Qt::CaseInsensitive) > 0) {
			break;
		}
	}

	beginInsertRows(QModelIndex(), i, i);
	m_groups.insert(i, new_item);
	endInsertRows();

	return new_item;
}

ContactItem *ItemModel::getContact(const QString &jid) {
	QString bare_jid;
	splitJid(jid, &bare_jid);
	return m_contacts.value(bare_jid);
}

void ItemModel::contactAdded(GroupItem *const sender, int ind) {
	beginInsertRows(index(m_groups.indexOf(sender), 0), ind, ind);
	endInsertRows();
}

void ItemModel::contactMoved(GroupItem *const sender, int from, int to) {
#ifdef MOVE_ROWS_WORKAROUND
	int groupID = m_groups.indexOf(sender);
	emit layoutAboutToBeChanged();
	changePersistentIndex(index(from, 0, groupID),
							index(to, 0, groupID));
	emit layoutChanged();
#else
	QModelIndex groupIndex = index(m_groups.indexOf(sender), 0);
	beginMoveRows(groupIndex, from, from, groupIndex, to);
	endMoveRows();
#endif
}

void ItemModel::contactChanged(ContactItem *const sender) {
	foreach (GroupItem *group, sender->getGroups()) {
		QModelIndex itemIndex = index(group->getContacts().indexOf(sender), 0, m_groups.indexOf(group));
		emit dataChanged(itemIndex, itemIndex);
	}
}

void ItemModel::groupChanged(GroupItem *const sender) {
	QModelIndex groupIndex = index(m_groups.indexOf(sender), 0);
	emit dataChanged(groupIndex, groupIndex);
}
