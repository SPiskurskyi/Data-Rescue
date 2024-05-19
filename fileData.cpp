#include "fileData.h"

std::vector<QFileData> convertToQFileData(const std::vector<FileData>& fileDataList) {
    std::vector<QFileData> qFileDataList;
    qFileDataList.reserve(fileDataList.size());

    for (const auto& fileData : fileDataList) {
        QFileData qFileData;
        qFileData.originalName = QString::fromStdWString(fileData.originalName);
        qFileData.internalName = fileData.internalName;
        qFileData.path = QString::fromStdWString(fileData.path);
        qFileData.size = fileData.size;
        qFileData.date = QString::fromStdWString(fileData.date);

        qFileDataList.push_back(std::move(qFileData));
    }

    return qFileDataList;
}

std::vector<FileData> convertToFileData(const std::vector<QFileData>& qFileDataList) {
    std::vector<FileData> fileDataList;
    fileDataList.reserve(qFileDataList.size());

    for (const auto& qFileData : qFileDataList) {
        FileData fileData;
        fileData.originalName = qFileData.originalName.toStdWString();
        fileData.internalName = qFileData.internalName;
        fileData.path = qFileData.path.toStdWString();
        fileData.size = qFileData.size;
        fileData.date = qFileData.date.toStdWString();

        fileDataList.push_back(std::move(fileData));
    }

    return fileDataList;
}
