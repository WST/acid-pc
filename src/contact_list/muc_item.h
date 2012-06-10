#ifndef CL_MUCITEM_H
#define CL_MUCITEM_H

#include "contact_item.h"

namespace CL {
	/*!
		Muc item. Represents a single resource in the room.
		There is only one instance of item per nick.
		*/
	class MucItem: public ContactItem {
		Q_OBJECT
	};

};

#endif // CL_MUCITEM_H
