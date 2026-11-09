#pragma once

#include <cstddef>
#include <string>

namespace amigo {

enum class RuntimeStatus {
    ok,
    directory_not_found,
    inspection_failed
};

struct RuntimeReport {
    RuntimeStatus status{RuntimeStatus::ok};
    std::size_t files{0};
    std::size_t directories{0};
    std::uintmax_t total_bytes{0};
    std::string message;
};

class Runtime {
public:
    static constexpr const char* version() noexcept {
        return "0.1.0";
    }

    // Inspects a user-selected data directory. This does not decrypt,
    // execute, or interpret proprietary game content.
    static RuntimeReport inspect_data_directory(const std::string& path);
};

} // namespace amigo
