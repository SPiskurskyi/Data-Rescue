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
    QString filename;
    QString path;
    qint64 size;
    QDateTime date;
};

class FileData {

public:
    std::string filename;
    std::string path;
    int64_t size;
    std::string date;
};


std::vector<QFileData> generateRandomFileData(int numFiles);

#endif // FILEDATA_H
