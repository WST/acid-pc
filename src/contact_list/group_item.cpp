#include "contact_item.h"
#include "item_model.h"

#include "group_item.h"

using namespace CL;

bool GroupItem::addContact(ContactItem *item) {
	if (m_contacts.indexOf(item) < 0) {
		int i;
		for (i = 0; i < m_contacts.size(); ++i)
			if (*m_contacts[i] < *item)
				break;
		m_contacts.insert(i, item);
		if (item->isOnline())
			++online_count;
		owner->contactAdded(this, i);
		owner->groupChanged(this);
		return true;
	}
	return false;
}

bool GroupItem::removeContact(ContactItem *item) {
	if (item->isOnline())
		--online_count;
	return m_contacts.removeOne(item);
}

void GroupItem::statusChanged(ContactItem *item) {
	int i;
	for (i = 0; i < m_contacts.size(); ++i)
		if (*m_contacts[i] < *item)
			break;
	int item_index = m_contacts.indexOf(item);
	if (i > item_index)
		--i;
	if (i != item_index) {
		m_contacts.move(item_index, i);
		owner->contactMoved(this, item_index, i);
	}

	int prev_online_count = online_count;
	online_count = 0;
	foreach (ContactItem *contact, m_contacts)
		if (contact->isOnline())
			++online_count;
	if (prev_online_count != online_count)
		owner->groupChanged(this);
}
