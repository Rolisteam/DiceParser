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


SOURCES += main.cpp \
    diceparser.cpp \
    diceresult.cpp \
    range.cpp \
    booleancondition.cpp \
    validator.cpp \
    die.cpp


HEADERS += \
    diceparser.h \
    diceresult.h \
    range.h \
    booleancondition.h \
    validator.h \
    die.h

OTHER_FILES += README.md


#QMAKE_CXXFLAGS += -O0
