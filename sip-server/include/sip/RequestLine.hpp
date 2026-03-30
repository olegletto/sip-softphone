#pragma once

#include <string>
#include <string_view>

namespace sip {
    struct RequestLine {
        std::string method;
        std::string uri;
        std::string sipVersion;
    };
    
    RequestLine parseRequestLine(std::string_view line);
}

