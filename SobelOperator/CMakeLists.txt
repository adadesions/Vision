cmake_minimum_required(VERSION 2.8)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
include_directories(include "../libs" "./src")
project( Sobel )
find_package( OpenCV REQUIRED HINTS "/usr/local/Cellar/opencv3/3.2.0/share/OpenCV")
add_executable(${PROJECT_NAME} ./src/workspace.cpp)
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
