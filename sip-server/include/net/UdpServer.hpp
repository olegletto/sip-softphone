#pragma once

#include <array>
#include <string>
#include <cstdint>
#include <optional>
#include "net/Datagram.hpp"

namespace net {
    class UdpServer {
    public:
        UdpServer(const std::string& bindAddress, uint16_t port);
        ~UdpServer();

        UdpServer(const UdpServer&) = delete;
        UdpServer& operator=(const UdpServer&) = delete;

        void run();

    private:
        std::optional<Datagram> tryReceive(std::array<char, 1024>& buf);

        int sock_ = -1;
    };
}
