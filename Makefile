# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -I include

# Targets
ENCRYPT_TARGET = Encrypt.exe
DECRYPT_TARGET = Decrypt.exe

# Common source files
COMMON_SRCS = \
	src/aes.cpp \
	src/hash.cpp \
	src/hmac.cpp \
	src/compressor.cpp \
	src/readFile.cpp \
	src/writeFile.cpp

# Encrypt-specific sources
ENCRYPT_SRCS = \
	src/encrypt.cpp \
	src/encryptionFormat.cpp \

# Decrypt-specific sources
DECRYPT_SRCS = \
	src/decrypt.cpp \
	src/decryptionFormat.cpp

# Default target
all: $(ENCRYPT_TARGET) $(DECRYPT_TARGET)

$(ENCRYPT_TARGET): $(COMMON_SRCS) $(ENCRYPT_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(DECRYPT_TARGET): $(COMMON_SRCS) $(DECRYPT_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(ENCRYPT_TARGET) $(DECRYPT_TARGET)
