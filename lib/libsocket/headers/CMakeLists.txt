cmake_minimum_required(VERSION 2.6)

set(headers
    ./unixdgram.hpp
    ./exception.hpp
    ./inetclientdgram.hpp
    ./libinetsocket.h
    ./unixserverstream.hpp
    ./dgramclient.hpp
    ./streamclient.hpp
    ./inetserverstream.hpp
    ./unixclientdgram.hpp
    ./socket.hpp
    ./inetbase.hpp
    ./inetserverdgram.hpp
    ./unixclientstream.hpp
    ./libunixsocket.h
    ./select.hpp
    ./inetclientstream.hpp
    ./unixbase.hpp
    ./unixserverdgram.hpp
    ./inetdgram.hpp
    ./dgramoverstream.hpp
    ./framing.hpp)

if(IS_LINUX)
  set(headers ${headers} ./epoll.hpp)
endif()

install(FILES ${headers} DESTINATION ${HEADER_DIR})
