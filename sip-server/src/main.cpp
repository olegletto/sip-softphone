#include "net/UdpServer.hpp"
#include <iostream>

int main() {
    try {
        net::UdpServer server("0.0.0.0", 5060);
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
