#include <iostream>
#include <filesystem>
#include <string>
#include <windows.h>
#include <shellapi.h> 

namespace fs = std::filesystem;
using namespace std::chrono;

int main() {
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argc < 2) {
        std::cerr << "Usage: program <file_or_folder>\n";
        return 1;
    }
    
    std::wstring widePath = argv[1];
    fs::path inputPath = widePath;
    fs::path out = L"" + widePath;
    std::cout << inputPath.stem().string();
    std::cout << out.stem().string();

    return 0;
}