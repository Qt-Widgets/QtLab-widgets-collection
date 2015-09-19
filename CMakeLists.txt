project(QtLab)
cmake_minimum_required(VERSION 2.8.11)

enable_testing(true)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
include(CommonUtils)

# activate C++ 11
add_definitions(-std=c++11 -g)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools)

#file (GLOB SRC_FILES *.cc)
#foreach(SRC_FILE ${SRC_FILES})
#  string(REGEX REPLACE ".*/\(.*\)[.]cc$" "\\1" TGT_NAME ${SRC_FILE})
#  add_executable(${TGT_NAME} ${SRC_FILE})
#  target_link_libraries(${TGT_NAME} Qt5::Widgets)
#  add_test(${TGT_NAME} ${TGT_NAME})
#endforeach(SRC_FILE)

AUTO_ADD_SIMPLE_TARGETS()

add_subdirectory(sdi2)
add_subdirectory(layout)
add_subdirectory(widgets)
add_subdirectory(qtoolbutton_defaultaction)
