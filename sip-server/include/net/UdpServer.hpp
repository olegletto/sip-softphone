#pragma once

#include <string>
#include <cstdint>

namespace net {
    class UdpServer {
        public:
            // Конструктор: инициализирует адрес и порт, открывает сокет
            UdpServer(const std::string& bindAddress, uint16_t port);
            
            // Деструктор: закрывает сокет через RAII
            ~UdpServer();
        
            // Запрет копирования (семантика владения дескриптором)
            UdpServer(const UdpServer&) = delete;
            UdpServer& operator=(const UdpServer&) = delete;
        
            // Основной цикл работы
            void run();
        
        private:
            int sock_ = -1; // RAII-обертка над дескриптором
        };
}
