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
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();
    const QString username();
    const QString password();
    const QString domain();
    bool autoLogin();
    bool savePassword();

private:
    Ui::LoginForm *ui;

signals:
    void finished();
};

#endif // LOGINFORM_H
