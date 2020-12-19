#ifndef VRT_SOCKET_SRC_SOCKET_ABSTRACTION_H_
#define VRT_SOCKET_SRC_SOCKET_ABSTRACTION_H_

#include "libsocket/headers/exception.hpp"
#include "libsocket/headers/inetclientdgram.hpp"
#include "libsocket/headers/inetclientstream.hpp"
#include "libsocket/headers/libinetsocket.h"

namespace vrt::socket {

/**
 * Socket abstraction, just for a common call to snd().
 */
class Socket {
   public:
    virtual void send(const void* buf, size_t size) = 0;
};

/**
 * UDP socket.
 */
class SocketUdp : public Socket {
   public:
    SocketUdp(std::string host, std::string service) : sock_{std::move(host), std::move(service), LIBSOCKET_BOTH, 0} {}

    void send(const void* buf, size_t size) override { sock_.snd(buf, size, 0); }

   private:
    libsocket::inet_dgram_client sock_;
};

/**
 * TCP socket.
 */
class SocketTcp : public Socket {
   public:
    SocketTcp(std::string host, std::string service) : sock_{std::move(host), std::move(service), LIBSOCKET_BOTH, 0} {}

    void send(const void* buf, size_t size) override { sock_.snd(buf, size, 0); }

   private:
    libsocket::inet_stream sock_;
};

}  // namespace vrt::socket

#endif
