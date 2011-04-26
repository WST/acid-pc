#ifndef CL_ITEMMODEL_H
#define CL_ITEMMODEL_H

#include <QStandardItemModel>
#include "QXmppRosterManager.h"
#include "QXmppPresence.h"

#include "contact_item.h"
#include "group_item.h"

namespace CL {
	class ItemModel : public QAbstractItemModel {
	public:
		ItemModel(QObject* parent): QAbstractItemModel(parent), notInRosterGroupName("Not-in-roster"), noGroupName("General") {}

		ContactItem *getItemByJid(const QString &jid);

		/*!
		  Changes the presence of an item with full jid JID
		  If an item does not exist, adds it to not-in-roster group (unless notInRosterGroupName is empty)
		  */
		void setPresence(const QString &jid, const QXmppPresence &_value);

		/*!
		  Adds or removes an entry for jid, updating it's state if needed
		  */
		void setEntry(const QString &jid, const QXmppRosterIq::Item &_value);

		void setAvatar(const QString &jid, const QImage &_value);
		void setNick(const QString &jid, const QString &_value);

		void removeEntry(const QString &jid);
		void clear();

		/*!
		  Gets an existing group from list (by name) or creates a new group
		  */
		GroupItem *getGroup(const QString &name);

		QString notInRosterGroupName;
		QString noGroupName;

	public:
		virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
		virtual QModelIndex parent(const QModelIndex &child) const;
		virtual int rowCount(const QModelIndex &parent) const;
		virtual int columnCount(const QModelIndex &parent) const;
		virtual QVariant data(const QModelIndex &index, int role) const;
		virtual Qt::ItemFlags flags(const QModelIndex &index) const;

	private:
		QMap<QString, ContactItem*> m_contacts;
		QList<GroupItem *> m_groups;

		struct TreeItem {
			GroupItem *group;
			ContactItem *contact;
		};

		/*!
		  Adds non-existent item to roster
		  */
		ContactItem *addItem(const QString &jid, const QString &nick);
	};
};

#endif // CL_ITEMMODEL_H
