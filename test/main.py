from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
import os

# Đường dẫn tới file cần mã hóa
input_file = r"D:\Downloads\apktool.zip"
output_file = r"D:\Downloads\apktool.zip.aes"

# Đọc dữ liệu nhị phân
with open(input_file, "rb") as f:
    data = f.read()

# Tạo hoặc định nghĩa khóa 32 byte (256-bit)
key = bytes.fromhex("a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3")
iv = bytes.fromhex("901f6fd130ff64bd093cb1b5be003ac7")
print(f"Key: {key.hex()}")
print(f"IV: {iv.hex()}")
# padded_data = pad(data, AES.block_size)  # block_size = 16

# Mã hóa bằng AES-256 ECB
cipher = AES.new(key, AES.MODE_CFB, iv, segment_size=128)
# ciphertext = cipher.encrypt(padded_data)
ciphertext = cipher.encrypt(data)


for i in ciphertext[:10]:
    print(hex(i), end=' ')
