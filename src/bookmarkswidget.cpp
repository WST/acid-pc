#include "bookmarkswidget.h"
#include "ui_bookmarkswidget.h"

BookmarksWidget::BookmarksWidget(QXmppBookmarkManager *manager, QWidget *parent): TabWidget("", parent), ui(new Ui::BookmarksWidget) {
    ui->setupUi(this);
    ui->groupBox->setVisible(false); // Скорее всего, эта штука не понадобится, но если понадобится, вернём
    TabWidget::setType(TabWidget::Bookmarks);
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

void BookmarksWidget::on_save_button_clicked() {
    // Построить новый список закладок, проверив корректность данных
    // и сохранить закладки на сервер
}

void BookmarksWidget::setOnline(bool is_online) {
    ui->save_button->setEnabled(is_online);
}
