#include "../utils/utils.h"

int main() {
    try {
        TempCleaner cleaner;

        cleaner.AddDirectory(cleaner.GetTempDirectory());
        cleaner.AddDirectory(cleaner.GetUserProfilePath() + L"\\AppData\\Local\\Temp");
        cleaner.AddDirectory(L"C:\\Windows\\SoftwareDistribution\\Download");
        cleaner.AddDirectory(L"C:\\Windows\\Temp");
        cleaner.AddDirectory(L"C:\\Windows\\WinSxS\\Temp");

        cleaner.CleanAllDirectories();

    }
    catch (const std::exception& e) {
        std::wcerr << L"An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
