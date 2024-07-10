#include "../include.h"

namespace fs = std::filesystem;

class TempCleaner {
public:
    TempCleaner() = default;

    void AddDirectory(const std::wstring& directory) {
        if (fs::exists(directory) && fs::is_directory(directory)) {
            directoriesToClean.push_back(directory);
        }
        else {
            std::wcerr << L"Directory does not exist or is not a directory: " << directory << std::endl;
        }
    }

    void CleanAllDirectories() {
        for (const auto& dir : directoriesToClean) {
            CleanDirectory(dir);
        }
    }

    std::wstring GetTempDirectory() {
        wchar_t buffer[MAX_PATH];
        DWORD length = GetTempPath(MAX_PATH, buffer);
        if (length > 0 && length <= MAX_PATH) {
            return std::wstring(buffer);
        }
        throw std::runtime_error("Failed to get temp directory");
    }

    std::wstring GetUserProfilePath() {
        wchar_t userProfilePath[MAX_PATH];
        if (SHGetSpecialFolderPathW(NULL, userProfilePath, CSIDL_PROFILE, FALSE)) {
            return std::wstring(userProfilePath);
        }
        throw std::runtime_error("Failed to get user profile path");
    }

private:
    std::vector<std::wstring> directoriesToClean;

    void CleanDirectory(const std::wstring& directory) {
        std::wcout << L"Cleaning directory: " << directory << std::endl;
        try {
            for (const auto& entry : fs::directory_iterator(directory)) {
                TryRemoveEntry(entry);
            }
        }
        catch (const fs::filesystem_error& e) {
            std::wcerr << L"Error accessing path: " << directory << L" - " << e.what() << std::endl;
        }
    }

    void TryRemoveEntry(const fs::directory_entry& entry) {
        for (int attempt = 1; attempt <= 3; ++attempt) {
            try {
                fs::remove_all(entry);
                std::wcout << L"Deleted: " << entry.path().c_str() << std::endl;
                break;
            }
            catch (const fs::filesystem_error& e) {
                std::wcerr << L"Attempt " << attempt << L" to delete " << entry.path().c_str() << L" failed - " << e.what() << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100 * attempt)); // Задержка между попытками
            }
        }
    }
};
