cmake_minimum_required(VERSION 3.0)

# Name target
set(TARGET_NAME run_merge_tests)

# Add test source files
file(GLOB SRC_FILES CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/../test/src/*.cpp)
add_executable(
  ${TARGET_NAME}
  ${SRC_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/../src/output_stream.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/process.cpp)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
target_include_directories(${TARGET_NAME} PUBLIC ${GTEST_INCLUDE_DIR})

# Set warning levels
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(
      ${TARGET_NAME}
      PRIVATE -Weverything
              -Wno-c++98-compat-pedantic
              -Wno-padded
              -Wno-newline-eof
              -Wno-global-constructors
              -Wno-weak-vtables
              -Wno-used-but-marked-unused)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic
                                                  -Wshadow)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    target_compile_options(${TARGET_NAME} PRIVATE -w3) # /W5 on Windows
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${TARGET_NAME} PRIVATE /W4)
  endif()
endif()

# Set C++ standard
set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17)

# Add include directory
target_include_directories(${TARGET_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include/)
target_include_directories(${TARGET_NAME} SYSTEM
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/src)

# Link executable
target_link_libraries(${TARGET_NAME} vrt ${GTEST_LIBRARIES} pthread vrt_lib)

# Add test
add_test(name ${TARGET_NAME} COMMAND ${TARGET_NAME})
