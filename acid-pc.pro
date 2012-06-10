TARGET      = acid-pc
TEMPLATE    = app
QT         += core gui multimedia network xml
CONFIG     += qt warn_on

DESTDIR     = bin
MOC_DIR     = build/moc
OBJECTS_DIR = build/obj
RCC_DIR     = build/rcc
UI_DIR      = build/ui

INCLUDEPATH = $$PWD
win32:INCLUDEPATH += C:/include

SOURCES += \
	src/main.cpp \
	src/messenger.cpp \
	src/loginform.cpp \
	src/trayicon.cpp \
	src/messageform.cpp \
	src/chatwidget.cpp \
	src/chatwindow.cpp \
	src/tabwidget.cpp \
	src/mucwidget.cpp \
	src/functions.cpp \
	src/aboutwindow.cpp \
	src/settingswindow.cpp \
	src/vcardwindow.cpp \
	src/contact_list/tree_view.cpp \
	src/contact_list/item_model.cpp \
	src/contact_list/group_item.cpp \
	src/contact_list/contact_item.cpp \
	src/contact_list/item.cpp \
	src/contact_list/item_delegate.cpp \
	src/contact_list/qxmpp_bridge.cpp \
	src/contact_list/muc_item.cpp \
	src/voicecallwindow.cpp \
	src/confirmationwindow.cpp \
	src/joinroomwindow.cpp \
	src/newcontactwindow.cpp \
	src/servicediscoverywidget.cpp \
	src/bookmarkswidget.cpp \
	src/transfermanagerwidget.cpp

HEADERS += \
	version.h \
	src/messenger.h \
	src/loginform.h \
	src/trayicon.h \
	src/messageform.h \
	src/chatwidget.h \
	src/chatwindow.h \
	src/tabwidget.h \
	src/mucwidget.h \
	src/functions.h \
	src/aboutwindow.h \
	src/settingswindow.h \
	src/vcardwindow.h \
	src/contact_list/tree_view.h \
	src/contact_list/item_model.h \
	src/contact_list/group_item.h \
	src/contact_list/contact_item.h \
	src/contact_list/item.h \
	src/contact_list/item_delegate.h \
	src/contact_list/qxmpp_bridge.h \
	src/contact_list/muc_item.h \
	src/voicecallwindow.h \
	src/confirmationwindow.h \
	src/joinroomwindow.h \
	src/newcontactwindow.h \
	src/servicediscoverywidget.h \
	src/bookmarkswidget.h \
	src/transfermanagerwidget.h

FORMS += \
	ui/loginform.ui \
	ui/messageform.ui \
	ui/chatwidget.ui \
	ui/chatwindow.ui \
	ui/mucwidget.ui \
	ui/aboutwindow.ui \
	ui/settingswindow.ui \
	ui/vcardwindow.ui \
	ui/voicecallwindow.ui \
	ui/confirmationwindow.ui \
	ui/joinroomwindow.ui \
	ui/newcontactwindow.ui \
	ui/servicediscoverywidget.ui \
	ui/bookmarkswidget.ui \
	ui/transfermanagerwidget.ui

RESOURCES += res/app.qrc

TRANSLATIONS += i18n/acid-pc_ru.ts i18n/acid-pc_id.ts i18n/acid-pc_jawa.ts i18n/acid-pc_sunda.ts

win32:LIBS += -L"C:/lib" -lqxmpp -lws2_32 -ldnsapi
unix:LIBS += -lqxmpp

win32:RC_FILE = \
	res/win32/app.rc
