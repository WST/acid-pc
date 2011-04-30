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
		if ((parent.internalId() & 0xFFFF) == 0xFFFF)
			return m_groups.at(parent.internalId() >> 16)->childCount();
		else
			return 0;
	} else {
		return m_groups.size();
	}
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
	case BareJid:
		return static_cast<const ContactItem *>(item)->getBareJid();
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
		// child is a group, it's parent is root
		return QModelIndex();
	else
		// child is a contact, it's parent is a group
		return createIndex(child.internalId() >> 16, 0, static_cast<int>(child.internalId()) | 0xFFFF);
}


void ItemModel::setStatus(const QString &jid, const ContactItem::Status &_value) {
	QString bare_jid, resource;
	split_jid(jid, &bare_jid, &resource);
	ContactItem *item = getItemByJid(jid);
	if (!item) {
		QSet<QString> notInRosterGroups;
		notInRosterGroups << notInRosterGroupName;
		item = addEntry(jid, QString(), notInRosterGroups);
	}

	item->updatePresence(resource, _value);
}

ContactItem *ItemModel::addEntry(const QString &jid, const QString &nick, const QSet<QString> &groups) {
	qDebug() << "adding" << jid << nick;

	ContactItem *new_item = new ContactItem(jid);
	new_item->setNick(nick);

	QString bare_jid;
	split_jid(jid, &bare_jid);
	m_contacts[bare_jid] = new_item;

	if (groups.empty()) {
		qDebug() << noGroupName;
		new_item->addToGroup(getGroup(noGroupName));
	} else
		foreach (const QString &groupName, groups) {
			qDebug() << groupName;
			new_item->addToGroup(getGroup(groupName));
		}

	reset();
	return new_item;
}

GroupItem *ItemModel::getGroup(const QString &name) {
	foreach (GroupItem *item, m_groups)
		if (item->groupName() == name)
			return item;
	GroupItem *new_item = new GroupItem(name);
	m_groups << new_item;
	return new_item;
}

ContactItem *ItemModel::getItemByJid(const QString &jid) {
	QString bare_jid;
	split_jid(jid, &bare_jid);
	return m_contacts[bare_jid];
}
