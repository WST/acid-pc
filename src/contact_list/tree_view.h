#ifndef CL_TREEVIEW_H
#define CL_TREEVIEW_H

#include <QtGui>
#include "contact_item.h"
#include "group_item.h"
#include "item_model.h"

namespace CL {
	class TreeView : public QTreeView {
		Q_OBJECT

	public:
		TreeView(QWidget* parent = 0);
		~TreeView();
		bool event(QEvent* e);
        void setNotified(const QString &jid, bool notified);

		virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
	public slots:
		void mousePressed(const QModelIndex& index);
		void doubleClicked(const QModelIndex& index);
		void clicked(const QModelIndex& index);

		/*!
			Shows or hides offline items, also setting a flag for the future changes
			*/
		void setHideOfflineItems(bool);

	private slots:
		void showChatDialog_helper();
		void showProfile_helper();
		void removeContact_helper();
		void callHelper();
        void sendFile_helper();

	protected:
		void keyPressEvent(QKeyEvent*);
		void drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const;

	signals:
        void wannaShowChatDialog(const ContactItem *item);
        void wannaShowProfile(const QString& bareJid);
        void wannaRemoveContact(const QString& bareJid);
        void wannaMakeVoiceCall(const QString &full_jid);
        void wannaSendFile(const QString &full_jid);

	public:
		/*!
			Returns selected roster item. NULL if no items are selected
			*/
		const ContactItem *selectedContact() const;

	private:
		QAction *action_chat;
		QAction *action_profile;
		QAction *action_remove;
		QAction *action_send_file;
		QAction *action_call;

		bool m_hideOfflineItems;
		const Item *itemFromIndex(const QModelIndex &index) const {
			return static_cast<const Item *>(index.data(ItemModel::ItemRole).value<const void *>());
		}
		void updateOfflineItems(const QModelIndex &topLeft, const QModelIndex &bottomRight);
	};
};

#endif // CL_TREEVIEW_H
