#include <QtGui>

#include "messenger.h"
#include <version.h>

#include <qxmpp/QXmppLogger.h>

int main(int argc, char *argv[])
{
    //QXmppLogger *logger = QXmppLogger::getLogger();
    //logger->setLoggingType(QXmppLogger::StdoutLogging);

    QSettings settings(APP_COMPANY, APP_NAME);
    QTranslator translator;

    if(settings.contains("settings/language")) {
        switch(settings.value("settings/language", 0).toInt()) {
            case 0: break;
            case 1: translator.load("acid-pc_id"); break;
            case 2: translator.load("acid-pc_jawa"); break;
            case 3: translator.load("acid-pc_sunda"); break;
            case 4: translator.load("acid-pc_ru"); break;
        }
    } else {
        translator.load("acid-pc_" + QLocale::system().name());
    }

    QApplication app(argc, argv);
        app.setApplicationName(APP_NAME);
        app.setApplicationVersion(APP_VERSION);
        app.setOrganizationName(APP_COMPANY);
        app.setOrganizationDomain(APP_SITE);
        app.setQuitOnLastWindowClosed(false);
        app.installTranslator(& translator);

    Messenger m(& translator, & settings);
    m.launch();

    return app.exec();
}
