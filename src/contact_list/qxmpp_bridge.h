#ifndef QXMPP_BRIDGE_H
#define QXMPP_BRIDGE_H

#include <QObject>
#include <qxmpp/QXmppPresence.h>
#include <qxmpp/QXmppRosterManager.h>

#include "contact_item.h"

namespace CL {
	class QXmppBridge: public QObject {
		Q_OBJECT

	public:
		explicit QXmppBridge(ItemModel *model,
				QXmppRosterManager *manager): m_model(model), m_manager(manager) {}

	public slots:
		void itemAdded(const QString &);
		void itemChanged(const QString &);
		void itemRemoved(const QString &);

	private:
		ItemModel *m_model;
		QXmppRosterManager *m_manager;

	public:
		static ContactItem::Status qxmpp2cl(const QXmppPresence &presence);

	private:
		static ContactItem::StatusType qxmpp2cl(const QXmppPresence::Status::Type type);
	};
};

#endif // QXMPP_BRIDGE_H
