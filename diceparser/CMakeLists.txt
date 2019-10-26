cmake_minimum_required(VERSION 3.5)

project(diceparser VERSION 1.9.0 DESCRIPTION "Parser of dice command")

set(QT_REQUIRED_VERSION "5.12.0")
find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Core Test Gui Svg)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} result node)

SET( dice_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/diceparser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/range.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/booleancondition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/validator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/validatorlist.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/operationcondition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/die.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parsingtoolbox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dicealias.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/result/result.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/result/scalarresult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/result/stringresult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/result/diceresult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/countexecutenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/dicerollernode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/executionnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/explodedicenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/helpnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/allsamenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/mergenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/jumpbackwardnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/keepdiceexecnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/listaliasnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/listsetrollnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/numbernode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/parenthesesnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/paintnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/rerolldicenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/scalaroperatornode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/sortresult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/startingnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/filternode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/stringnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/ifnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/splitnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/groupnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/bind.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/occurencecountnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/uniquenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/highlightdice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/variablenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/valueslistnode.cpp
)

IF(STATIC_BUILD)
    add_library(diceparser_static STATIC ${dice_sources} )
    target_include_directories(diceparser_static PRIVATE include)
    SET_TARGET_PROPERTIES(diceparser_static PROPERTIES OUTPUT_NAME diceparser CLEAN_DIRECT_OUTPUT 1)
    target_link_libraries(diceparser_static PUBLIC Qt5::Core Qt5::Gui Qt5::Svg)
    install(TARGETS diceparser_static
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()



add_library(diceparser_shared SHARED ${dice_sources})

target_include_directories(diceparser_shared PRIVATE include)


SET_TARGET_PROPERTIES(diceparser_shared PROPERTIES OUTPUT_NAME diceparser CLEAN_DIRECT_OUTPUT 1)
target_link_libraries(diceparser_shared PUBLIC Qt5::Core Qt5::Gui Qt5::Svg)

set_target_properties(diceparser_shared PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(diceparser_shared PROPERTIES SOVERSION 1)

set_target_properties(diceparser_shared PROPERTIES PUBLIC_HEADER "include/diceparser.h;include/highlightdice.h;include/parsingtoolbox.h;include/dicealias.h;include/diceparserhelper.h")

include(GNUInstallDirs)

install(TARGETS diceparser_shared
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


configure_file(diceparser.pc.in diceparser.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/diceparser.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)



