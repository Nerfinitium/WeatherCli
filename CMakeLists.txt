cmake_minimum_required(VERSION 3.22)
project(untitled2 C)

set(CMAKE_C_STANDARD 11)

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
