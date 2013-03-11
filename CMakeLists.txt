cmake_minimum_required(VERSION 2.8)
project(geolocate)

find_package(CURL)

add_executable(geolocate geolocate.cpp ../cJSON/cJSON.c)
target_link_libraries(geolocate ${CURL_LIBRARY})
