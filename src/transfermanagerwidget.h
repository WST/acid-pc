#ifndef TRANSFERMANAGERWIDGET_H
#define TRANSFERMANAGERWIDGET_H

// ACId
#include "tabwidget.h"

// qxmpp
#include "qxmpp/QXmppTransferManager.h"

namespace Ui {
class TransferManagerWidget;
}

class TransferManagerWidget : public TabWidget
{
    Q_OBJECT
    
public:
    explicit TransferManagerWidget(QWidget *parent = 0);
    ~TransferManagerWidget();
    void newTransferJob(QXmppTransferJob *job);
    void setOnline(bool is_online);
    
private:
    Ui::TransferManagerWidget *ui;
    int rowBySid(const QString &sid);

private slots:
    void changeState(QXmppTransferJob::State state);
    void progress(qint64 sent, qint64 total);
};

#endif // TRANSFERMANAGERWIDGET_H
