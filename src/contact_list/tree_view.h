#ifndef CL_TREEVIEW_H
#define CL_TREEVIEW_H

#include <QtGui>
#include "contact_item.h"
#include "group_item.h"

namespace CL {
	class TreeView : public QTreeView {
		Q_OBJECT

	public:
		TreeView(QWidget* parent = 0);
		~TreeView();
		bool event(QEvent* e);
        const QString getNickByJid(const QString &jid);
        void setBlinking(const QString &jid, bool blinking);

	public slots:
		void mousePressed(const QModelIndex& index);
		void doubleClicked(const QModelIndex& index);
		void clicked(const QModelIndex& index);

	private slots:
		void showChatDialog_helper();
		void showProfile_helper();
		void removeContact_helper();
		void callHelper();

	protected:
		void keyPressEvent(QKeyEvent*);
		void drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const;

	signals:
        void showChatDialog(const QString& bareJid, const QString &nick);
		void showProfile(const QString& bareJid);
		void removeContact(const QString& bareJid);
		void makeVoiceCall(const QString &full_jid);

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
	};
};

#endif // CL_TREEVIEW_H
