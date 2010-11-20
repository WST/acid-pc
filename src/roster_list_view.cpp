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

RosterListView::RosterListView(QWidget* parent):QListView(parent), m_chat("Chat", this),
m_profile("View Profile", this), m_removeContact("Remove", this)
{
    bool check = connect(this, SIGNAL(pressed(const QModelIndex&)), this,
                         SLOT(mousePressed(const QModelIndex&)));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this,
                         SLOT(doubleClicked(const QModelIndex&)));
    Q_ASSERT(check);
    check = connect(this, SIGNAL(clicked(const QModelIndex&)), this,
                         SLOT(clicked(const QModelIndex&)));
    Q_ASSERT(check);
    check = connect(&m_chat, SIGNAL(triggered()), this,
                         SLOT(showChatDialog_helper()));
    Q_ASSERT(check);

    check = connect(&m_profile, SIGNAL(triggered()), this,
                         SLOT(showProfile_helper()));
    Q_ASSERT(check);

    check = connect(&m_removeContact, SIGNAL(triggered()), this,
                         SLOT(removeContact_helper()));
    Q_ASSERT(check);
}

bool RosterListView::event(QEvent* e)
{
    return QListView::event(e);
}

void RosterListView::mousePressed(const QModelIndex& index)
{
    if(QApplication::mouseButtons() == Qt::RightButton)
    {
        QString bareJid = index.data().toString();
        QMenu menu(this);
        menu.addAction(&m_chat);
        menu.setDefaultAction(&m_chat);
        menu.addAction(&m_profile);
        menu.addAction(&m_removeContact);
        menu.exec(QCursor::pos());
    }
}

void RosterListView::doubleClicked(const QModelIndex& index)
{
    m_chat.trigger();
}

void RosterListView::clicked(const QModelIndex& index)
{
}

QString RosterListView::selectedBareJid()
{
    if(selectedIndexes().size() > 0)
        return selectedIndexes().at(0).data(RosterItem::BareJid).toString();
    else
        return "";
}

void RosterListView::showChatDialog_helper()
{
    QString bareJid = selectedBareJid();
    if(!bareJid.isEmpty())
        emit showChatDialog(bareJid);
}

void RosterListView::showProfile_helper()
{
    QString bareJid = selectedBareJid();
    if(!bareJid.isEmpty())
        emit showProfile(bareJid);
}

void RosterListView::keyPressEvent(QKeyEvent* event1)
{
    if(event1->key() == Qt::Key_Return)
    {
        showChatDialog_helper();
    }
    QListView::keyPressEvent(event1);
}

void RosterListView::removeContact_helper()
{
    QString bareJid = selectedBareJid();
    if(!bareJid.isEmpty())
        emit removeContact(bareJid);
}
