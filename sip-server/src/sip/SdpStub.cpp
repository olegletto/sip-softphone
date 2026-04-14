#include "sip/SdpStub.hpp"

#include <string>
#include <string_view>

namespace sip {

namespace {

std::string_view trimIpToken(std::string_view s) {
    std::size_t end = 0;
    while (end < s.size()) {
        const char ch = s[end];
        if (ch == '\r' || ch == '\n' || ch == ' ') {
            break;
        }
        ++end;
    }
    return s.substr(0, end);
}

}  // namespace

std::optional<SdpStub> parseSdpStub(std::string_view sdpBody) {
    if (sdpBody.empty()) {
        return std::nullopt;
    }

    // Учебная заглушка: ищем типичные строки SDP. Content-Type — в SIP-заголовках,
    // в теле после пустой строки обычно только SDP (v=0, c=, m=, ...).
    constexpr std::string_view kConnection = "c=IN IP4 ";
    constexpr std::string_view kMediaAudio = "m=audio ";

    const std::size_t posC = sdpBody.find(kConnection);
    const std::size_t posM = sdpBody.find(kMediaAudio);
    if (posC == std::string_view::npos || posM == std::string_view::npos) {
        return std::nullopt;
    }

    const std::string_view ip = trimIpToken(sdpBody.substr(posC + kConnection.size()));
    if (ip.empty()) {
        return std::nullopt;
    }

    const std::size_t portStart = posM + kMediaAudio.size();
    std::size_t portEnd = portStart;
    while (portEnd < sdpBody.size()) {
        const char ch = sdpBody[portEnd];
        if (ch == ' ' || ch == '\r' || ch == '\n') {
            break;
        }
        ++portEnd;
    }

    const std::string_view port = sdpBody.substr(portStart, portEnd - portStart);
    if (port.empty()) {
        return std::nullopt;
    }

    return SdpStub{std::string(ip), std::string(port)};
}

}  // namespace sip
