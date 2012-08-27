#include "tabwidget.h"

TabWidget::TabWidget(QString with, QWidget *parent): QWidget(parent) {
    jid = with.toLower();
    splitJid(jid, &bare_jid, &resource);
}

QString &TabWidget::getJid() {
    return jid;
}

QString &TabWidget::getBareJid() {
    return bare_jid;
}

TabWidget::Type TabWidget::getType() {
    return type;
}

void TabWidget::setType(TabWidget::Type newtype) {
    type = newtype;
}

