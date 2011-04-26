#include "group_item.h"

using namespace CL;

bool GroupItem::addContact(ContactItem *item) {
	if (m_contacts.indexOf(item) < 0) {
		m_contacts << item;
		return true;
	}
	return false;
}

bool GroupItem::removeContact(ContactItem *item) {
	return m_contacts.removeOne(item);
}
