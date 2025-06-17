#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <chrono>
#include "aes.h"
#include "hmac.h"
#include "hash.h"
#include "hexToRaw.h"

using namespace std;
using namespace std::chrono;

char modeToChar(AES::Mode mode) {
    switch (mode) {
        case AES::Mode::ECB: return 'E';
        case AES::Mode::CBC: return 'C';
        case AES::Mode::CFB: return 'F';
        case AES::Mode::OFB: return 'O';
        default:
            throw std::invalid_argument("Unknown AES mode");
    }
}

int main() {
    // 1. Sinh key từ SHA256("123") → lấy 16 byte đầu
    string k = "123";
    SHA256 sha256;
    auto enc_start = high_resolution_clock::now();
    // k = hexToRaw(sha256.hash(k));
    // vector<uint8_t> key(k.begin(), k.end()); 
    vector<uint8_t> key = sha256.hash(std::vector<uint8_t>(k.begin(), k.end()));

    // --- BẮT ĐẦU ĐO MÃ HÓA ---

    // 2. Tạo AES và IV
    AES aes(key, AES::Mode::CFB);
    vector<uint8_t> iv = aes.generateRandomIV();

    ostringstream key_hex;
    for (auto b : key) key_hex << hex << setw(2) << setfill('0') << (int)b;
    cout << "🔑 Key (hex): " << key_hex.str() << "\n";

    // In IV (hex)
    ostringstream iv_hex;
    for (auto b : iv) iv_hex << hex << setw(2) << setfill('0') << (int)b;
    cout << "🧂 IV (hex): " << iv_hex.str() << "\n";

    // 3. Đọc file đầu vào
    ifstream file(R"(D:\Downloads\ghidra.zip)", ios::binary);
    if (!file.is_open()) {
        cerr << "Failed to open input file." << endl;
        return 1;
    }

    file.seekg(0, ios::end);
    size_t filesize = file.tellg();
    file.seekg(0, ios::beg);
    vector<uint8_t> buffer(filesize);
    file.read(reinterpret_cast<char*>(&buffer[0]), filesize);
    file.close();

    // 4. Mã hóa
    vector<uint8_t> ciphertext;
    try {
        ciphertext = aes.encrypt(buffer, iv);
        cout << "✅ Mã hóa thành công\n";
    } catch (const exception& e) {
        cerr << "Encryption failed: " << e.what() << endl;
        return 1;
    }
    auto enc_end = high_resolution_clock::now();

    // 5. Ghi file mã hóa
    ofstream output(R"(D:\Downloads\out_ghidra.aes)", ios::binary);
    if (!output.is_open()) {
        cerr << "Failed to open output file." << endl;
        return 1;
    }

    cout << "Đang ghi vào file → out.aes\n";
    std::vector<uint8_t> finalOutput;

    // Thêm magic bytes
    std::string magic = "AES";  // 3 bytes
    finalOutput.insert(finalOutput.end(), magic.begin(), magic.end());

    // Thêm độ dài key
    uint8_t keyLenByte = static_cast<uint8_t>(key.size());  // 16 / 24 / 32
    finalOutput.push_back(keyLenByte);

    // Thêm mode
    finalOutput.push_back(static_cast<uint8_t>(modeToChar(aes.mode_)));

    // Thêm iv nếu như không phải là ECB
    if (aes.mode_ != AES::Mode::ECB) {
        finalOutput.insert(finalOutput.end(), iv.begin(), iv.end());  // 16 bytes
    }

    // Thêm ciphertext vào
    finalOutput.insert(finalOutput.end(), ciphertext.begin(), ciphertext.end());  // cipher

    // Tính HMAC toàn bộ phần trên và thêm vào cuối
    std::vector<uint8_t> hmac = HMAC::compute(key, finalOutput);
    // cout << "HMAC hex: " << hmac << "\n"; 
    finalOutput.insert(finalOutput.end(), hmac.begin(), hmac.end());  // append HMAC

    output.write(reinterpret_cast<const char*>(finalOutput.data()), finalOutput.size());
    output.close();

    // --- BẮT ĐẦU ĐO GIẢI MÃ ---
    auto dec_start = high_resolution_clock::now();

    // 6. Đọc lại file mã hóa để mô phỏng tình huống thực tế
    ifstream encFile(R"(D:\Downloads\out_ghidra.aes)", ios::binary);
    if (!encFile.is_open()) {
        cerr << "Failed to open encrypted file." << endl;
        return 1;
    }
    encFile.seekg(0, ios::end);
    size_t encSize = encFile.tellg();
    encFile.seekg(0, ios::beg);
    vector<uint8_t> cipherInput(encSize);
    encFile.read(reinterpret_cast<char*>(cipherInput.data()), encSize);
    encFile.close();

    // 7. Giải mã
    vector<uint8_t> decrypted;
    try {
        // decrypted = aes.decrypt(cipherInput, iv);
        decrypted = aes.decrypt(ciphertext, iv);
        cout << "✅ Giải mã thành công\n";
        cout << "🔍 10 byte đầu sau giải mã: ";
        for (size_t i = 0; i < 10 && i < decrypted.size(); ++i) {
            cout << hex << setw(2) << setfill('0') << (int)decrypted[i] << " ";
        }
        cout << "\n";
    } catch (const exception& e) {
        cerr << "Decryption failed: " << e.what() << endl;
        return 1;
    }

    auto dec_end = high_resolution_clock::now();

    // --- TÍNH TOÁN ---
    duration<double> enc_time = enc_end - enc_start;
    duration<double> dec_time = dec_end - dec_start;
    double mb = static_cast<double>(buffer.size()) / (1024 * 1024);

    cout << fixed << setprecision(3);
    cout << "🔐 Mã hóa: " << mb << " MB trong " << enc_time.count() << " giây → "
         << (mb / enc_time.count()) << " MB/s\n";

    cout << "🔓 Giải mã: " << mb << " MB trong " << dec_time.count() << " giây → "
         << (mb / dec_time.count()) << " MB/s\n";

    return 0;
}
