#include <iostream>
#include <filesystem>
#include <fstream>
#include "aes.h"
#include "hash.h"
#include "hmac.h"
#include "compressor.h"
#include "readFile.h"
#include "writeFile.h"
#include "encryptionFormat.h"
#include <chrono>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: program <file_or_folder>\n";
        return 1;
    }

    fs::path inputPath(argv[1]);
    if (!fs::exists(inputPath)) {
        std::cerr << "Path does not exist: " << inputPath << "\n";
        return 1;
    }

    // Thực hiện nén (file hoặc folder)
    Compressor compressor(inputPath);
    compressor.compress();
    std::string zipFile = compressor.getOutputName();

    // Đọc dữ liệu từ file nén
    fs::path inputFile = inputPath.parent_path() / zipFile;
    std::vector<uint8_t> buffer = readFileAsBytes(inputFile.string());

    // Nhập và băm key
    std::cout << "\nEnter key: ";
    std::string k;
    std::getline(std::cin, k);
    SHA256 hash;
    std::vector<uint8_t> key = hash.hash(std::vector<uint8_t> (k.begin(), k.end()));
    
    // Mã hóa
    AES aes(key, AES::Mode::ECB);
    std::cout << "\nEncrypting........";
    std::vector<uint8_t> ciphertext = aes.encrypt(buffer);

    ciphertext = buildEncryptedFormat(ciphertext, key, {}, 0); // Tạo format cho file mã hóa

    // Ghi dữ liệu mã hóa vào file
    std::cout << "\nWriting to file......";
    std::string outputFile = inputFile.stem().string() + ".aes";
    writeFileAsBytes(outputFile, ciphertext);
    return 0;
}
