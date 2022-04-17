cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-permissive -pedantic -Wall -Wextra")
set(CMAKE_AUTOMOC ON)

find_package(Qt6Core REQUIRED)
find_package(Qt6Gui REQUIRED)
find_package(Qt6Svg REQUIRED)
find_package(Qt6Concurrent REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

#SET (CMAKE_EXE_LINKER_FLAGS "-static")

add_definitions(-DPAINTER_OP)
set(MODE "cli")



set(fdsource testcase.cpp)

add_executable(fuzzTestCase ${fdsource} )


target_link_libraries(fuzzTestCase libdiceparser.a Qt6::Core Qt6::Gui Qt6::Svg Qt6::Concurrent)
#target_link_libraries(fuzzTestCase )
