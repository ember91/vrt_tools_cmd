cmake_minimum_required(VERSION 3.9)

project(
  vrt_socket
  LANGUAGES CXX
  DESCRIPTION
    "Output packets in a vita49 VRT format file with the same time interval as packet timestamps suggest"
)

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
  add_subdirectory(test)
endif()

# Set C++ standard
set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17)

# Include directory and library
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC ${Libvrt_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} ${Libvrt_LIBRARY} CLI11 Progress-CPP
                      vrt_common socket++)

# Install executable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)