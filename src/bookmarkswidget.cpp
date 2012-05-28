#include "bookmarkswidget.h"
#include "ui_bookmarkswidget.h"

BookmarksWidget::BookmarksWidget(QXmppBookmarkManager *manager): QWidget(), ui(new Ui::BookmarksWidget) {
    ui->setupUi(this);
    ui->groupBox->setVisible(false); // Скорее всего, эта штука не понадобится, но если понадобится, вернём
}

BookmarksWidget::~BookmarksWidget() {
    delete ui;
}

void BookmarksWidget::on_add_button_clicked() {
    ui->items->insertRow(ui->items->rowCount());
}

void BookmarksWidget::on_delete_button_clicked() {
    ui->items->removeRow(ui->items->currentRow());
}
