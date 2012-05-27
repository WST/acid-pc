#ifndef BOOKMARKSWIDGET_H
#define BOOKMARKSWIDGET_H

#include <QWidget>

#include "qxmpp/QXmppBookmarkManager.h"

namespace Ui {
class BookmarksWidget;
}

class BookmarksWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit BookmarksWidget(QXmppBookmarkManager *manager);
    ~BookmarksWidget();
    
private:
    Ui::BookmarksWidget *ui;
};

#endif // BOOKMARKSWIDGET_H
