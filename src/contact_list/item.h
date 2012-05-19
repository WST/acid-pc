#ifndef CL_ITEM_H
#define CL_ITEM_H

#include <QString>
#include <QIcon>

namespace CL {
	/*!
		General item. This class must be inherited
		*/
	class Item: public QObject {
		Q_OBJECT
	public:
		/*!
			Main display text of an item
			*/
		virtual QString getText() const = 0;

		/*!
			Sub-text of an item (to be displayed as a tooltip or smth).
			Usually status message
			*/
		virtual QString getSubText() const = 0;

		/*!
			Display icon
			*/
		virtual const QIcon &getIcon() const = 0;

		/*!
			Counts item children.
			This is always non-zero for group (because empty groups do not exist)
			and always zero for contacts (because contacts can't have groups)
			*/
		virtual int childCount() const { return 0; }
	};
}

#endif // CL_ITEM_H
