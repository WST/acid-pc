#include <QPainter>

#include "item_model.h"

#include "item_delegate.h"

using namespace CL;

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
	return index.parent().isValid() ? QSize(44, 20) : QSize(44, 16);
}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	painter->setRenderHint(QPainter::TextAntialiasing);

	QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
	QPixmap pixmap = icon.pixmap(QSize(16, 16), QIcon::Normal, QIcon::On);

	QColor selected_bg(60, 60, 210);

	QRect rect = option.rect;

	if (option.state & QStyle::State_Selected) {
		if (index.parent().isValid()) {
			rect.setLeft(0);
		}
		painter->fillRect(rect, selected_bg);
	}

	rect = option.rect;
	if (index.parent().isValid()) {
		rect.setLeft(0);
	}

	rect.setWidth(pixmap.width());
	rect.setHeight(pixmap.height());
	rect.moveTop(rect.y() + (option.rect.height() - pixmap.height())/2);
	rect.moveLeft(rect.left() + 10);
	painter->drawPixmap(rect, pixmap);

	rect = option.rect;

	if (index.parent().isValid()) {
		rect.setLeft(10);
	}

	rect.setLeft(rect.x() + pixmap.width() + 3);
	rect.moveTop(rect.y() + 3);

	// XXX: you know it.
	QFont font = painter->font();
	if (index.data(ItemModel::BlinkingRole).toBool()) {
		font.setBold(true);
		painter->setFont(font);
	}

	painter->drawText(rect, index.data(Qt::DisplayRole).toString());

	font.setBold(false);
	painter->setFont(font);
}
