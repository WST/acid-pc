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
	if (parent.isValid())
		if ((parent.internalId() & 0xFFFF) == 0xFFFF)
			return m_groups.at(parent.internalId() >> 16)->childCount();
		else
			return 0;
	else
		return m_groups.size();
}

QVariant ItemModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();

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
	case ContactItemRole:
		return (int)(item->childCount() ? NULL : item);
	case GroupItemRole:
		return (int)(item->childCount() ? item : NULL);
	default:
		return QVariant();
	}
}

Qt::ItemFlags ItemModel::flags(const QModelIndex &index) const {
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex ItemModel::index(int row, int column, const QModelIndex &parent) const {
	if (parent.isValid()) {
		// parent is a group, return contact ID
		int groupID = parent.internalId() >> 16;
		if (groupID < m_groups.size()) {
			GroupItem *group = m_groups.at(groupID);
			if (group->childCount() > row)
				return createIndex(row, column, (groupID << 16) + row);
		}
	} else {
		// parent is root, return group ID
		if (m_groups.size() > row)
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
	split_jid(jid, NULL, &resource);
	ContactItem *item = getContact(jid);

	unless (item) {
		if (throttleNotInRoster)
			return;
		QSet<QString> notInRosterGroups;
		notInRosterGroups << notInRosterGroupName;
		item = updateEntry(jid, QString(), notInRosterGroups);
	}

	item->setResourceStatus(resource, _value);
	reset();
}

ContactItem *ItemModel::updateEntry(const QString &jid, const QString &nick, QSet<QString> groups) {
	ContactItem *item = getContact(jid);
	unless (item) {
		item = new ContactItem(jid);
		QString bare_jid;
		split_jid(jid, &bare_jid);
		m_contacts[bare_jid] = item;
	}

	if (item->getNick() != nick)
		item->setNick(nick);

	// Synchronize groups (note: due to performance impact, this code is made unportable through containers)
	const QList<GroupItem *> &current_groups = item->getGroups();
	for (int i = 0; i < current_groups.size(); ++i)
		unless (groups.contains(current_groups[i]->getGroupName()))
			item->removeFromGroup(current_groups[i--]);
		else
			groups.remove(current_groups[i]->getGroupName());

	foreach (const QString &group, groups)
		item->addToGroup(getGroup(group));

	unless (current_groups.size())
		item->addToGroup(getGroup(noGroupName));
	reset();
	return item;
}

GroupItem *ItemModel::getGroup(const QString &name) {
	foreach (GroupItem *item, m_groups)
		if (item->getGroupName() == name)
			return item;
	GroupItem *new_item = new GroupItem(name);

	int i;
	for (i = 0; i < m_groups.size(); ++i)
		if (m_groups[i]->getGroupName().compare(name, Qt::CaseInsensitive) > 0)
			break;
	m_groups.insert(i, new_item);

	return new_item;
}

ContactItem *ItemModel::getContact(const QString &jid) {
	QString bare_jid;
	split_jid(jid, &bare_jid);
	return m_contacts.value(bare_jid);
}
