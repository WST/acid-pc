#ifndef CL_GROUPITEM_H
#define CL_GROUPITEM_H

#include <QObject>

#include "contact_item.h"
#include "item.h"

namespace CL {
	class ContactItem;

	class GroupItem: public Item {
	public:
		explicit GroupItem(const QString &_groupName = "") : m_groupName(_groupName) {}

		void setGroupName(const QString &_value) { m_groupName = _value; }
		const QString &groupName() const { return m_groupName; }

		/*!
		  Returns user-friendly group name
		  */
		// TODO: add text: "(online_items / total_items)" or smth
		virtual QString getText() const { return m_groupName; }

		virtual QString getSubText() const { return QString(); }

		/*!
		  Basically, this should return icon '+' or '-'
		  */
		virtual const QIcon &getIcon() const { return m_icon; }

		/*!
		  Contacts in group
		  */
		virtual int childCount() const { return m_contacts.size(); }

		/*!
		  Adds a contact to group. Supposed to be used internally by ContactItem
		  */
		bool addContact(ContactItem *item);

		/*!
		  Removes a contact from group. Supposed to be used internally by ContactItem
		  */
		bool removeContact(ContactItem *item);

		const QList<ContactItem *> getContacts() const { return m_contacts; }

	private:
		QString m_groupName;
		QIcon m_icon;

		QList<ContactItem *> m_contacts;
	};
};

#endif // CL_GROUPITEM_H
