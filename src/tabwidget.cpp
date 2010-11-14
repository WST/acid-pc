#include "tabwidget.h"

TabWidget::TabWidget(QString with, QWidget *parent): QWidget(parent) {
    jid = with;
}

QString &TabWidget::getJid() {
    return jid;
}
