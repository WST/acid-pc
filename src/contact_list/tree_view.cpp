#include <QApplication>
#include <QMenu>
#include <QKeyEvent>

#include "../functions.h"
#include "tree_view.h"

using namespace CL;

TreeView::TreeView(QWidget* parent):QTreeView(parent) {
	action_chat = new QAction(QIcon(":/menu/balloons.png"), "Chat", this);
	action_profile = new QAction(QIcon(":/menu/card-address.png"), "View profile", this);
	action_remove = new QAction(QIcon(":/menu/cross.png"), "Remove contact", this);
	action_send_file = new QAction(QIcon(":/menu/disk.png"), "Send a file", this);
	action_call = new QAction(QIcon(":/menu/mobile-phone.png"), "Make a voice call", this);

	connect(this, SIGNAL(pressed(const QModelIndex&)), this, SLOT(mousePressed(const QModelIndex&)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(doubleClicked(const QModelIndex&)));
	connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));

	connect(action_chat, SIGNAL(triggered()), this, SLOT(showChatDialog_helper()));
	connect(action_profile, SIGNAL(triggered()), this, SLOT(showProfile_helper()));
	connect(action_remove, SIGNAL(triggered()), this, SLOT(removeContact_helper()));
	connect(action_call, SIGNAL(triggered()), this, SLOT(callHelper()));
	connect(action_send_file, SIGNAL(triggered()), this, SLOT(sendFile_helper()));

	setHeaderHidden(true);
	m_hideOfflineItems = true;
}

void TreeView::setHideOfflineItems(bool show) {
	if (m_hideOfflineItems != show) {
		m_hideOfflineItems = show;
		QModelIndex rootModelIndex;
		updateOfflineItems(rootModelIndex, rootModelIndex);
	}
}

void TreeView::updateOfflineItems(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
	/*
	 * Scenario A: to == from are invalid => we update the whole contact list
	 * Scenario B: to/from are groups => we update a range of groups
	 * Scenario C: to/from are contacts => we update a range of contacts
	 */

	int firstIndex = 0, lastIndex = 1 << 30;
	if (topLeft.isValid()) {
		firstIndex = topLeft.row();
	}
	if (bottomRight.isValid()) {
		lastIndex = bottomRight.row();
	}

	QModelIndex level0m = topLeft.isValid() ? topLeft.parent() : topLeft;
	// At this stage we have handled Scenario A and converted it to B with infinite ranges
	for (int level1i = firstIndex; level1i <= lastIndex; ++level1i) {
		QModelIndex level1m = model()->index(level1i, 0, level0m);
		if (!level1m.isValid()) {
			break;
		}
		const Item *level1t = itemFromIndex(level1m);
		bool hideLevel1Item = m_hideOfflineItems;

		if (level1t->childCount()) {
			// Scenario B
			int onlineChildrenCount = 0;
			for (int level2i = 0; ; ++level2i) {
				QModelIndex level2m = model()->index(level2i, 0, level1m);
				if (!level2m.isValid()) {
					break;
				}
				const ContactItem *level2t = static_cast<const ContactItem *>(itemFromIndex(level2m));
				if (level2t->isOnline()) {
					++onlineChildrenCount;
				}
				setRowHidden(level2i, level1m,
					m_hideOfflineItems && !level2t->isOnline());
			}

			if (hideLevel1Item && onlineChildrenCount) {
				hideLevel1Item = false;
			}
		} else {
			// Scenario C
			if (hideLevel1Item && static_cast<const ContactItem *>(level1t)->isOnline()) {
				hideLevel1Item = false;
			}
		}
		setRowHidden(level1i, level0m, hideLevel1Item);
	}
}


void TreeView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
	QTreeView::dataChanged(topLeft, bottomRight);
	// XXX: if (m_hideOfflineItems)
	updateOfflineItems(topLeft, bottomRight);
}

TreeView::~TreeView() {
	delete action_chat;
	delete action_profile;
	delete action_remove;
	delete action_send_file;
	delete action_call;
}

bool TreeView::event(QEvent* e) {
	return QTreeView::event(e);
}

void TreeView::mousePressed(const QModelIndex& index) {
	if (QApplication::mouseButtons() == Qt::RightButton) {
		QString bareJid = index.data().toString();
		QMenu menu(this);
		menu.addAction(action_chat);
		menu.setDefaultAction(action_chat);
		menu.addSeparator();
		menu.addAction(action_call);
		menu.addAction(action_send_file);
		menu.addSeparator();
		menu.addAction(action_profile);
		menu.addAction(action_remove);
		menu.exec(QCursor::pos());
	}
}

void TreeView::doubleClicked(const QModelIndex& index) {
	Q_UNUSED(index);
	action_chat->trigger();
}

void TreeView::clicked(const QModelIndex& index) {
	Q_UNUSED(index);
}

void TreeView::drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const {
	if (!index.parent().isValid()) {
		QImage plus(":/roster/plus.png"), minus(":/roster/minus.png");
		painter->drawImage(rect, isExpanded(index) ? minus : plus);
	}
}

const ContactItem *TreeView::selectedContact() const {
	const QModelIndexList &selected = selectedIndexes();
	if (selected.size()) {
		const Item *item = itemFromIndex(selected.at(0));
		return item->childCount() ? NULL : static_cast<const ContactItem *>(item);
	}
	return NULL;
}

void TreeView::showChatDialog_helper() {
	if (selectedContact()) {
		QString bareJid = selectedContact()->getBareJid();
		if(!bareJid.isEmpty())
			emit wannaShowChatDialog(bareJid, selectedContact()->getNick());
	}
}

void TreeView::showProfile_helper() {
	if (selectedContact()) {
		QString bareJid = selectedContact()->getBareJid();
		if (!bareJid.isEmpty()) {
			emit wannaShowProfile(bareJid);
		}
	}
}

void TreeView::keyPressEvent(QKeyEvent* event1) {
	if(event1->key() == Qt::Key_Return) {
		showChatDialog_helper();
	}
	QTreeView::keyPressEvent(event1);
}

void TreeView::removeContact_helper() {
	if (selectedContact()) {
		QString bareJid = selectedContact()->getBareJid();
		if (!bareJid.isEmpty()) {
			emit wannaRemoveContact(bareJid);
		}
	}
}

void TreeView::callHelper() {
	if (selectedContact()) {
		const ContactItem::ResourceStatus &rs =
			selectedContact()->getResource();
		if (rs.status) {
			QString full_jid = selectedContact()->getBareJid() + "/" + rs.resourceName;
			emit wannaMakeVoiceCall(full_jid);
		}
	}
}

void TreeView::sendFile_helper() {
	if (selectedContact()) {
		const ContactItem::ResourceStatus &rs =
			selectedContact()->getResource();
		if (rs.status) {
			QString full_jid = selectedContact()->getBareJid() + "/" + rs.resourceName;
			emit wannaSendFile(full_jid);
		}
	}
}
