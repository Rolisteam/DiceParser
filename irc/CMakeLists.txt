cmake_minimum_required(VERSION 2.8)

SET(CMAKE_AUTOUIC ON)

project(irc)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5Network)

SET(mainwindow_SOURCES mainwindow.cpp)
SET(mainwindow_HEADERS mainwindow.h)
SET(mainwindow_FORMS mainwindow.ui)

set(EXECUTABLE_OUTPUT_PATH bin/)


include_directories(${Qt5Core_INCLUDES} ${Qt5Gui_INCLUDES} ${Qt5Widgets_INCLUDES} ${Qt5Network_INCLUDES} ../)
add_definitions(${Qt5Core_DEFINITIONS} ${Qt5Gui_DEFINITIONS} ${Qt5Widgets_DEFINITIONS} ${Qt5Network_DEFINITIONS})


qt5_wrap_ui(UI_HEADERS mainwindow.ui)
#set(MODE "cli")


ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)

add_executable(
    irc
    ../diceparser.cpp
    ../range.cpp
    ../highlightdice.cpp
    ../booleancondition.cpp
    ../validator.cpp
    ../compositevalidator.cpp
    ../operationcondition.cpp
    ../die.cpp
    ../parsingtoolbox.cpp
    ../dicealias.cpp
    ../result/result.cpp
    ../result/scalarresult.cpp
    ../result/stringresult.cpp
    ../result/diceresult.cpp
    ../node/countexecutenode.cpp
    ../node/dicerollernode.cpp
    ../node/executionnode.cpp
    ../node/explosedicenode.cpp
    ../node/helpnode.cpp
    ../node/mergenode.cpp
    ../node/jumpbackwardnode.cpp
    ../node/keepdiceexecnode.cpp
    ../node/listaliasnode.cpp
    ../node/listsetrollnode.cpp
    ../node/numbernode.cpp
    ../node/parenthesesnode.cpp
    ../node/paintnode.cpp
    ../node/rerolldicenode.cpp
    ../node/scalaroperatornode.cpp
    ../node/sortresult.cpp
    ../node/startingnode.cpp
    ../node/ifnode.cpp
    ${UI_HEADERS}
    botircdiceparser.cpp
    main.cpp)


target_link_libraries(irc ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Network_LIBRARIES})
INSTALL_TARGETS(/bin irc)

#qt5_use_modules()

