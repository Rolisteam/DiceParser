cmake_minimum_required(VERSION 3.16)

option(UPDATE_TRANSLATIONS "update Translation" OFF)
option(NO_PAINTER_OPERATOR "No PAINTING" OFF)
MESSAGE(STATUS "UPDATE TRANSLATIONS: ${UPDATE_TRANSLATIONS}")

project(dice)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(EXECUTABLE_OUTPUT_PATH bin/)

# Find the QtWidgets library
set(QT_REQUIRED_VERSION "5.12.0")



IF(NO_PAINTER_OPERATOR)
    find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Core Svg LinguistTools)
else()
    MESSAGE(STATUS "Compilation of operator Paint")
    find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Core Gui Svg LinguistTools)
    add_definitions(-DPAINTER_OP)
endif()


set(MODE "cli")

ADD_DEFINITIONS(
    -std=c++11
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

add_executable( dice ${cli_sources} ${dice_QM} ${documentation_files})
set(diceparser_shared_INCLUDE_DIRS "../diceparser")
target_include_directories(dice PRIVATE ../include ../ ../result ../node)
link_directories(BEFORE ${CMAKE_BINARY_DIR})
IF(NO_PAINTER_OPERATOR)
    target_link_libraries(dice PUBLIC Qt5::Core Qt5::Svg PRIVATE diceparser_shared)
ELSE()
    target_link_libraries(dice PUBLIC Qt5::Core Qt5::Gui Qt5::Svg PRIVATE diceparser_shared)
ENDIF()
INSTALL_TARGETS(/bin dice)
