#ifndef CL_ITEMMODEL_H
#define CL_ITEMMODEL_H

#include <QAbstractItemModel>

#include "contact_item.h"
#include "group_item.h"

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
		ContactItem *addEntry(const QString &jid, const QString &nick, const QSet<QString> &groups);

		/*!
		  Removes an entry for jid, updating groups as needed
		  Passing groups = NULL removes this entry from all groups
		  */
		void removeEntry(const QString &jid, const QSet<QString> *groups = NULL);


		void setAvatar(const QString &jid, const QImage &_value);
		void setNick(const QString &jid, const QString &_value);

		void removeEntry(const QString &jid);
		void clear();

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

	public:
		enum SpecificRole {
			BareJid = Qt::UserRole + 2
		};

		virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
		virtual QModelIndex parent(const QModelIndex &child) const;
		virtual int rowCount(const QModelIndex &parent) const;
		virtual int columnCount(const QModelIndex &parent) const;
		virtual QVariant data(const QModelIndex &index, int role) const;
		virtual Qt::ItemFlags flags(const QModelIndex &index) const;

	private:
		QMap<QString, ContactItem*> m_contacts;
		QList<GroupItem *> m_groups;

		union ItemID {
			struct {
				unsigned short group;
				unsigned short contact;
			};
			unsigned int id;
		};

		/*!
		  Adds non-existent item to roster
		  */
		ContactItem *addItem(const QString &jid, const QString &nick);
	};
};

#endif // CL_ITEMMODEL_H
