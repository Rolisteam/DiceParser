cmake_minimum_required(VERSION 3.5)

project(diceparser VERSION 1.9.0 DESCRIPTION "Parser of dice command")

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

add_library(diceparser_shared SHARED ${dice_sources} )
add_library(diceparser_static STATIC ${dice_sources} )

target_include_directories(diceparser_shared PRIVATE include)
target_include_directories(diceparser_static PRIVATE include)

SET_TARGET_PROPERTIES(diceparser_static PROPERTIES OUTPUT_NAME diceparser CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(diceparser_shared PROPERTIES OUTPUT_NAME diceparser CLEAN_DIRECT_OUTPUT 1)

target_link_libraries(diceparser_shared PUBLIC Qt5::Core Qt5::Gui Qt5::Svg)
target_link_libraries(diceparser_static PUBLIC Qt5::Core Qt5::Gui Qt5::Svg)

set_target_properties(diceparser_shared PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(diceparser_shared PROPERTIES SOVERSION 1)
#target_link_libraries(diceparsersta PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties(diceparser_shared PROPERTIES PUBLIC_HEADER "include/diceparser.h;include/highlightdice.h;include/parsingtoolbox.h;include/dicealias.h;include/diceparserhelper.h")
#set_target_properties(diceparsersta PROPERTIES PUBLIC_HEADER "include/diceparser.h;include/highlightdice.h;include/parsingtoolbox.h;include/dicealias.h;include/diceparserhelper.h")

add_subdirectory(cli)
add_subdirectory( tests )
#add_subdirectory(irc)
#add_subdirectory(mobile)
#add_subdirectory(webserver)


include(GNUInstallDirs)

install(TARGETS diceparser_shared
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS diceparser_static
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

configure_file(diceparser.pc.in diceparser.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/diceparser.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

