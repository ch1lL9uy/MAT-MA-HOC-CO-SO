#pragma once

#include <vector>
#include <cstdint>
#include <string>

int writeFileAsBytes(const std::string& filename, const std::vector<uint8_t>& buffer);