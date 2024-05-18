#include "fileData.h"

// Generate random file data
std::vector<QFileData> generateRandomFileData(int numFiles) {
    std::vector<QFileData> fileData;
    QStringList filenames = {"file1.txt", "file2.jpg", "file3.docx", "file4.pdf", "file5.png"};
    QStringList paths = {"/path/to/file1", "/path/to/file2", "/path/to/file3", "/path/to/file4", "/path/to/file5"};

    for (int i = 0; i < numFiles; ++i) {
        QFileData data;
        data.filename = filenames.at(rand() % filenames.size());
        data.path = paths.at(rand() % paths.size());
        data.size = rand() % 10000 + 100; // Random size between 100 and 10100 bytes
        data.date = QDateTime::currentDateTime().addDays(-(rand() % 365)); // Random date within the past year
        fileData.push_back(data);
    }

    return fileData;
}
