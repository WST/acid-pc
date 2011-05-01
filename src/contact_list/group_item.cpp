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

bool contactLessThan(const ContactItem *c1, const ContactItem *c2)
{
	return c1->isOnline() > c2->isOnline() ? true :
		c1->isOnline() < c2->isOnline() ? false :
		c1->getText().compare(c2->getText(), Qt::CaseInsensitive) < 0;
}

void GroupItem::sort() {
	qSort(m_contacts.begin(), m_contacts.end(), &contactLessThan);
}
