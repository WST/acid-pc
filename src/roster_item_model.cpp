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


#include "roster_item_model.h"

#include <QDebug>

RosterItemModel::RosterItemModel(QObject* parent): QStandardItemModel(parent) {

}

RosterItem* RosterItemModel::getRosterItemFromBareJid(const QString& bareJid)
{
    if(m_jidRosterItemMap.contains(bareJid))
        return m_jidRosterItemMap[bareJid];
    else
        return 0;
}


RosterItem *RosterItemModel::addRosterItemIfDontExist(const QString &bare_jid, const QString &nick, const QString &group) {
	if(!m_jidRosterItemMap.contains(bare_jid)) {
		RosterItem* item = new RosterItem(bare_jid);
		m_jidRosterItemMap[bare_jid] = item;
		appendRow(item); // adding a new row
		item->setBareJid(bare_jid);
		item->setName(nick);
		return item;
	} else {
		RosterItem *item = getRosterItemFromBareJid(bare_jid);
		item->setName(nick);
		return item;
	}
}

void RosterItemModel::updatePresence(const QString& bare_jid, const QMap<QString, QXmppPresence>& presences) {
	RosterItem *item = getRosterItemFromBareJid(bare_jid);
	if(item == 0) {
		return;
	}

	if(presences.count() > 0) {
        QString statusText = presences.begin().value().status().statusText();
        QXmppPresence::Status::Type statusType = presences.begin().value().status().type();
        QXmppPresence::Type presenceType = presences.begin().value().type();

		item->setStatusText(statusText);
		item->setStatusType(statusType);
		item->setPresenceType(presenceType);
    }
}

void RosterItemModel::updateRosterEntry(const QString& bareJid, const QXmppRosterIq::Item& rosterEntry) {
	QString name = rosterEntry.name();

	RosterItem *item = addRosterItemIfDontExist(bareJid, name, "foo_group");
	item->setName(name);
	// TODO другое (status, client, etc)
}

void RosterItemModel::updateAvatar(const QString& bareJid, const QImage& image) {
	RosterItem *item = getRosterItemFromBareJid(bareJid);
	if(item == 0) {
		return;
	}
	item->setAvatar(image);
}

void RosterItemModel::updateName(const QString& bareJid, const QString& name) {
	RosterItem *item = getRosterItemFromBareJid(bareJid);
	if(item == 0) {
		return;
	}

	if(name.isEmpty()) {
        return;
	}

	item->setName(name);
}

void RosterItemModel::clear() {
    QStandardItemModel::clear();
    m_jidRosterItemMap.clear();
}

void RosterItemModel::removeRosterEntry(const QString& bareJid) {
    RosterItem* item = getRosterItemFromBareJid(bareJid);
	if(item) {
        removeRow(item->row());
    }
}
