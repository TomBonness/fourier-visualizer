cmake_minimum_required(VERSION 3.10)
project(fourier-visualizer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/FourierEngine.cpp
)

# Create executable
add_executable(fourier-visualizer ${SOURCES})

# Link SFML libraries
target_link_libraries(fourier-visualizer SFML::Graphics SFML::Window SFML::System)
