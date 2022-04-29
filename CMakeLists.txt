cmake_minimum_required(VERSION 3.16)

project(diceparser VERSION 1.9.0 DESCRIPTION "Parser of dice command")

include(CTest)

option(BUILD_CLI ON)
option(NO_PAINTER_OPERATOR OFF)

IF(BUILD_TESTING)
  enable_testing()
ENDIF()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(src)
