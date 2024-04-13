cmake_minimum_required(VERSION 3.22)
project(untitled2 C)

set(CMAKE_C_STANDARD 11)

list(APPEND CMAKE_PREFIX_PATH "/usr/include/x86_64-linux-gnu/curl")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu")

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

add_executable(untitled2 main.c
        locationdetection.c
        locationdetection.h
        Cmanager.c
        Cmanager.h
        Cmanager.h
        locationdetection.h
        locationdetection.c)
target_link_libraries(untitled2 ${CURL_LIBRARIES} ${LIBXML2_LIBRARIES})