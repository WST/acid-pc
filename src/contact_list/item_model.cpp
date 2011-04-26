#include <QDebug>

#include "../functions.h"

#include "item_model.h"

using namespace CL;

int ItemModel::columnCount(const QModelIndex &parent) const {
	Q_UNUSED(parent);
	return 1;
}

int ItemModel::rowCount(const QModelIndex &parent) const {
	int rc;
	if (parent.isValid())
		rc = static_cast<GroupItem *>(parent.internalPointer())->childCount();
	else
		rc = m_groups.size();
	return rc;
}

QVariant ItemModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();

	Item* item = static_cast<Item *>(index.internalPointer());
	switch (role) {
	case Qt::DisplayRole:
		return item->getText();
	case Qt::DecorationRole:
		return item->getIcon();
	case Qt::ToolTipRole:
		return item->getSubText();
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
		GroupItem *group = static_cast<GroupItem *>(parent.internalPointer());
		if (group && group->childCount() > row)
			return createIndex(row, column, group->getContacts().at(row));
	} else {
		if (m_groups.size() > row)
			return createIndex(row, column, m_groups.at(row));
	}

	return QModelIndex();
}

QModelIndex ItemModel::parent(const QModelIndex &child) const {
	Item *item = static_cast<Item *>(child.internalPointer());
	if (item->childCount()) {
		return QModelIndex();
	} else {
		// TODO: lol
		ContactItem *contact = static_cast<ContactItem *>(item);
		GroupItem *group;
		if (contact->getGroups().size())
			group = contact->getGroups().at(0);
		else
			foreach (GroupItem *gitem, m_groups) {
				if (gitem->groupName() == noGroupName)
					group = gitem;
					break;
				}
		return createIndex(m_groups.indexOf(group), 0, group);
	}
}


void ItemModel::setPresence(const QString &jid, const QXmppPresence &_value) {
	ContactItem *item = getItemByJid(jid);
	if (item)
		item->addJid(jid);
	else
		item = addItem(jid, QString());
	item->setPresenceType(_value.type());
	item->setStatusType(_value.status().type());
	item->setStatusText(_value.status().statusText());
}

ContactItem *ItemModel::addItem(const QString &jid, const QString &nick) {
	ContactItem *new_item = new ContactItem(jid);
	new_item->setNick(nick);
	QString bare_jid;
	split_jid(jid, &bare_jid);
	m_contacts[bare_jid] = new_item;
	reset();
	return new_item;
}

void ItemModel::setEntry(const QString &jid, const QXmppRosterIq::Item &_value) {
	if (_value.subscriptionType() == QXmppRosterIq::Item::Remove) {
		// remove bare(jid) from roster
	} else {
		ContactItem *item = getItemByJid(jid);
		if (item)
			item->addJid(jid);
		else
			item = addItem(jid, _value.name());

		if (_value.groups().empty())
			item->addToGroup(getGroup(noGroupName));
		else
			foreach (const QString &groupName, _value.groups())
				item->addToGroup(getGroup(groupName));
	}
	reset();
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
