import hmac
import hashlib

def hmac_sha256(key: bytes, message: bytes) -> bytes:
    return hmac.new(key, message, hashlib.sha256).digest()

# Ví dụ sử dụng
if __name__ == "__main__":
    # Dữ liệu
    key = bytes.fromhex("a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3")
    message = open(r'D:\Downloads\out.aes', 'rb').read()[:-32]

    mac = hmac_sha256(key, message)

    print("HMAC (hex):", mac.hex())
    print("HMAC (raw bytes):", mac)
