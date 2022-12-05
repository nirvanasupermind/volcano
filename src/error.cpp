#include <iostream>
#include <string>

#include "error.h"

namespace eris {
    void raise_error(const std::string& filename, size_t line, const std::string& msg) {
        throw std::string(filename + ":" + std::to_string(line) + ": " + msg);
    }
} // namespace eris