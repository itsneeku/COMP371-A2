cmake_minimum_required(VERSION 3.10)

project(${PROJECT_NAME})

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(glfw3 REQUIRED)

# Add glad source files
add_library(glad ${CMAKE_SOURCE_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Add the main executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link libraries
target_link_libraries("${PROJECT_NAME}" PRIVATE glfw glad)
install(TARGETS ${PROJECT_NAME})