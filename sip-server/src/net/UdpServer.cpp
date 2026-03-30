#include "net/UdpServer.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string_view>
#include <array>

UdpServer::UdpServer(const std::string& addr_str, uint16_t port) {
    sock_ = socket(AF_INET, SOCK_DGRAM, 0); // Создание сокета (целое число file descriptor)
    if (sock_ == -1) throw std::runtime_error("socket failed"); //если сокет не создан, то выбрасывается ошибка 

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, addr_str.c_str(), &addr.sin_addr);

    if (bind(sock_, (struct sockaddr*)&addr, sizeof(addr)) == -1) 
        throw std::runtime_error("bind failed");
}

UdpServer::~UdpServer() {
    if (sock_ != -1) close(sock_); //закрытие сокета
}

void UdpServer::run() {
    std::array<char, 1024> buf; //буфер с ограничением размера 1024 байта
    sockaddr_in client{};
    socklen_t len = sizeof(client);

    while (true) {
        ssize_t n = recvfrom(sock_, buf.data(), buf.size(), 0, (struct sockaddr*)&client, &len);
        if (n < 0) continue;

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client.sin_addr, ip, INET_ADDRSTRLEN);
        
        std::string_view msg(buf.data(), std::min((size_t)n, (size_t)80));
        std::cout << ip << ":" << ntohs(client.sin_port) << " -> " << msg << std::endl;
    }
}
