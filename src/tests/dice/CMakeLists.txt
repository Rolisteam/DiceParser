set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-permissive -pedantic -Wall")
set(CMAKE_AUTOMOC ON)
find_package(Qt6 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Core Gui Svg Test)

set(test_source testnode.cpp tst_dice.cpp)
add_executable(test_dice ${test_source})

target_link_libraries(test_dice PUBLIC Qt6::Core Qt6::Gui Qt6::Test PRIVATE diceparser_shared)
target_include_directories(test_dice PRIVATE ../../libparser/include ../../libparser/ ../../libparser/node ../../libparser/result)
add_test(NAME tst_diceparser COMMAND test_dice)
