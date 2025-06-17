#include "writeFile.h"
#include <iostream>
#include <fstream>
#include <vector>

// ghi dữ liệu mã hóa vào file
int writeFileAsBytes(const std::string& filename, const std::vector<uint8_t>& buffer) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Khong the mo file: " << filename << std::endl;
        return 0;
    }
    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    return 1;
}