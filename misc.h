#ifndef MISC_H
#define MISC_H

#include <string>

std::wstring getFileName(const std::wstring& filePath);
void CalculateFolderSize(const std::wstring& folderPath, int64_t& folderSize);

#endif // MISC_H
