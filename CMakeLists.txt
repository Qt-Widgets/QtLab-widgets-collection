project(QtLab)
cmake_minimum_required(VERSION 2.8.11)

enable_testing(true)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

# activate C++ 11
add_definitions(-std=c++11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)

add_subdirectory(sdi2)
