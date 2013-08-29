#ifndef VERSION_H
#define VERSION_H

#define APP_NAME "ACId"
#define APP_NAME_FULL "Aplikasi Chat Indonesia"
#define APP_COMPANY "SmartCommunity"
#define APP_SITE "http://jsmart.web.id"
#define APP_VERSION "0.1.0.0"
#define APP_VERSION_WIN32 0,1,0,0

#define PROXY65_JID "proxy.jsmart.web.id"
#define STUN_ADDRESS "xmpp.jsmart.web.id"
#define SUPPORT_JID "support@conference.jsmart.web.id"
#define APP_CAPSNODE "http://acid-im.net/caps"
#define REGISTER_URL "https://register.jsmart.web.id:444/"
#define INCOMING_FILES_STORAGE QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
#define HYPERLINK_REPLACE_ARGS QRegExp("(http://)([a-z0-9@\\-_\\./\\?&;:=]+)(,|\\(|\\)| |\\?|!)?", Qt::CaseInsensitive), "<a href=\"\\1\\2\">\\1\\2</a>\\3"
#define KEEPALIVE_INTERVAL 60
#define KEEPALIVE_TIMEOUT 30

#define STATUS_ICON_OFFLINE QIcon(":/trayicon/offline-16px.png")
#define STATUS_ICON_ONLINE QIcon(":/trayicon/online-16px.png")
#define STATUS_ICON_AWAY QIcon(":/trayicon/away-16px.png")
#define STATUS_ICON_XA QIcon(":/trayicon/xa-16px.png")
#define STATUS_ICON_DND QIcon(":/trayicon/dnd-16px.png")
#define STATUS_ICON_F4C QIcon(":/trayicon/f4c-16px.png")

#endif // VERSION_H
