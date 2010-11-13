#include <QtGui>

#include "mainwindow.h"
#include "../version.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
        app.setApplicationName(APP_NAME);
        app.setApplicationVersion(APP_VERSION);
        app.setOrganizationName(APP_COMPANY);
        app.setOrganizationDomain(APP_SITE);

    MainWindow w;
    w.show();

    return app.exec();
}
