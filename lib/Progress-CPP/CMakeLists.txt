cmake_minimum_required(VERSION 3.9)

project(Progress-CPP LANGUAGES CXX)

# Name target the same as project
set(TARGET_NAME ${PROJECT_NAME})

# Add source files
file(GLOB FILES_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(${TARGET_NAME} ${FILES_SRC})

target_include_directories(${TARGET_NAME} PUBLIC include)

# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
