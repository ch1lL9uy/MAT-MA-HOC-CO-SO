#include "compressor.h"
#include <iostream>
#include <cstdlib>

Compressor::Compressor(const fs::path& input, const std::string& format, const std::string& level)
    : inputPath(input), archiveFormat(format), compressionLevel(level) {
    outputFile = getOutputName();
}

std::string Compressor::getOutputName() {
    if (fs::is_directory(inputPath))
        return inputPath.filename().string() + "." + archiveFormat;
    else
        return inputPath.stem().string() + "." + archiveFormat;
}

std::string Compressor::buildCompressCommand() const {
    std::string typeArg = "-t" + archiveFormat;
    std::string quotedOutput = "\"" + outputFile + "\"";
    std::string quotedInput = "\"" + inputPath.string() + "\"";
    return "7z a " + typeArg + " -mmt=on -ms=off " + quotedOutput + " " + quotedInput + " " + compressionLevel;
}

bool Compressor::compress() {
    if (!fs::exists(inputPath)) {
        std::cerr << "Path does not exist: " << inputPath << "\n";
        return false;
    }

    std::string cmd = buildCompressCommand();
    std::cout << "Executing: " << cmd << "\n";

    int result = system(cmd.c_str());
    if (result == 0) {
        std::cout << "Compression complete: " << outputFile << "\n";
        return true;
    } else {
        std::cerr << "Compression failed.\n";
        return false;
    }
}

bool Compressor::decompress(const fs::path& archivePath, const fs::path& destination) {
    if (!fs::exists(archivePath)) {
        std::cerr << "Archive not found: " << archivePath << "\n";
        return false;
    }

    std::string cmd = "7z x \"" + archivePath.string() + "\" -o\"" + destination.string() + "\" -y";
    std::cout << "⏳ Extracting: " << cmd << "\n";

    int result = system(cmd.c_str());
    if (result == 0) {
        std::cout << "Decompression complete.\n";
        return true;
    } else {
        std::cerr << "Decompression failed.\n";
        return false;
    }
}
