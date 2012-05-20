#include <QtGui>

#include "messenger.h"
#include <version.h>

#include <qxmpp/QXmppLogger.h>

int main(int argc, char *argv[])
{
    QXmppLogger *logger = QXmppLogger::getLogger();
    logger->setLoggingType(QXmppLogger::StdoutLogging);

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
