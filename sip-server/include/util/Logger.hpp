#pragma once

#include <iostream>
#include <string_view>

namespace util {

enum class Level { Info, Warn, Error };

inline void log(Level level, std::string_view message) {
    (void)level;
    std::cout << message << std::endl;
}

}  // namespace util
