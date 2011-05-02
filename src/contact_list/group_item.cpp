#include "contact_item.h"

#include "group_item.h"

using namespace CL;

bool GroupItem::addContact(ContactItem *item) {
	if (m_contacts.indexOf(item) < 0) {
		int i;
		for (i = 0; i < m_contacts.size(); ++i)
			if (*m_contacts[i] < *item)
				break;
		m_contacts.insert(i, item);
		//emit itemAdded(i);
		return true;
	}
	return false;
}

bool GroupItem::removeContact(ContactItem *item) {
	return m_contacts.removeOne(item);
}

void GroupItem::statusChanged(ContactItem *item) {
	int i;
	for (i = 0; i < m_contacts.size(); ++i)
		if (*m_contacts[i] < *item)
			break;
	int item_index = m_contacts.indexOf(item);
	if (i < item_index) {
		m_contacts.move(item_index, i);
		//emit itemMoved(item_index, i);
	} elsif (i > item_index) {
		m_contacts.move(item_index, i-1);
		//emit itemMoved(item_index, i-1);
	}
}

QString GroupItem::getSubText() const {
	int online_count = 0;
	foreach (const ContactItem *contact, m_contacts)
		if (contact->isOnline())
			++online_count;
	return QString("%1 / %2").arg(online_count).arg(m_contacts.size());
}
