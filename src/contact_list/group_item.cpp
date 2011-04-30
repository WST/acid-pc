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

QString GroupItem::getSubText() const {
	int online_count = 0;
	foreach (const ContactItem *contact, m_contacts)
		if (contact->isOnline())
			++online_count;
	return QString("%1 / %2").arg(online_count).arg(m_contacts.size());
}
