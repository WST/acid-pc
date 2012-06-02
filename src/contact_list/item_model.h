#ifndef CL_ITEMMODEL_H
#define CL_ITEMMODEL_H

#include <QAbstractItemModel>

#include "contact_item.h"
#include "group_item.h"

/*!
  This workaround makes changes of {begin,end}MoveRows manually, preventing crash in some cases
  */
#define MOVE_ROWS_WORKAROUND

namespace CL {
	class ItemModel: public QAbstractItemModel {
		Q_OBJECT

	public:
		explicit ItemModel(QObject* parent):
			QAbstractItemModel(parent),
			throttleNotInRoster(true),
			notInRosterGroupName("Not-in-roster"),
			noGroupName("General") {}

		/*!
		  Returns a contact by it's JID
		  */
		ContactItem *getContact(const QString &jid);

		/*!
		  Changes the status of an item with full jid JID
		  If an item does not exist, adds it to not-in-roster group (unless notInRosterGroupName is empty)
		  */
		void setStatus(const QString &jid, const ContactItem::Status &_value);

		/*!
		  Adds an entry for jid, updating it's groups as needed
		  */
		ContactItem *updateEntry(const QString &jid, const QString &nick, QSet<QString> groups);

		/*!
		  Removes an entry for jid from all groups
		  */
		void removeEntry(const QString &jid);

		/*!
		  Gets an existing group from list (by name) or creates a new group
		  */
		GroupItem *getGroup(const QString &name);

		/*!
		  Drop status if sender is not in roster
		  */
		bool throttleNotInRoster;

		/*!
		  Group name for new contacts not in roster
		  */
		QString notInRosterGroupName;

		/*!
		  Group name for new contacts without groups
		  */
		QString noGroupName;

	public: // QAbstractItemModel
		enum SpecificRole {
			ItemRole = Qt::UserRole + 2,
			BlinkingRole
		};

		virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
		virtual QModelIndex parent(const QModelIndex &child) const;
		virtual int rowCount(const QModelIndex &parent) const;
		virtual int columnCount(const QModelIndex &parent) const;
		virtual QVariant data(const QModelIndex &index, int role) const;
		virtual Qt::ItemFlags flags(const QModelIndex &index) const;

	public: // Event system
		void contactAdded(GroupItem *const sender, int ind);
		void contactMoved(GroupItem *const sender, int from, int to);
		void contactChanged(ContactItem *const sender);
		void groupChanged(GroupItem *const sender);

	private:
		QMap<QString, ContactItem*> m_contacts;
		QList<GroupItem *> m_groups;

		/*!
		  Returns index of contact (if groupID >= 0) or group
		  */
		QModelIndex index(int row, int column, int groupID = -1) const;

		/*!
		  Adds non-existent item to roster
		  */
		ContactItem *addItem(const QString &jid, const QString &nick);
	};
};

#endif // CL_ITEMMODEL_H
