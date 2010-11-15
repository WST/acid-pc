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
              src/mainwindow.cpp \
              src/messenger.cpp \
              src/loginform.cpp \
    src/trayicon.cpp \
    src/messageform.cpp \
    src/chatwidget.cpp \
    src/chatwindow.cpp \
    src/tabwidget.cpp \
    src/mucwidget.cpp
HEADERS    += version.h \
              src/mainwindow.h \
              src/messenger.h \
              src/loginform.h \
    src/trayicon.h \
    src/messageform.h \
    src/chatwidget.h \
    src/chatwindow.h \
    src/tabwidget.h \
    src/mucwidget.h
FORMS      += \
              ui/loginform.ui \
    ui/messageform.ui \
    ui/chatwidget.ui \
    ui/chatwindow.ui \
    ui/mucwidget.ui
RESOURCES  += res/app.qrc

include(src/3rd-party/qxmpp.pri)

win32:RC_FILE = \
              res/win32/app.rc
