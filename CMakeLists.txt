cmake_minimum_required(VERSION 3.5)
project(TheCar)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

#link_directories("/usr/local/lib")

link_libraries(
        opencv_core
        opencv_highgui
        opencv_imgproc
        opencv_video
        opencv_plot
        opencv_video
        opencv_videoio
        opencv_photo
        opencv_xphoto
        opencv_imgcodecs
        lua
        serial
        stdc++fs
        pthread
)

set(SOURCE_FILES main.cpp TheCarCV.cpp TheCarCV.hpp Config.cpp Config.hpp RoboControl.cpp RoboControl.hpp Detect.cpp Detect.hpp maps.hpp ImageVector.cpp ImageVector.hpp)
add_executable(TheCar ${SOURCE_FILES})