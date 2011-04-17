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

#ifndef NOTIFICATIONWIDGET_H
#define NOTIFICATIONWIDGET_H

#include <QtGui>

class NotificationWidget: public QWidget
{
    Q_OBJECT

public:
	NotificationWidget(QWidget *parent = 0, unsigned short int timeout = 5);
    ~NotificationWidget();
	void showMessage(const QPixmap &icon, QWidget *widget, const bool &waitForUser = false);

signals:
    void hidden();

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    enum DesktopPanel {
        PanelTop,
        PanelBottom,
        PanelLeft,
        PanelRight
    };

    QRect desktopWidgetAvailableGeometry;
    QRect desktopWidgetFullGeometry;
    DesktopPanel panelPosition;
    QPropertyAnimation *geometryAnimation;
    QPropertyAnimation *opacityAnimation;
    QTimer *waitingTimer;

    QLabel *iconLabel;
    QLabel *messageLabel;

    QRect initialRect();
    QRect destinationRect();

    bool waitForUserEvent;
    bool showing;

private slots:
    void showMe();
    void hideMe();
    void onHideMeFinished();
};

#endif // NOTIFICATIONWIDGET_H
