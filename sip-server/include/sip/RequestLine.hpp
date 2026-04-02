#pragma once

#include <string>
#include <string_view>
#include <optional>

namespace sip {
    struct RequestLine {
        std::string method;
        std::string uri;
        std::string sipVersion;
    };
    
    std::optional<RequestLine> parseRequestLine(std::string_view line);
}

