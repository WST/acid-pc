#include <QPainter>

#include "item_delegate.h"

using namespace CL;

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
	return QSize(44, 20);
}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	// Здесь определяется отрисовка элемента ростера
	painter->save();
	painter->setRenderHint(QPainter::TextAntialiasing);
	QVariant value = index.data(Qt::DecorationRole);

	QColor selectedBg(60, 140, 222);
	QColor alternateBg(239, 245, 254);
	QColor selectedText(Qt::white);

	QColor nameTextColor(Qt::black);
	QColor statusTextColor(Qt::darkGray);

	QPixmap pixmap;
	if(value.type() == QVariant::Icon)
	{
		QIcon icon = qvariant_cast<QIcon>(value);
		pixmap = icon.pixmap(QSize(16, 16), QIcon::Normal, QIcon::On);
	}

	QPen penDivision;
	if (index.row() % 2)
		painter->fillRect(option.rect, alternateBg);

	if (option.state & QStyle::State_Selected)
	{
		painter->fillRect(option.rect, selectedBg);
		penDivision.setColor(selectedBg);
		nameTextColor = selectedText;
		statusTextColor = selectedText;
	} else {
		penDivision.setColor(QColor(244, 244, 244));
	}

	QRect rect = option.rect;
	rect.setWidth(pixmap.width());
	rect.setHeight(pixmap.height());
	rect.moveTop(rect.y() + (option.rect.height() - pixmap.height())/2);
	rect.moveLeft(rect.left() + 2);
	painter->drawPixmap(rect, pixmap);

	rect = option.rect;
	rect.setLeft(rect.x() + pixmap.width() + 8);
	rect.moveTop(rect.y() + 3);
	QFont font;
	painter->setFont(font);
	painter->setPen(nameTextColor);
	painter->drawText(rect, index.data(Qt::DisplayRole).toString());

	painter->setPen(statusTextColor);
	rect.setTop(rect.y() + rect.height()/2);
	rect.moveTop(rect.y() - 3);
	//QString statusText = index.data(RosterItem::StatusText).toString();
	//QFontMetrics fontMetrics(font);
	//statusText = fontMetrics.elidedText(statusText, Qt::ElideRight, rect.width() - 34);
	//painter->drawText(rect, statusText);

	penDivision.setWidth(0);
	painter->setPen(penDivision);

	rect = option.rect;
	QPoint left = rect.bottomLeft();
	left.setX(left.x() + 4);
	QPoint right = rect.bottomRight();
	right.setX(right.x() - 4);
	painter->drawLine(left, right);

//	QImage image;
//	value = index.data(RosterItem::Avatar);
//	if(value.type() == QVariant::Image)
//	{
//		image = qvariant_cast<QImage>(value);
//	}

	pixmap = QPixmap(":/icons/resource/avatar.png");
	rect = option.rect;
	rect.setWidth(pixmap.width());
	rect.setHeight(pixmap.height());
	rect.moveTop(rect.y() + (option.rect.height() - pixmap.height())/2);
	rect.moveLeft(option.rect.x() + option.rect.width() - pixmap.width() - 2);

//    if(image.isNull())
//        painter->drawPixmap(rect, pixmap);
//    else
		//painter->drawImage(rect, image);

	painter->restore();
}
