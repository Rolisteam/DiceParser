cmake_minimum_required(VERSION 3.5)


project(irc)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
set(EXECUTABLE_OUTPUT_PATH bin/)

# Find the QtWidgets library
set(QT_REQUIRED_VERSION "5.12.0")

set(irc_SOURCES botircdiceparser.cpp main.cpp)
set(irc_HEADERS botircdiceparser.h)



find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Core Widgets Gui Svg LinguistTools Network)


set(MODE "cli")

ADD_DEFINITIONS(
    -std=c++11
)

add_executable(dirc ${irc_SOURCES} ${irc_HEADERS} ${irc_FORMS})
set(diceparser_shared_INCLUDE_DIRS "../diceparser")
target_include_directories(dirc PRIVATE ../include ../ ../result ../node)
link_directories(BEFORE ${CMAKE_BINARY_DIR})



target_link_libraries(dirc PUBLIC Qt5::Core Qt5::Gui Qt5::Svg Qt5::Widgets Qt5::Network PRIVATE diceparser_shared)
INSTALL_TARGETS(/bin dirc)
