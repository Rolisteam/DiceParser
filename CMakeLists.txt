cmake_minimum_required(VERSION 3.10)

project(DiceParser)
add_subdirectory(diceparser)
add_subdirectory(cli)

option(TESTS "compile & run tests" OFF)
if (TESTS)
add_subdirectory(tests)
endif()




