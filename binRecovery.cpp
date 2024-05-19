#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <iostream>
#include <vector>

#include "binRecovery.h"
#include "misc.h"

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")

void ListFilesInRecycleBin(std::vector<FileData>& fileList) {
    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize COM library." << std::endl;
        return;
    }

    LPITEMIDLIST pidlRecycleBin = nullptr;
    hr = SHGetSpecialFolderLocation(nullptr, CSIDL_BITBUCKET, &pidlRecycleBin);
    if (FAILED(hr)) {
        std::cerr << "Failed to get Recycle Bin location." << std::endl;
        CoUninitialize();
        return;
    }

    IShellFolder* pDesktopFolder = nullptr;
    hr = SHGetDesktopFolder(&pDesktopFolder);
    if (FAILED(hr)) {
        std::cerr << "Failed to get Desktop folder." << std::endl;
        CoTaskMemFree(pidlRecycleBin);
        CoUninitialize();
        return;
    }

    IShellFolder* pRecycleBinFolder = nullptr;
    hr = pDesktopFolder->BindToObject(pidlRecycleBin, nullptr, IID_IShellFolder, (void**)&pRecycleBinFolder);
    if (FAILED(hr)) {
        std::cerr << "Failed to bind to Recycle Bin folder." << std::endl;
        pDesktopFolder->Release();
        CoTaskMemFree(pidlRecycleBin);
        CoUninitialize();
        return;
    }

    IEnumIDList* pEnumIDList = nullptr;
    hr = pRecycleBinFolder->EnumObjects(nullptr, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &pEnumIDList);
    if (FAILED(hr)) {
        std::cerr << "Failed to enumerate Recycle Bin items." << std::endl;
        pRecycleBinFolder->Release();
        pDesktopFolder->Release();
        CoTaskMemFree(pidlRecycleBin);
        CoUninitialize();
        return;
    }

    LPITEMIDLIST pidlItem = nullptr;
    while (pEnumIDList->Next(1, &pidlItem, nullptr) == S_OK) {
        STRRET str;
        hr = pRecycleBinFolder->GetDisplayNameOf(pidlItem, SHGDN_FORPARSING, &str);
        if (SUCCEEDED(hr)) {
            wchar_t szInternalName[MAX_PATH];
            StrRetToBufW(&str, pidlItem, szInternalName, ARRAYSIZE(szInternalName));

            // Get the original display name
            hr = pRecycleBinFolder->GetDisplayNameOf(pidlItem, SHGDN_NORMAL, &str);
            if (SUCCEEDED(hr)) {
                wchar_t szOriginalName[MAX_PATH];
                StrRetToBufW(&str, pidlItem, szOriginalName, ARRAYSIZE(szOriginalName));

                // Retrieve file information
                WIN32_FIND_DATA findFileData;
                HANDLE hFind = FindFirstFile(szInternalName, &findFileData);
                if (hFind != INVALID_HANDLE_VALUE) {
                    FileData fileData;
                    fileData.internalName = szInternalName;
                    fileData.path = szOriginalName;
                    fileData.originalName = getFileName(szOriginalName);

                    if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        // Calculate folder size
                        int64_t folderSize = 0;
                        CalculateFolderSize(szInternalName, folderSize);
                        fileData.size = folderSize;
                    }
                    else {
                        fileData.size = findFileData.nFileSizeLow + (static_cast<int64_t>(findFileData.nFileSizeHigh) << 32);
                    }

                    // Get the file's deletion date
                    FILETIME ftWrite = findFileData.ftLastWriteTime;
                    SYSTEMTIME stUTC, stLocal;
                    FileTimeToSystemTime(&ftWrite, &stUTC);
                    SystemTimeToTzSpecificLocalTime(nullptr, &stUTC, &stLocal);
                    wchar_t dateBuf[128];
                    swprintf(dateBuf, ARRAYSIZE(dateBuf), L"%02d/%02d/%d %02d:%02d", stLocal.wMonth, stLocal.wDay, stLocal.wYear, stLocal.wHour, stLocal.wMinute);
                    fileData.date = dateBuf;

                    fileList.push_back(fileData);

                    FindClose(hFind);
                }
            }
        }

        CoTaskMemFree(pidlItem);
    }

    pEnumIDList->Release();
    pRecycleBinFolder->Release();
    pDesktopFolder->Release();
    CoTaskMemFree(pidlRecycleBin);
    CoUninitialize();
}
