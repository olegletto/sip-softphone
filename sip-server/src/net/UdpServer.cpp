#include "net/UdpServer.hpp"
#include "net/Endpoint.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <string_view>
#include <array>

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
    
    void UdpServer::run() {
        std::array<char, 1024> buf;
        sockaddr_in client{};
        socklen_t len = sizeof(client);
    
        while (true) {
            ssize_t n = recvfrom(sock_, buf.data(), buf.size(), 0, (struct sockaddr*)&client, &len);
            if (n < 0) continue;

            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client.sin_addr, ip, INET_ADDRSTRLEN);
            Endpoint endpoint {std::string(ip), ntohs(client.sin_port)};
            
            std::string_view msg(buf.data(), std::min((size_t)n, (size_t)80));
            std::cout << endpoint.ip << ":" << endpoint.port << " -> " << msg << std::endl;
        }
    }
}

