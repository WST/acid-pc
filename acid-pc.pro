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

SOURCES    += src/main.cpp \
              src/mainwindow.cpp \
              src/messanger.cpp \
              src/loginform.cpp
HEADERS    += version.h \
              src/mainwindow.h \
              src/messanger.h \
              src/loginform.h
FORMS      += ui/mainwindow.ui \
              ui/loginform.ui
RESOURCES  += res/app.qrc
	
win32:RC_FILE = \
              res/win32/app.rc


