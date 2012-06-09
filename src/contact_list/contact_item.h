#ifndef CL_CONTACTITEM_H
#define CL_CONTACTITEM_H

#include <QMap>
#include <QMetaType>

#include "../functions.h"
#include "item.h"

namespace CL {
	class GroupItem;
	class ItemModel;

	/*!
		Roster item. Just an entry in a roster tree.
		There is only one instance of item per JID
		*/
	class ContactItem: public Item {
		Q_OBJECT

	public:
		explicit ContactItem(ItemModel *_owner, const QString &_jid);

		/*!
			Gets bare jid of an item, as set by addJid or initial constructor
			*/
		const QString &getBareJid() const { return m_bareJid; }

	public:
		enum StatusType {
			Unchanged = -1, /* This value is for internal usage only */
			Offline,
			DND,
			XA,
			Away,
			Online,
			Chat
		};

		static const char *statusString[];
		static const char *statusColor[];

		struct Status {
			StatusType type;
			QString text;
			int priority;

			explicit Status(): type(Offline), priority(0) {}

			/*!
				For suitable resource search.
				Note: only resources of the same jid should be compared with this operator (because of priority)
				*/
			bool operator<(const Status &_other) const {
				return priority < _other.priority || (priority == _other.priority && type < _other.type);
			}
		};

		/*!
			Greater items are more attractive target for converstation
			*/
		bool operator<(const ContactItem &_other) const;

	public:
		/*!
			Gets a list of all online resources. Empty list indicates no resources (basically, JID is offline)
			*/
		QList<QString> getResourceNames() const { return m_resources.keys(); }

		typedef struct {
			QString resourceName;
			Status *status;
		} ResourceStatus;

		/*!
			Gets the name and status by resource name.
			For empty name returns status of 'the most online' resource (highest priority, highest status).
			Returns NULL for non-existent resource
			*/
		ResourceStatus getResource(const QString &resource = QString()) const;

		/*!
			Adds / removes / updates resources as needed
			*/
		void setResourceStatus(const QString &resource, const Status &_value);

		const QString &getNick() const { return m_nick.isEmpty() ? m_bareJid : m_nick; }
		void setNick(const QString &_value);

		/*!
			Returns true if at least one resource is not offline
			*/
		bool isOnline() const { return !m_resources.isEmpty(); }

		/*!
			Sets internal state to 'offline', consequently removing all resources
			*/
		void setOffline();

		/*!
			Adds a group to internal groups list (if needed) and updates group's children
			*/
		bool addToGroup(GroupItem *group);

		/*!
			Removes a group from internal groups list (if needed) and updates group's children
			*/
		bool removeFromGroup(GroupItem *group);

		/*!
			Same as another version, but searches for the group by name
			*/
		bool removeFromGroup(const QString &group_name);

		/*!
			Remove a contact from all groups
			*/
		void remove();

		/*!
			Returns a list of all groups item is presented in
			*/
		const QList<GroupItem *> &getGroups() const { return m_groups; }

		/*!
			Returns user-friendly text string to display as a name
			*/
		virtual QString getText() const;

		/*!
			Describes general status
			*/
		virtual QString getSubText() const;

		/*!
			In this case icon normally indicates user status type
			*/
		virtual const QIcon &getIcon() const { return m_icon; }

		/*!
			Sets the notification status for a roster item.
			If @reason is given, it will be displayed with getSubText
			*/
		void setNotified(bool notified, const QString &text = QString());

		virtual bool notified() const { return m_notified; }
	
	signals:
		/*!
			Contact icon has changed.
			Emitted after the actual change is rendered to the roster
			*/
		void iconChanged(const QIcon &new_icon);
	
		/*!
			Contact nickname has changed.
			Emitted after the actual change is rendered to the roster
			*/
		void nickChanged(const QString &new_nick);

		/*!
			Contact has been removed from all groups.
			Emitted after the change is rendered, but BEFORE the item is destroyed,
			i.e. you can still read the properties.
			*/
		void removed();

	private:
		ItemModel *owner;
		QMap<QString, Status *> m_resources;
		QList<GroupItem *> m_groups;
		QString m_bareJid;
		QString m_nick;
		QIcon m_icon;
		QString icon_name;
		bool m_notified;
		QString m_notification;

		void updateIcon();
		void changeStatus();
	};

};

#endif // CL_CONTACTITEM_H
