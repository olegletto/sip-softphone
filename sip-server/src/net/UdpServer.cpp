#include "net/UdpServer.hpp"
#include "sip/Message.hpp"
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
                } else {
                    line << " method=?";
                }
                const auto& h = msg->headers;
                const auto cid = h.find("call-id");
                const auto cs = h.find("cseq");
                line << " call-id=" << (cid != h.end() ? cid->second : std::string("-"));
                line << " cseq=" << (cs != h.end() ? cs->second : std::string("-"));
            } else {
                line << " sip-parse-failed preview=";
                line.write(package.data(), static_cast<std::streamsize>(
                    std::min(package.size(), static_cast<size_t>(80))));
            }

            util::log(util::Level::Info, line.str());
        }
    }
}
