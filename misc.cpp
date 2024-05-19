#include "windows.h"
#include "misc.h"

std::wstring getFileName(const std::wstring& filePath) {
    size_t pos = filePath.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        return filePath.substr(pos + 1);
    }
    return filePath; // In case there are no slashes, the entire path is the file name
}

void CalculateFolderSize(const std::wstring& folderPath, int64_t& folderSize) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((folderPath + L"\\*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
            std::wstring fullPath = folderPath + L"\\" + findFileData.cFileName;

            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                CalculateFolderSize(fullPath, folderSize);
            }
            else {
                folderSize += findFileData.nFileSizeLow + (static_cast<int64_t>(findFileData.nFileSizeHigh) << 32);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}
