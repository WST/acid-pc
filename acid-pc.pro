TARGET      = acid-pc
TEMPLATE    = app
QT         += core \
              gui
CONFIG     += qt \
              warn_on \
              release

DESTDIR     = bin
MOC_DIR     = build/moc
OBJECTS_DIR = build/obj
RCC_DIR     = build/rcc
UI_DIR      = build/ui

INCLUDEPATH += \
              $$PWD

SOURCES    += src/main.cpp \
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
	 src/roster_list_view.cpp \
	 src/roster_item_model.cpp \
	 src/roster_item.cpp
HEADERS    += version.h \
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
	 src/roster_list_view.h \
	 src/roster_item_model.h \
	 src/roster_item.h
FORMS      += \
              ui/loginform.ui \
    ui/messageform.ui \
    ui/chatwidget.ui \
    ui/chatwindow.ui \
    ui/mucwidget.ui \
    ui/aboutwindow.ui
RESOURCES  += res/app.qrc

include(src/3rd-party/qxmpp.pri)

win32:RC_FILE = \
              res/win32/app.rc
