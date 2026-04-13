#include <optional>
#include <string_view>

#include "sip/Headers.hpp"
#include "sip/Message.hpp"
#include "sip/RequestLine.hpp"

namespace sip {

std::optional<Message> parseMessage(std::string_view bytes) {
    const size_t pos1 = bytes.find("\r\n");
    if (pos1 == std::string_view::npos) {
        return std::nullopt;
    }

    const std::string_view requestLine = bytes.substr(0, pos1);
    const auto request = parseRequestLine(requestLine);
    if (!request.has_value()) {
        return std::nullopt;
    }

    const size_t headerStart = pos1 + 2;
    if (headerStart > bytes.size()) {
        return std::nullopt;
    }

    Message message;
    message.request = request;

    const size_t delim = bytes.find("\r\n\r\n", headerStart);
    std::string_view block;
    std::string_view body;

    if (delim == std::string_view::npos) {
        block = bytes.substr(headerStart);
        body = {};
    } else {
        block = bytes.substr(headerStart, delim - headerStart);
        body = bytes.substr(delim + 4);
    }

    message.headers = parseHeaders(block);
    message.body = body;
    return message;
}

}  // namespace sip
