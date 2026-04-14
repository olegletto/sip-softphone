#pragma once

#include <string>
#include <string_view>
#include <optional>

namespace sip {
    struct SdpStub {
        std::string ip;
        std::string port;
    };

    std::optional<SdpStub> parseSdpStub(std::string_view sdpBody);
}