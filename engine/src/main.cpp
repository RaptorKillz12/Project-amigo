#include "amigo/runtime.hpp"

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc != 3 || std::string(argv[1]) != "--inspect-data") {
        std::cerr << "Usage: amigo_inspect --inspect-data <directory>\n";
        return 2;
    }

    const auto report = amigo::Runtime::inspect_data_directory(argv[2]);
    std::cout << "Project-amigo native runtime " << amigo::Runtime::version() << "\n";
    std::cout << report.message << "\n";

    if (report.status != amigo::RuntimeStatus::ok) {
        return 1;
    }

    std::cout << "Files: " << report.files << "\n"
              << "Directories: " << report.directories << "\n"
              << "Total bytes: " << report.total_bytes << "\n";
    return 0;
}
