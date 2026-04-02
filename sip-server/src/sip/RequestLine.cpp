#include "sip/RequestLine.hpp"
#include <istream>
#include <sstream>
#include <string>
#include <string_view>

namespace sip {

std::optional<RequestLine> parseRequestLine(std::string_view line) {
    RequestLine requestLine;
    std::string s(line);
    std::istringstream iss(s);

    iss >> requestLine.method >> requestLine.uri >> requestLine.sipVersion;
    if (iss.fail()) {
        return std::nullopt;
    }
    if (requestLine.method.empty() || requestLine.uri.empty() ||
        requestLine.sipVersion.empty() || !requestLine.sipVersion.starts_with("SIP/")) {
        return std::nullopt;
    }

    iss >> std::ws;
    if (!iss.eof()) {
        return std::nullopt;
    }

    return requestLine;
}

}  // namespace sip
