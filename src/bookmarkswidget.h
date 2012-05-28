#ifndef BOOKMARKSWIDGET_H
#define BOOKMARKSWIDGET_H

// ACId
#include "tabwidget.h"

// qxmpp
#include "qxmpp/QXmppBookmarkManager.h"

namespace Ui {
class BookmarksWidget;
}

class BookmarksWidget: public TabWidget
{
    Q_OBJECT
    
public:
    explicit BookmarksWidget(QXmppBookmarkManager *manager, QWidget *parent = 0);
    ~BookmarksWidget();
    
private slots:
    void on_add_button_clicked();
    void on_delete_button_clicked();
    void on_save_button_clicked();
    void setOnline(bool is_online);

private:
    Ui::BookmarksWidget *ui;
};

#endif // BOOKMARKSWIDGET_H
