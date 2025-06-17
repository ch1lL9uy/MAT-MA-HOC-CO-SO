#include "readFile.h"
#include <iostream>
#include <fstream>
#include <vector>

std::vector<uint8_t> readFileAsBytes(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Khong the mo file: " << filename << std::endl;
        return {};
    }
    file.seekg(0, std::ios::end);
    size_t filesize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(filesize);
    file.read(reinterpret_cast<char*>(&buffer[0]), filesize);
    return buffer;
}