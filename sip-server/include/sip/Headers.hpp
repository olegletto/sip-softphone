#pragma once

#include <map>
#include <string>
#include <string_view>

namespace sip {
    std::map<std::string, std::string> parseHeaders(std::string_view block);
}