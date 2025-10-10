CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

# Set the project name
PROJECT(synf)

# Find curl package
find_package(CURL REQUIRED)

# add the executable
ADD_EXECUTABLE(main

        # OSX Files
        source/osx/cpu.c
        source/osx/ram.c
        source/osx/disk.c
        source/osx/device.c


        # Linux Files
        source/linux/cpu.c
        source/linux/ram.c
        source/linux/disk.c
        source/linux/device.c

        # Main File
        source/main.c

)

TARGET_LINK_LIBRARIES(main PRIVATE CURL::libcurl)
TARGET_INCLUDE_DIRECTORIES(main PRIVATE source/lib/osx source/lib/linux)
