#include "sip/Registrar.hpp"

namespace sip {
void Registrar::upsert(const std::string& aor, const Registration& registration) {
    registrations_[aor] = registration;
}

size_t Registrar::size() const {
    return registrations_.size();
}
}  // namespace sip