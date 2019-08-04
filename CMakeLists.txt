cmake_minimum_required(VERSION 3.0.0)

project(vision-stuff-cpp)

set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(OpenCV REQUIRED)
find_package(dlib REQUIRED)

if(NOT dlib_FOUND)
	message(FATAL_ERROR "dlib not found")
endif(NOT dlib_FOUND)

if(NOT OpenCV_FOUND)
	message(FATAL_ERROR "OpenCV not available")
endif(NOT OpenCV_FOUND)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(hello_dlib hello_dlib.cpp)
target_link_libraries(hello_dlib dlib::dlib)

add_executable(display display.cpp)
target_link_libraries(display ${OpenCV_LIBS})

add_executable(find_faces find_faces.cpp)
target_link_libraries(find_faces ${OpenCV_LIBS} dlib::dlib)

add_executable(face_thuglife face_thuglife.cpp)
target_link_libraries(find_faces ${OpenCV_LIBS} dlib::dlib)
