set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        "${CMAKE_CURRENT_SOURCE_DIR}/../"
)

set(GEOLOCATE_SRCS
  parse-eyefi-log.cpp 
  exif-gps-exiv2.cpp 
  geolocate.cpp 
  ../cJSON/cJSON.c
)

find_package(CURL)
find_package(exiv2)

add_library(geolocate ${GEOLOCATE_SRCS})
set_target_properties(geolocate PROPERTIES linker_language CXX)
target_link_libraries(geolocate ${CURL_LIBRARY} ${EXIV2_LIBRARIES})



