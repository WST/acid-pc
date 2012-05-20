#ifndef NEWCONTACTWINDOW_H
#define NEWCONTACTWINDOW_H

#include <QDialog>

namespace Ui {
class NewContactWindow;
}

class NewContactWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewContactWindow(QWidget *parent = 0);
    ~NewContactWindow();

signals:
    void newContactAdditionRequested(const QString &, const QString &); // JID, nick
    
private slots:
    void on_buttonBox_accepted();

    void on_contact_jid_textChanged(const QString &arg1);

    void on_contact_name_textChanged(const QString &arg1);

private:
    Ui::NewContactWindow *ui;
};

#endif // NEWCONTACTWINDOW_H
