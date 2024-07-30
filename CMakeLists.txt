cmake_minimum_required(VERSION 3.10)

project(${PROJECT_NAME})

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # STATIC stuff (Windows)
# if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
#         set(BUILD_FOR_WIN TRUE)
# endif()
# option(STATIC_BUILD "Build a static binary." ${BUILD_FOR_WIN})

# if (STATIC_BUILD)
#         set(CMAKE_EXE_LINKER_FLAGS "-static")
#         set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" CONFIG)
#         set(BUILD_SHARED_LIBS OFF)
# endif()

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