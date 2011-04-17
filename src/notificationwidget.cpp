/**************************************************************************
**
**    This file is a part of Immensus Gmail Notifier
**    Copyright (C) 2009-2010 Erik, Immensus
**
**    Immensus Gmail Notifier is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    Immensus Gmail Notifier is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with Immensus Gmail Notifier. If not, see <http://www.gnu.org/licenses/>.
**
**    Contact e-mail: Yury Erik Potapov <erik@immensus.net>
**
**************************************************************************/

#include "notificationwidget.h"

// PUBLIC

NotificationWidget::NotificationWidget(QWidget *parent): QWidget(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setFixedWidth(500);
    setGeometry(0, 0, 500, 1000);

    iconLabel = new QLabel(this);
        iconLabel->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    messageLabel = new QLabel(this);
        messageLabel->setTextFormat(Qt::RichText);
        messageLabel->setOpenExternalLinks(true);
        messageLabel->setWordWrap(true);

    QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(iconLabel);
        layout->addWidget(messageLabel);
        layout->setStretch(1, 1);

    setWindowFlags(Qt::ToolTip | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Desktop);

    geometryAnimation = new QPropertyAnimation(this, "geometry", this);
        geometryAnimation->setDuration(1000);
        geometryAnimation->setEasingCurve(QEasingCurve::InQuint);

    opacityAnimation = new QPropertyAnimation(this, "windowOpacity", this);
        opacityAnimation->setDuration(1000);
        opacityAnimation->setEasingCurve(QEasingCurve::InQuint);
        opacityAnimation->setStartValue(0.0);
        opacityAnimation->setEndValue(0.9);

    waitingTimer = new QTimer(this);
        waitingTimer->setSingleShot(true);
        waitingTimer->setInterval(5000);
        connect(waitingTimer, SIGNAL(timeout()), this, SLOT(hideMe()));

    showing = false;
}

NotificationWidget::~NotificationWidget()
{
}

void NotificationWidget::showMessage(const QPixmap &icon, const QString &message, const bool &waitForUser)
{
    if (showing)
        return;

    iconLabel->setPixmap(icon);
    messageLabel->setText(message);
    layout()->invalidate();
    layout()->activate();
    adjustSize();

    showing = true;

    desktopWidgetAvailableGeometry = qApp->desktop()->availableGeometry();
    desktopWidgetFullGeometry = qApp->desktop()->geometry();

    if (desktopWidgetAvailableGeometry.top() != desktopWidgetFullGeometry.top()) {
        panelPosition = PanelTop;
    } else if (desktopWidgetAvailableGeometry.bottom() != desktopWidgetFullGeometry.bottom()) {
        panelPosition = PanelBottom;
    } else if (desktopWidgetAvailableGeometry.left() != desktopWidgetFullGeometry.left()) {
        panelPosition = PanelLeft;
    } else {
        panelPosition = PanelRight;
    }

    waitForUserEvent = waitForUser;

    geometryAnimation->setStartValue(initialRect());
    geometryAnimation->setEndValue(destinationRect());

    showMe();
}

// PROTECTED

void NotificationWidget::mousePressEvent(QMouseEvent *event)
{
    if (waitForUserEvent && event->button() == Qt::RightButton)
        hideMe();

    QWidget::mousePressEvent(event);
}

// PRIVATE

QRect NotificationWidget::initialRect()
{
    switch (panelPosition) {
    case PanelTop:
    default:
        return QRect(desktopWidgetAvailableGeometry.width() - width(),
                     desktopWidgetAvailableGeometry.top() - height(),
                     width(),
                     height());
    case PanelBottom:
        return QRect(desktopWidgetAvailableGeometry.width() - width(),
                     desktopWidgetAvailableGeometry.top() + desktopWidgetAvailableGeometry.height(),
                     width(),
                     height());
    case PanelLeft:
        return QRect(desktopWidgetAvailableGeometry.left() - width(),
                     desktopWidgetAvailableGeometry.top() + desktopWidgetAvailableGeometry.height() - height(),
                     width(),
                     height());
    case PanelRight:
        return QRect(desktopWidgetAvailableGeometry.left() + desktopWidgetAvailableGeometry.width(),
                     desktopWidgetAvailableGeometry.top() + desktopWidgetAvailableGeometry.height() - height(),
                     width(),
                     height());
    }
}

QRect NotificationWidget::destinationRect()
{
    switch (panelPosition) {
    case PanelTop:
    default:
        return QRect(desktopWidgetAvailableGeometry.width() - width(),
                     desktopWidgetAvailableGeometry.top(),
                     width(),
                     height());
    case PanelBottom:
        return QRect(desktopWidgetAvailableGeometry.width() - width(),
                     desktopWidgetAvailableGeometry.top() + desktopWidgetAvailableGeometry.height() - height(),
                     width(),
                     height());
    case PanelLeft:
        return QRect(desktopWidgetAvailableGeometry.left(),
                     desktopWidgetAvailableGeometry.top() + desktopWidgetAvailableGeometry.height() - height(),
                     width(),
                     height());
    case PanelRight:
        return QRect(desktopWidgetAvailableGeometry.left() + desktopWidgetAvailableGeometry.width() - width(),
                     desktopWidgetAvailableGeometry.top() + desktopWidgetAvailableGeometry.height() - height(),
                     width(),
                     height());
    }
}

// PRIVATE SLOTS

void NotificationWidget::showMe()
{
    if (!waitForUserEvent)
        connect(geometryAnimation, SIGNAL(finished()), waitingTimer, SLOT(start()));

    geometryAnimation->setDirection(QAbstractAnimation::Forward);
    opacityAnimation->setDirection(QAbstractAnimation::Forward);
    geometryAnimation->start();
    opacityAnimation->start();
    show();
}

void NotificationWidget::hideMe()
{
    if (!waitForUserEvent)
        disconnect(geometryAnimation, SIGNAL(finished()), waitingTimer, SLOT(start()));
    connect(geometryAnimation, SIGNAL(finished()), this, SLOT(onHideMeFinished()));

    geometryAnimation->setDirection(QAbstractAnimation::Backward);
    opacityAnimation->setDirection(QAbstractAnimation::Backward);
    geometryAnimation->start();
    opacityAnimation->start();
}

void NotificationWidget::onHideMeFinished()
{
    hide();
    showing = false;
    disconnect(geometryAnimation, SIGNAL(finished()), this, SLOT(onHideMeFinished()));
    emit hidden();
}
