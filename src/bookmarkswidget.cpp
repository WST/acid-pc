#include "bookmarkswidget.h"
#include "ui_bookmarkswidget.h"

BookmarksWidget::BookmarksWidget(QXmppBookmarkManager *manager): QWidget(), ui(new Ui::BookmarksWidget) {
    ui->setupUi(this);
    ui->groupBox->setVisible(false); // Скорее всего, эта штука не понадобится, но если понадобится, вернём
}

BookmarksWidget::~BookmarksWidget() {
    delete ui;
}
