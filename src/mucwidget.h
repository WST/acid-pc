#ifndef MUCWIDGET_H
#define MUCWIDGET_H

#include "tabwidget.h"

namespace Ui {
    class MUCWidget;
}

class MUCWidget : public TabWidget
{
    Q_OBJECT

public:
    explicit MUCWidget(QString with, QWidget *parent = 0);
    ~MUCWidget();

private:
    Ui::MUCWidget *ui;
};

#endif // MUCWIDGET_H
