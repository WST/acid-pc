#ifndef TRANSFERMANAGERWIDGET_H
#define TRANSFERMANAGERWIDGET_H

#include "tabwidget.h"

namespace Ui {
class TransferManagerWidget;
}

class TransferManagerWidget : public TabWidget
{
    Q_OBJECT
    
public:
    explicit TransferManagerWidget(QWidget *parent = 0);
    ~TransferManagerWidget();
    
private:
    Ui::TransferManagerWidget *ui;
};

#endif // TRANSFERMANAGERWIDGET_H
