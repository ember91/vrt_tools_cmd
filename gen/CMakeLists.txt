cmake_minimum_required(VERSION 3.0)

project(generate LANGUAGES CXX)

# Add test source files
add_executable(data_packet ${CMAKE_CURRENT_SOURCE_DIR}/write_data_packets.cpp)

add_executable(if_context_packet
               ${CMAKE_CURRENT_SOURCE_DIR}/write_if_context_packets.cpp)

target_link_libraries(data_packet vrt)
target_link_libraries(if_context_packet vrt)
