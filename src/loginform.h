#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>

namespace Ui {
    class LoginForm;
}

class LoginForm: public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QString title, QWidget *parent = 0);
    ~LoginForm();
    const QString username();
    const QString password();
    const QString domain();
    bool autoLogin();
    bool savePassword();

private:
    Ui::LoginForm *ui;
    bool username_valid;
    bool password_valid;

signals:
    void finished();

private slots:
    void validateUsername(QString username);
    void validatePassword(QString password);
    void refreshButton();
};

#endif // LOGINFORM_H
