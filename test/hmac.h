#pragma once

#include <string>
#include <cstdint>
#include <vector>

class HMAC {
public:
    HMAC() = delete;
    static std::string compute(const std::string &key, const std::vector<uint8_t> &data);
};
