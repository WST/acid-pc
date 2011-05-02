#ifndef CL_CONTACTITEM_H
#define CL_CONTACTITEM_H

#include <QMap>
#include <QMetaType>

#include "../functions.h"
#include "item.h"

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

			explicit Status(): type(Offline), priority(0) {}

			/*!
			  For suitable resource search
			  */
			bool operator<(const Status &_other) const {
				return priority < _other.priority ? true :
					type < _other.type;
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

		typedef QPair<QString, const Status *> ResourceStatus;

		/*!
		  Gets the name and status by resource name.
		  For empty name returns status of 'the most online' resource (highest priority, highest status).
		  Returns NULL for non-existent resource
		  */
		ResourceStatus getResource(const QString &resource = "") const;

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
		  Removes a group from internal groups list (if needed) and updates group's children
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
		virtual QString getSubText() const { return isOnline() ? getResource().second->text : QString(); }

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

Q_DECLARE_METATYPE(const CL::ContactItem *)

#endif // CL_CONTACTITEM_H
