#include "net/UdpServer.hpp"
#include "sip/Message.hpp"
#include "sip/SdpStub.hpp"
#include "sip/Registrar.hpp"
#include "util/Logger.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <sstream>
#include <string_view>
#include <array>
#include <optional>
#include <chrono>
#include <cerrno>
#include <cstring>

namespace net {
    UdpServer::UdpServer(const std::string& addr_str, uint16_t port) {
        sock_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock_ == -1) throw std::runtime_error("socket failed");
    
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, addr_str.c_str(), &addr.sin_addr);
    
        if (bind(sock_, (struct sockaddr*)&addr, sizeof(addr)) == -1) 
            throw std::runtime_error("bind failed");
    }
    
    UdpServer::~UdpServer() {
        if (sock_ != -1) close(sock_);
    }

    std::optional<Datagram> UdpServer::tryReceive(std::array<char, 1024>& buf) {
        sockaddr_in client{};
        socklen_t len = sizeof(client);

        ssize_t n = 0;
        for (;;) {
            len = sizeof(client);
            n = recvfrom(sock_, buf.data(), buf.size(), 0, (struct sockaddr*)&client, &len);
            if (n >= 0) break;

            const int e = errno;
            if (e == EINTR) {
                continue;
            }
            util::log(util::Level::Warn, strerror(e));
            return std::nullopt;
        }

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client.sin_addr, ip, INET_ADDRSTRLEN);
        Endpoint endpoint{std::string(ip), ntohs(client.sin_port)};

        return Datagram{static_cast<size_t>(n), endpoint};
    }
    
    void UdpServer::run() {
        std::array<char, 1024> buf;
        sip::Registrar registrar;
    
        while (true) {
            auto d = tryReceive(buf);
            if (!d) continue;
            auto& [size, endpoint] = d.value();

            const std::string_view package(buf.data(), size);
            std::ostringstream line;
            line << endpoint.ip << ':' << endpoint.port;

            if (const auto msg = sip::parseMessage(package)) {
                if (msg->request) {
                    line << " method=" << msg->request->method;

                    if (msg->request->method == "REGISTER") {
                        const auto& h = msg->headers;
                        const auto to = h.find("to");
                        const auto expires = h.find("expires");
                        const auto contact = h.find("contact");
                        if (to == h.end() || to->second.empty()) {
                            util::log(util::Level::Warn, "REGISTER missing To header");
                        } else {
                            int expiresSeconds = 3600;
                            if (expires != h.end() && !expires->second.empty()) {
                                try {
                                    expiresSeconds = std::stoi(expires->second);
                                } catch (...) {
                                    expiresSeconds = 3600;
                                }
                                if (expiresSeconds <= 0) {
                                    expiresSeconds = 3600;
                                }
                            }

                            const auto now = std::chrono::steady_clock::now();
                            const std::string& aor = to->second;
                            const std::string contactValue = (contact != h.end() ? contact->second : "");

                            registrar.upsert(
                                aor,
                                sip::Registration{endpoint, contactValue, now + std::chrono::seconds(expiresSeconds)});

                            line << " aor=" << aor << " expires=" << expiresSeconds
                                 << " registrar-size=" << registrar.size()
                                 << " from=" << endpoint.ip << ':' << endpoint.port;
                        }
                    }
                } else {
                    line << " method=?";
                }
                const auto& h = msg->headers;
                const auto cid = h.find("call-id");
                const auto cs = h.find("cseq");
                line << " call-id=" << (cid != h.end() ? cid->second : std::string("-"));
                line << " cseq=" << (cs != h.end() ? cs->second : std::string("-"));

                const auto ct = h.find("content-type");
                const bool sdpOffer =
                    ct != h.end() &&
                    ct->second.find("application/sdp") != std::string::npos;
                if (sdpOffer && !msg->body.empty()) {
                    if (const auto sdp = sip::parseSdpStub(msg->body)) {
                        line << " rtp-offer=" << sdp->ip << ':' << sdp->port;
                    }
                }
            } else {
                line << " sip-parse-failed preview=";
                line.write(package.data(), static_cast<std::streamsize>(
                    std::min(package.size(), static_cast<size_t>(80))));
            }

            util::log(util::Level::Info, line.str());
        }
    }
}
