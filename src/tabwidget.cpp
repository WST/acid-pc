#include "tabwidget.h"

TabWidget::TabWidget(QString with, QWidget *parent): QWidget(parent) {
    jid = with;
}

QString &TabWidget::getJid() {
    return jid;
}

TabWidget::Type TabWidget::getType() {
    return type;
}

void TabWidget::setType(TabWidget::Type newtype) {
    type = newtype;
}

