#include "hmac.h"
#include "hash.h"
#include "hexToRaw.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

// HMAC(K,M)=H((K′⊕opad)∥H((K′⊕ipad)∥M)) với ∥ là phép nối chuỗi
std::string HMAC::compute(const std::string &key, const std::vector<uint8_t> &data) {
    const size_t block_size = 64;
    std::string k = key;
    std::string msg(data.begin(), data.end());
    SHA256 sha256;

    // Chuyển key từ chuỗi hex sang chuỗi nhị phân
    k = hexToRaw(k);

    // Đệm thêm các byte 0x0 vào để bằng độ dài với block 64bytes
    k.resize(block_size, 0x00);

    // Tạo khóa đệm ipad và opad
    std::string o_key_pad(block_size, 0x00);
    std::string i_key_pad(block_size, 0x00);
    for (size_t i = 0; i < block_size; ++i)
    {
        o_key_pad[i] = k[i] ^ 0x5c;
        i_key_pad[i] = k[i] ^ 0x36;
    }

    // Tính HMAC
    std::string inner_hash_hex = sha256.hash(i_key_pad + msg);
    std::string inner_hash_raw = hexToRaw(inner_hash_hex);

    return sha256.hash(o_key_pad + inner_hash_raw);
}
