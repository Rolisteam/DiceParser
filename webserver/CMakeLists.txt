cmake_minimum_required(VERSION 2.8)

option(UPDATE_TRANSLATIONS "update Translation" OFF)
MESSAGE(STATUS "UPDATE TRANSLATIONS: ${UPDATE_TRANSLATIONS}")


project(diceserver)


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Core Network)
#find_package(Qt5QuickCompiler)

set(EXECUTABLE_OUTPUT_PATH bin/)

include_directories(${Qt5Core_INCLUDES} ${Qt5Network_INCLUDES} webserver/qhttp/src ../)
add_definitions(${Qt5Core_DEFINITIONS} ${Qt5Network_DEFINITIONS} )

ADD_DEFINITIONS(
    -std=c++11
)

#set(diceserver_RESOURCES diceparser.qrc)
FIND_PACKAGE(Qt5LinguistTools)



SET( diceserver_sources
   ../diceparser.cpp
   ../range.cpp
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
   ../node/explodedicenode.cpp
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
   ../node/filternode.cpp
   ../node/stringnode.cpp
    main.cpp
    diceserver.cpp
    ../highlightdice.cpp
)
#qt5_add_resources(RESOURCE_ADDED mobile.qrc)

add_executable( diceserver ${diceserver_sources} )

target_link_libraries(diceserver ${Qt5Core_LIBRARIES} ${Qt5Network_LIBRARIES} /home/renaud/application/mine/DiceParser/webserver/qhttp/xbin/libqhttp.so)
INSTALL_TARGETS(/bin diceserver)

#qt5_use_modules()
