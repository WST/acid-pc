#include <QtGui>

#include "messenger.h"
#include <version.h>

#include "QXmppLogger.h"

int main(int argc, char *argv[])
{
    QXmppLogger::getLogger()->setLoggingType(QXmppLogger::STDOUT);

    QApplication app(argc, argv);
        app.setApplicationName(APP_NAME);
        app.setApplicationVersion(APP_VERSION);
        app.setOrganizationName(APP_COMPANY);
        app.setOrganizationDomain(APP_SITE);
        app.setQuitOnLastWindowClosed(false);

    Messenger m;
    m.launch();

    return app.exec();
}
