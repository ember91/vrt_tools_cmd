cmake_minimum_required(VERSION 3.0)

project(generate LANGUAGES CXX)

# Add test source files
file(GLOB APP_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(app_src ${APP_SOURCES})
  get_filename_component(TARGET_NAME ${app_src} NAME_WE)
  add_executable(${TARGET_NAME} ${app_src})
  target_link_libraries(${TARGET_NAME} vrt)
  target_include_directories(${TARGET_NAME} SYSTEM
                             PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
endforeach()
