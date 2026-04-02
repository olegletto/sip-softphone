#pragma once

#include "net/Endpoint.hpp"
#include <cstddef>

namespace net {
    struct Datagram {
        size_t size;
        Endpoint endpoint;
    };
}
