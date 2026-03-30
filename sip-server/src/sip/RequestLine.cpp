#include "sip/RequestLine.hpp"
#include <sstream>
#include <string_view>
#include <string>

namespace sip {
    RequestLine parseRequestLine(std::string_view line) {
        RequestLine requestLine;
        std::string s(line);
        std::istringstream iss(s);
        iss >> requestLine.method >> requestLine.uri >> requestLine.sipVersion;
        return requestLine;
    }
}

