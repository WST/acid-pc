#include <QApplication>
#include <QMenu>
#include <QKeyEvent>

#include "../functions.h"
#include "tree_view.h"

using namespace CL;

TreeView::TreeView(QWidget* parent):QTreeView(parent) {
    action_chat = new QAction(QIcon(":/menu/balloons.png"), tr("Chat"), this);
    action_profile = new QAction(QIcon(":/menu/card-address.png"), tr("View profile"), this);
    action_remove = new QAction(QIcon(":/menu/cross.png"), tr("Remove contact"), this);
    action_send_file = new QAction(QIcon(":/menu/disk.png"), tr("Send a file"), this);
    action_call = new QAction(QIcon(":/menu/mobile-phone.png"), tr("Make a voice call"), this);

	connect(this, SIGNAL(pressed(const QModelIndex&)), this, SLOT(mousePressed(const QModelIndex&)));
	connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(doubleClicked(const QModelIndex&)));
	connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));

	connect(action_chat, SIGNAL(triggered()), this, SLOT(showChatDialog_helper()));
	connect(action_profile, SIGNAL(triggered()), this, SLOT(showProfile_helper()));
	connect(action_remove, SIGNAL(triggered()), this, SLOT(removeContact_helper()));
	connect(action_call, SIGNAL(triggered()), this, SLOT(callHelper()));
	connect(action_send_file, SIGNAL(triggered()), this, SLOT(sendFile_helper()));

	setHeaderHidden(true);
	m_hideOfflineItems = false;
}

void TreeView::setHideOfflineItems(bool show) {
	if (m_hideOfflineItems != show) {
		m_hideOfflineItems = show;
		QModelIndex rootModelIndex;
		updateOfflineItems(rootModelIndex, rootModelIndex);
	}
}

void TreeView::setThrottleNotInRoster(bool throttle) {
	// TODO
}

void TreeView::updateOfflineItems(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
	/*
	 * Scenario A: to == from are invalid => we update the whole contact list
	 * Scenario B: to/from are groups => we update a range of groups
	 * Scenario C: to/from are contacts => we update a signle group
	 */

	int groupIndexStart = 0, groupIndexEnd = 1 << 30;

	if (topLeft.isValid()) {
		QModelIndex parentModelIndex = topLeft.parent();
		if (parentModelIndex.isValid()) {
			groupIndexStart = parentModelIndex.row();
		} else {
			groupIndexStart = topLeft.row();
		}
	}

	if (bottomRight.isValid()) {
		QModelIndex parentModelIndex = bottomRight.parent();
		if (parentModelIndex.isValid()) {
			groupIndexEnd = parentModelIndex.row();
		} else {
			groupIndexEnd = bottomRight.row();
		}
	}

	QModelIndex rootModelIndex = QModelIndex();
	for (int groupIndex = groupIndexStart; groupIndex <= groupIndexEnd; ++groupIndex) {
		QModelIndex groupModelIndex = model()->index(groupIndex, 0, rootModelIndex);
		if (!groupModelIndex.isValid()) {
			break;
		}
		int onlineChildrenCount = 0;
		for (int contactIndex = 0; ; ++contactIndex) {
			QModelIndex contactModelIndex = model()->index(contactIndex, 0, groupModelIndex);
			if (!contactModelIndex.isValid()) {
				break;
			}
			const ContactItem *contactItem =
				static_cast<const ContactItem *>(itemFromIndex(contactModelIndex));
			if (contactItem->isOnline()) {
				++onlineChildrenCount;
			}
			setRowHidden(contactIndex, groupModelIndex, m_hideOfflineItems && !contactItem->isOnline());
		}
		setRowHidden(groupIndex, rootModelIndex, m_hideOfflineItems && !onlineChildrenCount);
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

ContactItem *TreeView::selectedContact() const {
	const QModelIndexList &selected = selectedIndexes();
	if (selected.size()) {
		Item *item = itemFromIndex(selected.at(0));
		return item->childCount() ? NULL : static_cast<ContactItem *>(item);
	}
	return NULL;
}

void TreeView::showChatDialog_helper() {
	if (selectedContact()) {
        emit wannaShowChatDialog(selectedContact());
	}
}

void TreeView::showProfile_helper() {
    if (selectedContact()) {
        emit wannaShowProfile(selectedContact());
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
        emit wannaRemoveContact(selectedContact());
    }
}

void TreeView::callHelper() {
	if (selectedContact()) {
        emit wannaMakeVoiceCall(selectedContact());
	}
}

void TreeView::sendFile_helper() {
	if (selectedContact()) {
        emit wannaSendFile(selectedContact());
	}
}
