cmake_minimum_required(VERSION 3.5)
project(tst_diceParser)

enable_testing(true)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Test REQUIRED)

add_test(tst_diceParser tst_dice.cpp)

target_link_libraries(foo Qt5::Test)
