cmake_minimum_required(VERSION 3.10)
project(CurryVision LANGUAGES CXX)

# Require C++17 (safe baseline)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV (installed via Homebrew)
find_package(OpenCV REQUIRED)

# Add source files
add_executable(curry_vision src/main.cpp)

# Link OpenCV to executable
target_link_libraries(curry_vision PRIVATE ${OpenCV_LIBS})
