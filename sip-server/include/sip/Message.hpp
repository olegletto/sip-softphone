#pragma once

#include "sip/RequestLine.hpp"
#include <optional>
#include <string>
#include <string_view>
#include <map>

namespace sip {
    struct Message {
        std::optional<RequestLine> request;
        std::map<std::string, std::string> headers;
        std::string_view body; // just view, no copy
    };

    std::optional<Message> parseMessage(std::string_view bytes);
}