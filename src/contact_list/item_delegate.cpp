#include <QPainter>

#include "item_delegate.h"

using namespace CL;

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
	if (index.parent().isValid())
		return QSize(44, 20);
	else
		return QSize(44, 16);
}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	painter->setRenderHint(QPainter::TextAntialiasing);

	QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
	QPixmap pixmap = icon.pixmap(QSize(16, 16), QIcon::Normal, QIcon::On);


	QColor selected_bg(30, 30, 220);

	QRect rect = option.rect;

	if (option.state & QStyle::State_Selected) {
		if (index.parent().isValid())
			rect.setLeft(0);
		painter->fillRect(option.rect, selected_bg);
	}

	rect = option.rect;
	if (index.parent().isValid())
		rect.setLeft(0);

	rect.setWidth(pixmap.width());
	rect.setHeight(pixmap.height());
	rect.moveTop(rect.y() + (option.rect.height() - pixmap.height())/2);
	rect.moveLeft(rect.left() + 10);
	painter->drawPixmap(rect, pixmap);

	rect = option.rect;

	if (index.parent().isValid())
		rect.setLeft(10);

	rect.setLeft(rect.x() + pixmap.width() + 3);
	rect.moveTop(rect.y() + 3);

	painter->drawText(rect, index.data(Qt::DisplayRole).toString());
}
