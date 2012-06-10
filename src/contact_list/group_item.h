#ifndef CL_GROUPITEM_H
#define CL_GROUPITEM_H

#include <QMetaType>
#include "item.h"

namespace CL {
	class ContactItem;
	class ItemModel;

	class GroupItem: public Item {
		Q_OBJECT

	public:
		explicit GroupItem(ItemModel *_owner, const QString &_groupName = ""):
			owner(_owner), m_groupName(_groupName), onlineCount(0) {}

		void setGroupName(const QString &_value) { m_groupName = _value; }
		const QString &getGroupName() const { return m_groupName; }

		/*!
			Returns user-friendly group name
			*/
		virtual QString getText() const {
			return QString("%1 (%2/%3)").
				arg(m_groupName).
				arg(onlineCount).
				arg(m_contacts.size());
		}

		/*!
			Provides some detail about group items
			*/
		virtual QString getSubText() const { return ""; }

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

		const QList<ContactItem *> &getContacts() const { return m_contacts; }

	public:
		void statusChanged(ContactItem *item);

	signals:
		/*!
			Emitted when the group is removed.
			The group is destroyed automatically when the last item is removed.
			*/
		void removed();

	private:
		ItemModel *owner;
		QString m_groupName;
		QIcon m_icon;
		QList<ContactItem *> m_contacts;
		int onlineCount;
	};

};

#endif // CL_GROUPITEM_H
