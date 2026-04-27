#pragma once

#include <string>
#include <chrono>
#include <cstddef>
#include <map>
#include "net/Endpoint.hpp"

namespace sip {
struct Registration {
    net::Endpoint endpoint;
    std::string contact;
    std::chrono::steady_clock::time_point expiresAt;
};

class Registrar {
    public:
    void upsert(const std::string& aor, const Registration& registration);
    size_t purgeExpired(std::chrono::steady_clock::time_point now);
    size_t size() const;
    private:
    std::map<std::string, Registration> registrations_;
};
}  // namespace sip