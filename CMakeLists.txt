cmake_minimum_required(VERSION 2.8)
project(geolocate-demo)

add_subdirectory(src)

add_executable(geolocate-demo demo.cpp)
target_link_libraries(geolocate-demo geolocate)
