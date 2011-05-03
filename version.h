#ifndef VERSION_H
#define VERSION_H

#define APP_NAME "ACId"
#define APP_NAME_FULL "Aplikasi Chat Indonesia"
#define APP_COMPANY "SmartCommunity"
#define APP_SITE "http://jsmart.web.id"
#define APP_VERSION "0.1.0.0"
#define APP_VERSION_WIN32 0,1,0,0

#define PROXY65_JID "proxy.jsmart.web.id"
#define SUPPORT_JID "support@conference.jsmart.web.id"
#define APP_CAPSNODE "http://acid-im.net/caps"
#define INCOMING_FILES_STORAGE QDesktopServices::storageLocation(QDesktopServices::DesktopLocation)
#define HYPERLINK_REPLACE_ARGS QRegExp("(http://)([a-z0-9\\-_\\./\\?&;:=]+)(,|\\.|\\(|\\)| |\\?|!)?", Qt::CaseInsensitive), "<a href=\"\\1\\2\">\\1\\2</a>\\3"
#define KEEPALIVE_INTERVAL 60
#define KEEPALIVE_TIMEOUT 30

#endif // VERSION_H
