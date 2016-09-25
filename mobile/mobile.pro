TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp maincontroler.cpp \
    commandmodel.cpp

HEADERS += maincontroler.h \
    commandmodel.h

RESOURCES += qml.qrc

include(../diceparser.pri)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
