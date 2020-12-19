cmake_minimum_required(VERSION 3.9)

project(
  vrt_common
  LANGUAGES CXX
  DESCRIPTION "Common files for VRT command line tools")

# Name target the same as project
set(TARGET_NAME ${PROJECT_NAME})

# Add source files
file(GLOB FILES_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(${TARGET_NAME} SHARED ${FILES_SRC})

# Set warning levels
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  enable_warnings(${TARGET_NAME})
endif()

# Set C++ standard
set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17)

# Include directory and library
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC ${Libvrt_INCLUDE_DIR})
target_include_directories(${TARGET_NAME} PUBLIC include)
target_link_libraries(${TARGET_NAME} ${Libvrt_LIBRARY})

# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
