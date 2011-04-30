#ifndef CL_CONTACTITEM_H
#define CL_CONTACTITEM_H

#include <QMap>

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
			Online,
			Chat,
			DND,
			Away,
			XA
		};

		struct Status {
			StatusType type;
			QString text;
			int priority;

			explicit Status(): type(Offline), text(""), priority(0) {}
		};

	public:
		/*!
		  Gets a list of all online resources. Empty list indicates no resources (basically, JID is offline)
		  */
		QList<QString> getResources() const { return m_resources.keys(); }

		/*!
		  Gets the resource by it's name. For empty name returns a resource with the highest priority.
		  If there are several resources with the same priority, may return any
		  */
		const Status *getResource(const QString &name = "") const;

		const QString &getNick() const { return m_nick; }
		void setNick(const QString &_value) { m_nick = _value; }

		/*!
		  Returns true if at least one resource is not offline
		  */
		bool isOnline() const { return m_resources.size(); }

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

		const QImage &getAvatar() const { return m_avatar; }
		void setAvatar(const QImage& _value) { m_avatar = _value; }

		/*!
		  Returns user-friendly text string to display as a name
		  */
		virtual QString getText() const;

		/*!
		  In this case subtext is a status message
		  */
		virtual QString getSubText() const { return isOnline() ? getResource()->text : "offline"; }

		/*!
		  In this case icon normally indicates user status type
		  */
		virtual const QIcon &getIcon() const { return m_icon; }

		/*!
		  Adds / removes / updates resources as needed
		  */
		void updatePresence(const QString &resource, const Status &status);

	private:
		QMap<QString, Status *> m_resources;
		QString m_bareJid;
		QString m_nick;
		QList<GroupItem *> m_groups;
		QImage m_avatar;
		QIcon m_icon;

		void updateIcon();
	};
};

#endif // CL_CONTACTITEM_H
