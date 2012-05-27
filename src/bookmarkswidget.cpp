#include "bookmarkswidget.h"
#include "ui_bookmarkswidget.h"

BookmarksWidget::BookmarksWidget(QXmppBookmarkManager *manager): QWidget(), ui(new Ui::BookmarksWidget) {
    ui->setupUi(this);
}

BookmarksWidget::~BookmarksWidget() {
    delete ui;
}
