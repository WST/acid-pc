#ifndef QXMPP_BRIDGE_H
#define QXMPP_BRIDGE_H

#include <QObject>
#include <qxmpp/QXmppPresence.h>
#include <qxmpp/QXmppRosterManager.h>
#include <qxmpp/QXmppMucManager.h>

#include "contact_item.h"

namespace CL {
	class QXmppBridge: public QObject {
		Q_OBJECT

	public:
		explicit QXmppBridge(ItemModel *model, QXmppRosterManager *rosterManager);
		explicit QXmppBridge(ItemModel *model, QXmppMucRoom *mucRoom);

	public slots:
		void rosterEntryAdded(const QString &);
		void rosterEntryChanged(const QString &);
		void rosterEntryRemoved(const QString &);
		void rosterEntryPresence(const QString &, const QString &);
		void rosterSynchronize();

		// XXX: this seems overwhelming
		void mucEntryAdded(const QString &);
		void mucEntryChanged(const QString &);
		void mucEntryRemoved(const QString &);
		void mucEntryPresence(const QString &); /* deprecated? */

	private:
		ItemModel *m_model;
		QXmppRosterManager *m_rosterManager;
		QXmppMucRoom *m_mucRoom;

	public:
		static ContactItem::Status qxmpp2cl(const QXmppPresence &presence);

	private:
		static ContactItem::StatusType qxmpp2cl(const QXmppPresence::Status::Type type);
	};
};

#endif // QXMPP_BRIDGE_H
