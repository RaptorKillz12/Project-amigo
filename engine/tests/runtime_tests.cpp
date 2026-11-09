#include "amigo/runtime.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>

int main() {
    namespace fs = std::filesystem;
    const auto root = fs::temp_directory_path() / "project_amigo_runtime_test";
    std::error_code error;
    fs::remove_all(root, error);
    fs::create_directories(root / "nested");

    {
        std::ofstream file(root / "sample.dat", std::ios::binary);
        file << "amigo";
    }
    {
        std::ofstream file(root / "nested" / "chart.bin", std::ios::binary);
        file << "rhythm";
    }

    const auto report = amigo::Runtime::inspect_data_directory(root.string());
    assert(report.status == amigo::RuntimeStatus::ok);
    assert(report.files == 2);
    assert(report.directories == 1);
    assert(report.total_bytes == 11);

    const auto missing = amigo::Runtime::inspect_data_directory(
        (root / "does_not_exist").string());
    assert(missing.status == amigo::RuntimeStatus::directory_not_found);

    fs::remove_all(root, error);
    return 0;
}
