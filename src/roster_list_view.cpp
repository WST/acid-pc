/*
 * Copyright (C) 2008-2010 The QXmpp developers
 *
 * Author:
 *	Manjeet Dahiya
 *
 * Source:
 *	http://code.google.com/p/qxmpp
 *
 * This file is a part of QXmpp library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 */


#include "roster_list_view.h"
#include "roster_item.h"
#include <QApplication>
#include <QMenu>
#include <QKeyEvent>

RosterListView::RosterListView(QWidget* parent):QTreeView(parent) {
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

	setHeaderHidden(true);
}

RosterListView::~RosterListView() {
    delete action_chat;
    delete action_profile;
    delete action_remove;
    delete action_send_file;
    delete action_call;
}

bool RosterListView::event(QEvent* e) {
	return QTreeView::event(e);
}

void RosterListView::mousePressed(const QModelIndex& index) {
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

void RosterListView::doubleClicked(const QModelIndex& index) {
    action_chat->trigger();
}

void RosterListView::clicked(const QModelIndex& index) {
}

QString RosterListView::selectedBareJid() {
	return (selectedIndexes().size() > 0) ? selectedIndexes().at(0).data(RosterItem::BareJid).toString() : "";
}

void RosterListView::showChatDialog_helper() {
    QString bareJid = selectedBareJid();
    if(!bareJid.isEmpty())
        emit showChatDialog(bareJid);
}

void RosterListView::showProfile_helper() {
    QString bareJid = selectedBareJid();
    if(!bareJid.isEmpty())
        emit showProfile(bareJid);
}

void RosterListView::keyPressEvent(QKeyEvent* event1) {
    if(event1->key() == Qt::Key_Return) {
        showChatDialog_helper();
    }
	QTreeView::keyPressEvent(event1);
}

void RosterListView::removeContact_helper() {
    QString bareJid = selectedBareJid();
    if(!bareJid.isEmpty())
        emit removeContact(bareJid);
}
