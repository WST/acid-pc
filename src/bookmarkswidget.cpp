#include "bookmarkswidget.h"
#include "ui_bookmarkswidget.h"

#include "qxmpp/QXmppBookmarkManager.h"
#include "qxmpp/QXmppBookmarkSet.h"

BookmarksWidget::BookmarksWidget(QXmppBookmarkManager *bookmark_manager, QWidget *parent): TabWidget("", parent), ui(new Ui::BookmarksWidget) {
    ui->setupUi(this);
    ui->groupBox->setVisible(false); // Скорее всего, эта штука не понадобится, но если понадобится, вернём
    TabWidget::setType(TabWidget::Bookmarks);
    manager = bookmark_manager;

    QList<QXmppBookmarkConference> bookmarks = manager->bookmarks().conferences();
    QListIterator<QXmppBookmarkConference> iterator(bookmarks);

    cleanup();

    int rows = ui->items->rowCount();
    while(iterator.hasNext()) {
        ui->items->insertRow(rows);
        QXmppBookmarkConference bookmark = iterator.next();
        ui->items->setItem(rows, 0, new QTableWidgetItem(bookmark.name())); // Интересно, оно будет удаляться?
        ui->items->setItem(rows, 1, new QTableWidgetItem(bookmark.nickName()));
        ui->items->setItem(rows, 2, new QTableWidgetItem(bookmark.jid()));
    }
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
    setEnabled(false);
    QXmppBookmarkSet bookmarks;
    QList<QXmppBookmarkConference> conferences;
    unsigned int i,rows;
    rows = ui->items->rowCount();
    for(i = 0; i < rows; i ++) {
        QTableWidgetItem *name_item = ui->items->item(i, 0);
        QTableWidgetItem *nickname_item = ui->items->item(i, 1);
        QTableWidgetItem *jid_item = ui->items->item(i, 2);

        bool success = false;

        if(name_item && nickname_item && jid_item) {
            if(!name_item->text().isEmpty() && !nickname_item->text().isEmpty() && !jid_item->text().isEmpty()) {
                QXmppBookmarkConference conference;
                conference.setName(name_item->text());
                conference.setNickName(nickname_item->text());
                conference.setJid(jid_item->text());

                conferences.push_back(conference);
                success = true;
            }
        }

        if(!success) {
            setEnabled(true);
            QMessageBox::warning(this, tr("Bookmark editor"), tr("Please fill in all the fields"));
            return;
        }
    }

    bookmarks.setConferences(conferences);
    manager->setBookmarks(bookmarks);

    setEnabled(true);
    QMessageBox::information(this, tr("Bookmark editor"), tr("Bookmarks have been saved"));
}

void BookmarksWidget::setOnline(bool is_online) {
    ui->save_button->setEnabled(is_online);
    ui->add_button->setEnabled(is_online);
    ui->delete_button->setEnabled(is_online);
    ui->items->setEnabled(is_online);
    cleanup();
}

void BookmarksWidget::cleanup() {
    unsigned int i,rows;
    rows = ui->items->rowCount();
    for(i = 0; i < rows; i ++) {
        ui->items->removeRow(0);
    }
}
