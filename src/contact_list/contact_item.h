#ifndef CL_CONTACTITEM_H
#define CL_CONTACTITEM_H

#include <QMap>
#include <QDebug>

#include "item.h"
#include "group_item.h"

namespace CL {
	class GroupItem;

	/*!
	  Roster item. Just an entry in a roster tree.
	  There is only one instance of item per JID
	  */
	class ContactItem: public Item {
	public:
		explicit ContactItem(const QString &jid);

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

		struct Status {
			StatusType type;
			QString text;
			int priority;

			explicit Status(): type(Offline), text(""), priority(0) {}

			/*!
			  For suitable resource search
			  */
			bool operator<(const Status &_other) const {
				return priority < _other.priority ? true :
					type < _other.type;
			}
		};

	public:
		/*!
		  Gets a list of all online resources. Empty list indicates no resources (basically, JID is offline)
		  */
		QList<QString> getResourceNames() const { return m_resources.keys(); }

		/*!
		  Gets the status by resource name. For empty name returns status with the highest priority.
		  If there are several resources with the same priority, returns one with most available status
		  */
		const Status *getResourceStatus(const QString &resource = "") const;

		/*!
		  Adds / removes / updates resources as needed
		  */
		void setResourceStatus(const QString &resource, const Status &_value);

		const QString &getNick() const { return m_nick; }
		void setNick(const QString &_value) { m_nick = _value; }

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
		  Removes a group from internal groups list (if needed) and update group's children
		  */
		bool removeFromGroup(GroupItem *group);

		/*!
		  Same as previous version, but searches for the group by name
		  */
		bool removeFromGroup(const QString &group_name);

		/*!
		  Returns a list of all groups item is presented in
		  */
		const QList<GroupItem *> &getGroups() const { return m_groups; }

		/*!
		  Returns user-friendly text string to display as a name
		  */
		virtual QString getText() const;

		/*!
		  In this case subtext is a status message
		  */
		virtual QString getSubText() const { return isOnline() ? getResourceStatus()->text : QString(); }

		/*!
		  In this case icon normally indicates user status type
		  */
		virtual const QIcon &getIcon() const { return m_icon; }

	private:
		QMap<QString, Status *> m_resources;
		QList<GroupItem *> m_groups;
		QString m_bareJid;
		QString m_nick;
		QIcon m_icon;

		void updateIcon();
	};
};

#endif // CL_CONTACTITEM_H
