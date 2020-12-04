cmake_minimum_required(VERSION 3.0)

project(generate LANGUAGES CXX)

# Add test source files
file(GLOB APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(app_src ${APP_SOURCES})
  get_filename_component(name ${app_src} NAME_WE)
  add_executable(${name} ${app_src})
  target_link_libraries(${name} vrt)
endforeach()
