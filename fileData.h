#ifndef FILEDATA_H
#define FILEDATA_H

#include <QMainWindow>
#include <QDateTime>

#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <iostream>
#include <vector>


#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")

class QFileData {

public:
    QString originalName; // Display name
    std::wstring internalName; // Name used for recovery
    QString path;
    qint64 size;
    QString date; // e.g., "04/16/2024 21:11"
};

class FileData {
public:
    std::wstring originalName; // Display name
    std::wstring internalName; // Name used for recovery
    std::wstring path;
    int64_t size = 0;
    std::wstring date;
};

std::vector<QFileData> convertToQFileData(const std::vector<FileData>& fileDataList);
std::vector<FileData> convertToFileData(const std::vector<QFileData>& qFileDataList);

#endif // FILEDATA_H
