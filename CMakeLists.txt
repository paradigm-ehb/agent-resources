CMAKE_MINIMUM_REQUIRED(VERSION 3.24)

SET(CMAKE_C_STANDARD 23)
SET(CMAKE_C_STANDARD_REQUIRED TRUE)

PROJECT(synf-core)

ADD_EXECUTABLE(main

        source/main.c
        source/lib.c
)

TARGET_INCLUDE_DIRECTORIES(main PRIVATE source/lib/osx source/lib/linux)
