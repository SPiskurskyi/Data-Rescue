#ifndef BINRECOVERY_H
#define BINRECOVERY_H

#include <vector>
#include "fileData.h"

void ListFilesInRecycleBin(std::vector<FileData>& fileList);
int RecoverFileFromRecycleBin(const std::wstring& filename);

#endif // BINRECOVERY_H
