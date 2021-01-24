cmake_minimum_required(VERSION 3.9)

# Name target
set(TARGET_NAME run_length_tests)

# Add test source files
file(GLOB SRC_FILES CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/../test/src/*.cpp)
add_executable(
  ${TARGET_NAME} ${SRC_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/../src/printer.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/../src/process.cpp)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
target_include_directories(${TARGET_NAME} PUBLIC ${GTEST_INCLUDE_DIR})

# Set warning levels
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  enable_warnings(${TARGET_NAME})
endif()

# Set C++ standard
set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17)

# Add include directory
target_include_directories(${TARGET_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include/)

# Link executable
target_link_libraries(${TARGET_NAME} vrt ${GTEST_LIBRARIES} pthread vrt_common
                      Progress-CPP)

# Add test
add_test(name ${TARGET_NAME} COMMAND ${TARGET_NAME})
