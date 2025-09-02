cmake_minimum_required(VERSION 3.10)
project(CurryVision LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)

add_library(curryvision_core
  src/video_stream.cpp
  src/ball_detector.cpp
  src/arc_tracker.cpp
  src/types.cpp
)
target_include_directories(curryvision_core PUBLIC include)
target_link_libraries(curryvision_core PUBLIC ${OpenCV_LIBS})

add_executable(curry_vision src/main.cpp)
target_link_libraries(curry_vision PRIVATE curryvision_core)
