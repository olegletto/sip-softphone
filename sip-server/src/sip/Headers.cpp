#include "sip/Headers.hpp"
#include <cctype>
#include <string>
#include <string_view>

namespace {

std::string_view trim(std::string_view s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) {
        s.remove_prefix(1);
    }
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) {
        s.remove_suffix(1);
    }
    return s;
}

std::string toLowerAscii(std::string_view s) {
    std::string out;
    out.reserve(s.size());
    for (unsigned char c : s) {
        out.push_back(static_cast<char>(std::tolower(c)));
    }
    return out;
}

}  // namespace

namespace sip {

std::map<std::string, std::string> parseHeaders(std::string_view block) {
    std::map<std::string, std::string> headers;

    while (!block.empty()) {
        std::string_view line;
        const auto sep = block.find("\r\n");
        if (sep == std::string_view::npos) {
            line = block;
            block = {};
        } else {
            line = block.substr(0, sep);
            block.remove_prefix(sep + 2);
        }

        line = trim(line);
        if (line.empty()) {
            continue;
        }

        const auto colon = line.find(':');
        if (colon == std::string_view::npos) {
            continue;
        }

        std::string_view name = trim(line.substr(0, colon));
        std::string_view value = trim(line.substr(colon + 1));
        if (name.empty()) {
            continue;
        }

        const std::string key = toLowerAscii(name);
        headers.insert_or_assign(key, std::string(value));
    }

    return headers;
}

}  // namespace sip
