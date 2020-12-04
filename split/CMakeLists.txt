cmake_minimum_required(VERSION 3.0)

project(
  vrt_split
  LANGUAGES CXX
  DESCRIPTION
    "Split a vita49 VRT format files into multiple files depending on class and stream ID"
)

# Name target the same as project
set(TARGET_NAME ${PROJECT_NAME})

# Add source files
file(GLOB FILES_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${TARGET_NAME} ${FILES_SRC})

# Set warning levels
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  # TODO: Should it
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(
      ${TARGET_NAME} PRIVATE -Weverything -Wno-padded -Wno-newline-eof
                             -Wno-c++98-compat)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic
                                                  -Wshadow)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    target_compile_options(${TARGET_NAME} PRIVATE -w3) # /W5 on Windows
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${TARGET_NAME} PRIVATE /W4)
  endif()
endif()

if(${TEST})
  # add_subdirectory(test)
endif()

# Set C++ standard
set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17)

# Include directory and library
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC ${Libvrt_INCLUDE_DIR})
target_include_directories(${TARGET_NAME} SYSTEM
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../common)
target_link_libraries(${TARGET_NAME} ${Libvrt_LIBRARY})

# Install executable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)