cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # -DVERBOSE")

add_definitions("-DMIXED")

set(sources
    dgramclient.cpp
    dgramoverstream.cpp
    framing.cpp
    inetbase.cpp
    inetclientstream.cpp
    inetserverdgram.cpp
    select.cpp
    streamclient.cpp
    unixclientdgram.cpp
    unixdgram.cpp
    unixserverstream.cpp
    exception.cpp
    inetclientdgram.cpp
    inetdgram.cpp
    inetserverstream.cpp
    socket.cpp
    unixbase.cpp
    unixclientstream.cpp
    unixserverdgram.cpp)

add_definitions(-fPIC) # for the static library which needs to be linked into
                       # the shared libsocket++.so object.
add_library(socket++_o OBJECT ${sources})

if(BUILD_SHARED_LIBS)
  add_library(socket++ SHARED $<TARGET_OBJECTS:socket++_o>)

  target_link_libraries(socket++ socket_int)

  target_include_directories(socket++ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../..)

  install(TARGETS socket++ DESTINATION ${LIB_DIR})
endif()

if(BUILD_STATIC_LIBS)
  add_library(socket++_int STATIC $<TARGET_OBJECTS:socket++_o>)

  set_target_properties(socket++_int PROPERTIES OUTPUT_NAME socket++)

  target_link_libraries(socket++_int socket_int)

  install(TARGETS socket++_int DESTINATION ${LIB_DIR})
endif()
