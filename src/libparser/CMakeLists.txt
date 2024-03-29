cmake_minimum_required(VERSION 3.16)

set(QT_REQUIRED_VERSION "6.2.0")
set(QT_VERSION_MAJOR "6")
find_package(Qt${QT_VERSION_MAJOR} ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Core Test Gui Svg)
find_package(Qt${QT_VERSION_MAJOR} ${QT_REQUIRED_VERSION} CONFIG COMPONENTS Qml Concurrent)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} result node)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(dice_header_path include/diceparser)

set(dice_public_headers ${dice_header_path}/diceparser_global.h
${dice_header_path}/dicealias.h
${dice_header_path}/diceparser.h
${dice_header_path}/diceparserhelper.h
${dice_header_path}/highlightdice.h
${dice_header_path}/parsingtoolbox.h # should not be public…
)


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
    ${CMAKE_CURRENT_SOURCE_DIR}/node/repeaternode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/switchcasenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node/replacevaluenode.cpp
)

include(install_helper OPTIONAL RESULT_VARIABLE installFound)

if(installFound STREQUAL "NOTFOUND")
  set(HAS_INSTALL OFF)
else()
  set(HAS_INSTALL ON)
endif()

IF(STATIC_BUILD)
    set_target_properties(diceparser_static PROPERTIES OUTPUT_NAME diceparser CLEAN_DIRECT_OUTPUT 1)
    add_library(diceparser_static STATIC ${dice_sources} )
    target_include_directories(diceparser_static PRIVATE include)
    target_link_libraries(diceparser_static PUBLIC Qt6::Core Qt6::Gui Qt6::Svg)
    if(${HAS_INSTALL})
      rinstallLib(diceparser_static libraries)
    endif()
endif()

add_library(diceparser_shared SHARED ${dice_sources} ${dice_public_headers})
set_target_properties(diceparser_shared PROPERTIES PUBLIC_HEADER "${public_header_widget}")
target_compile_definitions(diceparser_shared PRIVATE DICEPARSER_LIBRARY)

target_include_directories(diceparser_shared
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/node
    ${CMAKE_CURRENT_SOURCE_DIR}/result
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


target_link_libraries(diceparser_shared PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Svg)
set_target_properties(diceparser_shared PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(diceparser_shared PROPERTIES SOVERSION 1)


#include(GNUInstallDirs)


configure_file(diceparser.pc.in diceparser.pc @ONLY)

if(${HAS_INSTALL})
  rinstallLib(diceparser_shared libraries)
else()
  install(TARGETS diceparser_shared LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/diceparser.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
endif()

if(Qt${QT_VERSION_MAJOR}Qml_FOUND)
    message(STATUT "qml found")
    set(dice_header_qobject include/diceparser_qobject/diceparser_qobject_global.h include/diceparser_qobject/qmltypesregister.h include/diceparser_qobject/diceroller.h)
    set(dice_source_qobject qmltypesregister.cpp diceroller.cpp)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    add_library(diceparser_qobject SHARED ${dice_header_qobject} ${dice_source_qobject})
    set_target_properties(diceparser_qobject PROPERTIES PUBLIC_HEADER "${dice_header_qobject}")
    target_compile_definitions(diceparser_qobject PRIVATE DICEPARSER_QOBJECT_LIBRARY)
    target_link_libraries(diceparser_qobject PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::Concurrent PRIVATE diceparser_shared)
    set_target_properties(diceparser_qobject PROPERTIES VERSION ${PROJECT_VERSION})
    set_target_properties(diceparser_qobject PROPERTIES SOVERSION 1)

    target_include_directories(diceparser_qobject
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
      PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


      if(${HAS_INSTALL})
        message(STATUT "Normal install " ${HAS_INSTALL})
        rinstallLib(diceparser_qobject libraries)
      else()
        message(STATUT "Degraded install" ${HAS_INSTALL})
        install(TARGETS diceparser_qobject LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
      endif()
endif()



