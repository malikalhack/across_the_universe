cmake_minimum_required(VERSION 3.16)

project(AcrossTheUniverse LANGUAGES CXX)

find_package(SDL2 REQUIRED CONFIG)

add_executable(AcrossTheUniverse
    main.cpp
    universe.cpp
)

target_compile_features(AcrossTheUniverse PRIVATE cxx_std_11)
target_link_libraries(AcrossTheUniverse PRIVATE SDL2::SDL2)
