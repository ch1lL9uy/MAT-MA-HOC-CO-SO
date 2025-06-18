# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -I include -O3

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
	src/encryptionFormat.cpp

# Decrypt-specific sources
DECRYPT_SRCS = \
	src/decrypt.cpp \
	src/decryptionFormat.cpp

# Default target: build optimized versions
all: $(ENCRYPT_TARGET) $(DECRYPT_TARGET)

# Encrypt build
$(ENCRYPT_TARGET): $(COMMON_SRCS) $(ENCRYPT_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Decrypt build
$(DECRYPT_TARGET): $(COMMON_SRCS) $(DECRYPT_SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Debug mode (optional)
debug: CXXFLAGS := -std=c++17 -I include -O0 -g
debug: all

# Release mode
release: clean all

# Clean compiled binaries
clean:
	rm -f $(ENCRYPT_TARGET) $(DECRYPT_TARGET)
