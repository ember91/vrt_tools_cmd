cmake_minimum_required(VERSION 3.9)

project(
  vrt_truncate
  LANGUAGES CXX
  DESCRIPTION "Change length of packet stream by removing packets")

# Name target the same as project
set(TARGET_NAME ${PROJECT_NAME})

# Add source files
file(GLOB FILES_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${TARGET_NAME} ${FILES_SRC})

# Add preprocessor flag with program description
target_compile_definitions(
  ${TARGET_NAME} PUBLIC "CMAKE_PROJECT_NAME=\"${PROJECT_NAME}\""
                        "CMAKE_PROJECT_DESCRIPTION=\"${PROJECT_DESCRIPTION}\"")

# Set warning levels
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  enable_warnings(${TARGET_NAME})
endif()

if(${TEST})
  # add_subdirectory(test)
endif()

# Set C++ standard
set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17)

# Include directory and library
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC)
target_link_libraries(${TARGET_NAME} vrt vrt_common CLI11 Progress-CPP)

# Install executable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
