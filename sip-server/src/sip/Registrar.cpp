#include "sip/Registrar.hpp"

namespace sip {
void Registrar::upsert(const std::string& aor, const Registration& registration) {
    registrations_[aor] = registration;
}

size_t Registrar::purgeExpired(std::chrono::steady_clock::time_point now) {
    size_t purged = 0;
    for (auto it = registrations_.begin(); it != registrations_.end();) {
        if (it->second.expiresAt <= now) {
            it = registrations_.erase(it);
            purged++;
        } else {
            ++it;
        }
    }
    return purged;
}

size_t Registrar::size() const {
    return registrations_.size();
}
}  // namespace sip