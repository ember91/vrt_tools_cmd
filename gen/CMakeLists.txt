cmake_minimum_required(VERSION 3.9)

project(generate LANGUAGES CXX)

# Add test source files
file(GLOB APP_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(app_src ${APP_SOURCES})
  get_filename_component(TARGET_NAME ${app_src} NAME_WE)
  add_executable(${TARGET_NAME} ${app_src})
  set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17)
  target_link_libraries(${TARGET_NAME} vrt vrt_common)
endforeach()
