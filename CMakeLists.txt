cmake_minimum_required(VERSION 3.5)

project(diceparser)

set(QT_REQUIRED_VERSION "5.12.0")
find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Core Test Gui Svg)

enable_testing(true)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} result node)

SET( dice_sources
    diceparser.cpp
    range.cpp
    booleancondition.cpp
    validator.cpp
    compositevalidator.cpp
    operationcondition.cpp
    die.cpp
    parsingtoolbox.cpp
    dicealias.cpp
    result/result.cpp
    result/scalarresult.cpp
    result/stringresult.cpp
    result/diceresult.cpp
    node/countexecutenode.cpp
    node/dicerollernode.cpp
    node/executionnode.cpp
    node/explodedicenode.cpp
    node/helpnode.cpp
    node/mergenode.cpp
    node/jumpbackwardnode.cpp
    node/keepdiceexecnode.cpp
    node/listaliasnode.cpp
    node/listsetrollnode.cpp
    node/numbernode.cpp
    node/parenthesesnode.cpp
  #  node/forloopnode.cpp
    node/paintnode.cpp
    node/rerolldicenode.cpp
    node/scalaroperatornode.cpp
    node/sortresult.cpp
    node/startingnode.cpp
    node/filternode.cpp
    node/stringnode.cpp
    node/ifnode.cpp
    node/splitnode.cpp
    node/groupnode.cpp
    node/bind.cpp
    node/occurencecountnode.cpp
    node/uniquenode.cpp
    highlightdice.cpp
    node/variablenode.cpp
    node/valueslistnode.cpp
)

add_library(diceparser SHARED ${dice_sources} )
target_link_libraries(diceparser PUBLIC Qt5::Core Qt5::Gui Qt5::Svg)

add_subdirectory(cli)
add_subdirectory( tests )
#add_subdirectory(irc)
#add_subdirectory(mobile)
#add_subdirectory(webserver)
