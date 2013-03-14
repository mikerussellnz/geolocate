cmake_minimum_required(VERSION 2.8)
project(geolocate)

set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	"${PROJECT_SOURCE_DIR}/"
)

find_package(CURL)
find_package(exiv2)

add_executable(geolocate demo.cpp parse-eyefi-log.cpp exif-gps-exiv2.cpp geolocate.cpp cJSON/cJSON.c)
target_link_libraries(geolocate ${CURL_LIBRARY} ${EXIV2_LIBRARIES})
