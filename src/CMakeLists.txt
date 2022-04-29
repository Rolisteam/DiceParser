cmake_minimum_required(VERSION 3.16)


add_subdirectory(libparser)
add_subdirectory(bin)

if(TESTS)
    add_subdirectory(tests)
endif()