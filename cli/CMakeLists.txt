cmake_minimum_required(VERSION 3.5)

option(UPDATE_TRANSLATIONS "update Translation" OFF)
MESSAGE(STATUS "UPDATE TRANSLATIONS: ${UPDATE_TRANSLATIONS}")

project(dice)

SET( dice_sources
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
    ../node/allsamenode.cpp
    ../node/listsetrollnode.cpp
    ../node/numbernode.cpp
    ../node/parenthesesnode.cpp
  # ../ node/forloopnode.cpp
    ../node/paintnode.cpp
    ../node/rerolldicenode.cpp
    ../node/scalaroperatornode.cpp
    ../node/sortresult.cpp
    ../node/startingnode.cpp
    ../node/filternode.cpp
    ../node/stringnode.cpp
    ../node/ifnode.cpp
    ../node/splitnode.cpp
    ../node/groupnode.cpp
    ../node/bind.cpp
    ../node/occurencecountnode.cpp
    ../node/uniquenode.cpp
    ../highlightdice.cpp
    ../node/variablenode.cpp
    ../node/valueslistnode.cpp
)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(EXECUTABLE_OUTPUT_PATH bin/)

# Find the QtWidgets library
find_package(Qt5Core)
include_directories(${Qt5Core_INCLUDES} ../)
add_definitions(${Qt5Core_DEFINITIONS})

IF(NOT DEFINED NO_PAINTER_OPERATOR)
    MESSAGE(STATUS "Compilation of operator Paint")
    find_package(Qt5Gui)
    include_directories(${Qt5Gui_INCLUDES} ../)
    add_definitions(${Qt5Gui_DEFINITIONS})
    add_definitions(-DPAINTER_OP)
endif()

set(MODE "cli")

ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)

set(dice_RESOURCES diceparser.qrc)
find_package(Qt5LinguistTools)
find_package(Qt5Svg)

IF(UPDATE_TRANSLATIONS)
	MESSAGE( update Translation )
    FILE(GLOB_RECURSE translate_dice_SRCS ../*.cpp ../*.h)
    SET(translate_SRCS ${translate_dice_SRCS})
    SET(dice_TS "${CMAKE_CURRENT_SOURCE_DIR}/i18n/dice_en.ts" "${CMAKE_CURRENT_SOURCE_DIR}/i18n/dice_fr.ts")
ELSE()
	MESSAGE( NO updates for translations)
    FILE(GLOB dice_TS "${CMAKE_CURRENT_SOURCE_DIR}/i18n/*.ts")
ENDIF(UPDATE_TRANSLATIONS)

if(Qt5Core_FOUND)
    MESSAGE(status  "find" ${dice_TS} ${translate_SRCS} )
    QT5_ADD_RESOURCES(dice_RESOURCES_RCC ${dice_RESOURCES})
    set(QT_PLUGINS_DIR "${Qt5Core_DIR}/../../../plugins")
    get_target_property(QT_LIBRARY_DIR Qt5::Core LOCATION)
    get_filename_component(QT_LIBRARY_DIR ${QT_LIBRARY_DIR} PATH)
endif()

SET( cli_sources
    main.cpp
    displaytoolbox.cpp
)

set(documentation_files ../HelpMe.md ../README.md)

add_executable( dice ${cli_sources} ${dice_QM} ${dice_sources}  ${documentation_files})
target_include_directories(dice PRIVATE ../include)


target_link_libraries(dice ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Svg_LIBRARIES})
INSTALL_TARGETS(/bin dice)
