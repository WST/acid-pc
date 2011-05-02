#include <QApplication>
#include <QMenu>
#include <QKeyEvent>

#include "../functions.h"
#include "item_model.h"
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

	setHeaderHidden(true);
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
    if(QApplication::mouseButtons() == Qt::RightButton) {
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
	unless (index.parent().isValid()) {
		QImage plus(":/roster/plus.png"), minus(":/roster/minus.png");
		painter->drawImage(rect, isExpanded(index) ? minus : plus);
	}
}

const ContactItem *TreeView::selectedContact() const {
	// a long long doomed chain
	return reinterpret_cast<const ContactItem *>(qvariant_cast<int>(selectedIndexes().at(0).data(ItemModel::ContactItemRole)));
}

const GroupItem *TreeView::selectedGroup() const {
	// fuck me gently with a chainsaw
	return reinterpret_cast<const GroupItem *>(qvariant_cast<int>(selectedIndexes().at(0).data(ItemModel::GroupItemRole)));
}

void TreeView::showChatDialog_helper() {
	if (selectedContact()) {
		QString bareJid = selectedContact()->getBareJid();
		if(!bareJid.isEmpty())
			emit showChatDialog(bareJid);
	}
}

void TreeView::showProfile_helper() {
	if (selectedContact()) {
		QString bareJid = selectedContact()->getBareJid();
		if(!bareJid.isEmpty())
			emit showProfile(bareJid);
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
		if(!bareJid.isEmpty())
			emit removeContact(bareJid);
	}
}

void TreeView::callHelper() {
	if (selectedContact()) {
		ContactItem::ResourceStatus status = selectedContact()->getResource();
		if(status.second) {
			QString full_jid = selectedContact()->getBareJid() + "/" + status.first;
			emit makeVoiceCall(full_jid);
		}
	}
}
