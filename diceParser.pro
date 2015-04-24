#-------------------------------------------------
#
# Project created by QtCreator 2013-12-24T13:06:36
#
#-------------------------------------------------

QT       += core

QT       -= gui

include(node/node.pri)

TARGET = diceParser
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#CONFIG+= IRC
#CONFIG+= GUI
CONFIG+= CLI
CLI {
DEFINES += CLI
include(cli/cli.pri)cd
}
IRC {
include(irc/irc.pri)
QT       += gui widgets
DEFINES+= HAVE_IRC
message("test")
}
GUI {
include(gui/gui.pri)
QT       += gui widgets
DEFINES+= HAVE_GUI
}


SOURCES += diceparser.cpp \
    result/diceresult.cpp \
    range.cpp \
    booleancondition.cpp \
    validator.cpp \
    die.cpp \
    result/result.cpp \
    result/scalarresult.cpp \
    parsingtoolbox.cpp \
    result/stringresult.cpp \
    dicealias.cpp


HEADERS += \
    diceparser.h \
    result/diceresult.h \
    range.h \
    booleancondition.h \
    validator.h \
    die.h \
    result/result.h \
    result/scalarresult.h \
    result/parsingtoolbox.h \
    result/stringresult.h \
    dicealias.h

OTHER_FILES += README.md \
    HelpMe.md


#QMAKE_CXXFLAGS += -O0
