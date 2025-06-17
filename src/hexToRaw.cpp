#include "hexToRaw.h"
#include <string>

std::string hexToRaw(const std::string& hex) {
    std::string raw;
    for (size_t i = 0; i < hex.length(); i += 2) {
        raw.push_back((char)strtol(hex.substr(i, 2).c_str(), nullptr, 16));
    }
    return raw;
}