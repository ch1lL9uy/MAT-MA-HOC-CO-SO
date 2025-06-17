#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <random>

namespace crypto {

class AES {
public:
    size_t Nk = 8;
    size_t Nb = 4;
    size_t Nr = 14;
    enum class KeyLength { AES_128 = 16, AES_192 = 24, AES_256 = 32 };
    enum class Mode { ECB, CBC, CFB, OFB };

    AES(const std::vector<uint8_t>& key, Mode mode, KeyLength keyLength);
    
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext, 
                               const std::vector<uint8_t>& iv = {});
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext,
                               const std::vector<uint8_t>& iv = {});

private:
    Mode mode_;
    KeyLength keyLength_;
    size_t rounds_;
    std::vector<uint32_t> roundKeys_;

    // Key expansion
    void keyExpansion(const std::vector<uint8_t>& key);
    uint32_t subWord(uint32_t word);
    uint32_t rotWord(uint32_t word);
    // uint8_t xtime(uint8_t x);
    // uint8_t multiply(uint8_t a, uint8_t b);

    // Block processing
    void encryptBlock(uint8_t state[16]);
    void decryptBlock(uint8_t state[16]);

    // AES transformations
    void subBytes(uint8_t state[16]);
    void shiftRows(uint8_t state[16]);
    void mixColumns(uint8_t state[16]);
    void addRoundKey(uint8_t state[16], size_t round);

    // Inverse transformations
    void invSubBytes(uint8_t state[16]);
    void invShiftRows(uint8_t state[16]);
    void invMixColumns(uint8_t state[16]);

    // Mode implementations
    std::vector<uint8_t> generateRandomIV();
    void processECB(const uint8_t* input, uint8_t* output, size_t blocks, bool encrypt);
    void processCBC(const uint8_t* input, uint8_t* output, size_t blocks, 
                   const uint8_t iv[16], bool encrypt);
    void processCFB(const uint8_t* input, uint8_t* output, size_t blocks,
                   const uint8_t iv[16], bool encrypt);
    void processOFB(const uint8_t* input, uint8_t* output, size_t blocks,
                   const uint8_t iv[16]);

    // Helpers
    void validateKeyIV(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv) const;
    void xorBlock(uint8_t* target, const uint8_t* source);
    void padData(std::vector<uint8_t>& data) const;
    void unpadData(std::vector<uint8_t>& data) const;

    // Lookup tables
    static const uint8_t sbox[256];
    static const uint8_t inv_sbox[256];
    static const uint8_t rcon[11];
    static const uint8_t galoisMul2[256];
    static const uint8_t galoisMul3[256];
    static const uint8_t galoisMul9[256];
    static const uint8_t galoisMul11[256];
    static const uint8_t galoisMul13[256];
    static const uint8_t galoisMul14[256];
};

} // namespace crypto
