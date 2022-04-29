cmake_minimum_required(VERSION 3.16)

project(diceparser VERSION 1.9.0 DESCRIPTION "Parser of dice command")


option(BUILD_CLI OFF)
option(TESTS OFF)
option(NO_PAINTER_OPERATOR OFF)

IF(TESTS)
  enable_testing()
ENDIF()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(src)
