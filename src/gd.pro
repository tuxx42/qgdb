QT += gui core widgets

TEMPLATE = app

SOURCES+=gd.cpp

SOURCES+=mainwindow.cpp
HEADERS+=mainwindow.h

SOURCES+=core.cpp
HEADERS+=core.h

SOURCES+=codeviewer.cpp
HEADERS+=codeviewer.h

SOURCES+=com.cpp
HEADERS+=com.h

SOURCES+=log.cpp
HEADERS+=log.h

SOURCES+=util.cpp
HEADERS+=util.h

SOURCES+=tree.cpp
HEADERS+=tree.h

SOURCES+=aboutdialog.cpp
HEADERS+=aboutdialog.h

SOURCES+=highlighter.cpp
HEADERS+=highlighter.h

SOURCES+=ini.cpp
HEADERS+=ini.h

SOURCES+= opendialog.cpp
HEADERS+=opendialog.h

SOURCES+=settings.cpp
HEADERS+=settings.h

SOURCES+=tagscanner.cpp
HEADERS+=tagscanner.h

HEADERS+=config.h

SOURCES+=settingsdialog.cpp
HEADERS+=settingsdialog.h
FORMS += settingsdialog.ui

FORMS += mainwindow.ui
FORMS += aboutdialog.ui
FORMS += opendialog.ui

RESOURCES += resource.qrc

QMAKE_CXXFLAGS += -I./  -g

TARGET=qgdb
