cmake_minimum_required(VERSION 2.8)

option(UPDATE_TRANSLATIONS "update Translation" OFF)
MESSAGE(STATUS "UPDATE TRANSLATIONS: ${UPDATE_TRANSLATIONS}")


project(diceGui)


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Quick Qml Core Widgets Gui)
#find_package(Qt5QuickCompiler)

set(EXECUTABLE_OUTPUT_PATH bin/)

include_directories(${Qt5Core_INCLUDES} ${Qt5Widgets_INCLUDES} ${Qt5Gui_INCLUDES} ${Qt5Qml_INCLUDES} ${Qt5Quick_INCLUDES}../)
add_definitions(${Qt5Core_DEFINITIONS} ${Qt5Qml_DEFINITIONS} ${Qt5Quick_DEFINITIONS} ${Qt5Widgets_DEFINITIONS} ${Qt5Gui_DEFINITIONS} )

ADD_DEFINITIONS(
    -std=c++11
)

set(diceGui_RESOURCES diceparser.qrc)
FIND_PACKAGE(Qt5LinguistTools)


IF(UPDATE_TRANSLATIONS)
        MESSAGE( update Translation )
    FILE(GLOB_RECURSE translate_diceGui_SRCS ../*.cpp ../*.h)
    SET(translate_SRCS ${translate_dice_SRCS})
    SET(diceGui_TS "${CMAKE_CURRENT_SOURCE_DIR}/i18n/diceGui_en.ts" "${CMAKE_CURRENT_SOURCE_DIR}/i18n/diceGui_fr.ts")
ELSE()
        MESSAGE( NO updates for translations)
    FILE(GLOB diceGui_TS "${CMAKE_CURRENT_SOURCE_DIR}/i18n/*.ts")
ENDIF(UPDATE_TRANSLATIONS)

if(Qt5Core_FOUND)

    IF(UPDATE_TRANSLATIONS)
                MESSAGE(status  "find" ${diceGui_TS} ${translate_SRCS} )
        QT5_CREATE_TRANSLATION(diceGui_QM ${translate_SRCS} ${diceGui_TS})
    ELSE()
        QT5_ADD_TRANSLATION(diceGui_QM ${diceGui_TS})
    ENDIF()

    QT5_ADD_RESOURCES(diceGui_RESOURCES_RCC ${diceGui_RESOURCES})

    # guess plugins and libraries directory
    set(QT_PLUGINS_DIR "${Qt5Core_DIR}/../../../plugins")
    get_target_property(QT_LIBRARY_DIR Qt5::Core LOCATION)
    get_filename_component(QT_LIBRARY_DIR ${QT_LIBRARY_DIR} PATH)
endif()

SET( diceGui_sources
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
    ../node/filternode.cpp
    main.cpp
    maincontroler.cpp
    commandmodel.cpp
    ../highlightdice.cpp
)
qt5_add_resources(RESOURCE_ADDED mobile.qrc)

add_executable( diceGui ${diceGui_sources} ${diceGui_QM} ${RESOURCE_ADDED} )

target_link_libraries(diceGui ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Qml_LIBRARIES} ${Qt5Quick_LIBRARIES})
INSTALL_TARGETS(/bin diceGui)

#qt5_use_modules()
