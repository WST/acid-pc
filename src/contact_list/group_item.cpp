#include "contact_item.h"
#include "item_model.h"

#include "group_item.h"

using namespace CL;

bool GroupItem::addContact(ContactItem *item) {
	if (m_contacts.indexOf(item) < 0) {
		int i;
		for (i = 0; i < m_contacts.size(); ++i) {
			if (*m_contacts[i] < *item) {
				break;
			}
		}
		m_contacts.insert(i, item);
		if (item->isOnline()) {
			++onlineCount;
		}
		owner->contactAdded(this, i);
		owner->groupChanged(this);
		return true;
	}
	return false;
}

bool GroupItem::removeContact(ContactItem *item) {
	if (item->isOnline()) {
		--onlineCount;
	}
	// XXX: remove a group if it's empty
	return m_contacts.removeOne(item);
}

void GroupItem::statusChanged(ContactItem *item) {
	int i;
	for (i = 0; i < m_contacts.size(); ++i) {
		if (*m_contacts[i] < *item) {
			break;
		}
	}

	int itemIndex = m_contacts.indexOf(item);
	if (i > itemIndex) {
		--i;
	}
	if (i != itemIndex) {
		m_contacts.move(itemIndex, i);
		owner->contactMoved(this, itemIndex, i);
	}

	int prevOnlineCount = onlineCount;
	onlineCount = 0;
	foreach (ContactItem *contact, m_contacts) {
		if (contact->isOnline()) {
			++onlineCount;
		}
	}

	if (prevOnlineCount != onlineCount) {
		owner->groupChanged(this);
	}
}
