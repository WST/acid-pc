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
    
private slots:
    void on_add_button_clicked();

    void on_delete_button_clicked();

private:
    Ui::BookmarksWidget *ui;
};

#endif // BOOKMARKSWIDGET_H
