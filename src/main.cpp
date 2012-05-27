#include <QtGui>

#include "messenger.h"
#include <version.h>

#include <qxmpp/QXmppLogger.h>

int main(int argc, char *argv[])
{
    //QXmppLogger *logger = QXmppLogger::getLogger();
    //logger->setLoggingType(QXmppLogger::StdoutLogging);

    QTranslator translator;
    translator.load("acid-pc_" + QLocale::system().name());

    QApplication app(argc, argv);
        app.setApplicationName(APP_NAME);
        app.setApplicationVersion(APP_VERSION);
        app.setOrganizationName(APP_COMPANY);
        app.setOrganizationDomain(APP_SITE);
        app.setQuitOnLastWindowClosed(false);
        app.installTranslator(& translator);

    Messenger m(& translator);
    m.launch();

    return app.exec();
}
