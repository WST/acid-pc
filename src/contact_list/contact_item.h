#ifndef CL_CONTACTITEM_H
#define CL_CONTACTITEM_H

#include <QItemDelegate>
#include <QPainter>
#include <QSet>

#include "QXmppPresence.h"

#include "item.h"
#include "group_item.h"

namespace CL {
	class GroupItem;

	/*!
	  Roster item. Just an entry in a roster tree.
	  Typically there is only one instance of item per JID,
	  but when it comes to 'single item in multiple groups', we duplicate items as hell
	  */
	class ContactItem: public Item {
	public:
		explicit ContactItem(const QString &jid);

		/*!
		  Adds a JID to the roster item.
		  JID is being splitted into bare jid and resource. Resource is appended to getResources() set
		  Since item's bare jid of is being replaced, be aware about assigning wrong JID
		  */
		void addJid(const QString &jid);

		/*!
		  'Removes' a JID from the roster item
		  In no circumstances this will cause an error or change item's bare jid
		  */
		void dropJid(const QString &jid);

		/*!
		  Gets bare jid of an item, as set by addJid or initial constructor
		  */
		const QString &getBareJid() const { return m_bareJid; }

		/*!
		  Gets all resources added by addJid and not yet dropped by dropJid
		  */
		const QSet<QString> &getResources() const { return m_resources; }

		const QString &getStatusText() const { return m_statusText; }
		void setStatusText(const QString &_value) { m_statusText = _value; }

		const QString &getNick() const { return m_nick; }
		void setNick(const QString &_value) { m_nick = _value; }

		const QList<GroupItem *> &getGroups() const { return m_groups; }
		QList<GroupItem *> &getGroups() { return m_groups; }

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

		QXmppPresence::Status::Type getStatusType() const { return m_statusType; }
		void setStatusType(QXmppPresence::Status::Type _value);

		QXmppPresence::Type getPresenceType() const { return m_presenceType; }
		void setPresenceType(QXmppPresence::Type _value);

		const QImage &getAvatar() const { return m_avatar; }
		void setAvatar(const QImage& _value) { m_avatar = _value; }

		/*!
		  Returns user-friendly text string to display as a name
		  */
		virtual QString getText() const;

		/*!
		  In this case subtext is a status message
		  */
		virtual QString getSubText() const { return getStatusText(); }

		/*!
		  In this case icon normally indicates user status type
		  */
		virtual const QIcon &getIcon() const { return m_icon; }

	public slots:
		/*!
		  Updates text() (aka Qt::DisplayRole)
		  */
		void updateText();

	private:
		QSet<QString> m_resources;
		QString m_bareJid;
		QString m_statusText;
		QString m_nick;
		QList<GroupItem *> m_groups;
		QXmppPresence::Status::Type m_statusType;
		QXmppPresence::Type m_presenceType;
		QImage m_avatar;
		QIcon m_icon;
	};
};

#endif // CL_CONTACTITEM_H
