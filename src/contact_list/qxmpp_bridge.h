#ifndef QXMPP_BRIDGE_H
#define QXMPP_BRIDGE_H

#include "qxmpp/QXmppPresence.h"

#include "contact_item.h"

namespace CL {
	class QXmppBridge {
	public:
		static ContactItem::Status qxmpp2cl(const QXmppPresence &presence);
	private:
		static ContactItem::StatusType qxmpp2cl(const QXmppPresence::Status::Type type);
	};
};

#endif // QXMPP_BRIDGE_H
