#pragma once
#include <string>
#include <cstdint>

namespace net {
    struct Endpoint {
        std::string ip;
        uint16_t port;
    };
}