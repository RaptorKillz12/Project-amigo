#include "amigo/runtime.hpp"

#include <filesystem>
#include <system_error>

namespace amigo {
namespace fs = std::filesystem;

RuntimeReport Runtime::inspect_data_directory(const std::string& path) {
    RuntimeReport report;
    std::error_code error;
    const fs::path root(path);

    if (!fs::exists(root, error) || error || !fs::is_directory(root, error)) {
        report.status = RuntimeStatus::directory_not_found;
        report.message = "The selected data path does not exist or is not a directory.";
        return report;
    }

    fs::recursive_directory_iterator it(
        root,
        fs::directory_options::skip_permission_denied,
        error
    );
    const fs::recursive_directory_iterator end;

    if (error) {
        report.status = RuntimeStatus::inspection_failed;
        report.message = "Unable to begin scanning the selected directory.";
        return report;
    }

    for (; it != end; it.increment(error)) {
        if (error) {
            report.status = RuntimeStatus::inspection_failed;
            report.message = "A filesystem error occurred while scanning.";
            return report;
        }

        std::error_code item_error;
        if (it->is_directory(item_error)) {
            if (!item_error) {
                ++report.directories;
            }
            continue;
        }

        if (it->is_regular_file(item_error) && !item_error) {
            ++report.files;
            const auto bytes = it->file_size(item_error);
            if (!item_error) {
                report.total_bytes += bytes;
            }
        }
    }

    if (error) {
        report.status = RuntimeStatus::inspection_failed;
        report.message = "A filesystem error occurred while finishing the scan.";
        return report;
    }

    report.status = RuntimeStatus::ok;
    report.message = "Directory scan completed.";
    return report;
}

} // namespace amigo
