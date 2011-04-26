#ifndef CL_ITEMDELEGATE_H
#define CL_ITEMDELEGATE_H

#include <QItemDelegate>
#include <QSize>

namespace CL {
	class ItemDelegate : public QItemDelegate {
		Q_OBJECT
	public:
		explicit ItemDelegate(QObject *parent = 0) : QItemDelegate(parent) {}

		QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
		void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	};
};

#endif // CL_ITEMDELEGATE_H
