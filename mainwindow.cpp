#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "fileData.h"
#include "binRecovery.h"

#include <QComboBox>
#include <QStorageInfo>
#include <QCheckBox>

#include <QMessageBox>

#define HEADER_LABELS_SIZE 5

std::vector<QFileData> qfilesData;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fillStorageComboBox();
    fillTableHeaders();

    QString infoMessage;
    infoMessage += "----------------------------------------------------------\n";
    infoMessage += "Attention this is very raw version of this project\n";
    infoMessage += "for now it proccesses and recoveres all files in bin\n";
    infoMessage += "in future the functional of program will be extended\n";
    infoMessage += "----------------------------------------------------------\n";
    QMessageBox::information(this, "INFO", infoMessage);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_searchButton_clicked()
{
    std::vector<FileData> filesData;
    bool binRecovery = 1;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(HEADER_LABELS_SIZE);

    if(binRecovery)
        ListFilesInRecycleBin(filesData);

    qfilesData = convertToQFileData(filesData);
    fillTableItems(qfilesData);
}


void MainWindow::on_recoverButton_clicked()
{
    bool binRecovery = true;
    bool selected = false;
    int ret;
    QString infoMessage;
    int rowCount = ui->tableWidget->rowCount();
    int dataSize = static_cast<int>(qfilesData.size());

    infoMessage += "-----------------------------\n";
    for (int row = 0; row < rowCount; ++row) {
        // Get the QWidget from the first column, which contains the QCheckBox
        QWidget *widget = ui->tableWidget->cellWidget(row, 0);
        if (widget) {
            QCheckBox *checkBox = widget->findChild<QCheckBox *>();
            if (checkBox && checkBox->isChecked()) {
                selected = true;
                if (row < dataSize) {
                    const QFileData& fileData = qfilesData[row];
                    infoMessage += fileData.originalName + "\n";
                    if (binRecovery) {
                        ret = RecoverFileFromRecycleBin(qfilesData[row].internalName);
                        if(ret != 0) {
                            QMessageBox::information(this, "Error", "Something gone wrong. ");
                            return;
                        }
                    }
                } else {
                    infoMessage += "Error: Row index out of bounds for qfilesData\n";
                }
            }
        }
    }
    infoMessage += "-----------------------------\n";
    infoMessage += "Following files will was recovered successfully! \n";
    if (selected) {
        QMessageBox::information(this, "Selected File Information", infoMessage);
    } else {
        QMessageBox::information(this, "Selected File Information", "No items selected. Please select some items. ");
    }
}


void MainWindow::on_infoButton_clicked()
{
    QString infoMessage;
    infoMessage += "-----------------------------\n";
    infoMessage += "Data-rescue\n";
    //infoMessage += "Here must be info\n";
    infoMessage += "(c) Serhii Piskurskyi\n";
    infoMessage += "-----------------------------\n";
    QMessageBox::information(this, "Info", infoMessage);
}

void MainWindow::fillTableHeaders() {
    // Set header labels
    QStringList headerLabels;
    headerLabels << "Selected" << "Filename" << "Path" << "Size" << "Date";
    ui->tableWidget->setColumnCount(headerLabels.size());
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget->setColumnWidth(0, 75);   // Selected
    ui->tableWidget->setColumnWidth(1, 175);  // Filename
    ui->tableWidget->setColumnWidth(2, 475);  // Path
    ui->tableWidget->setColumnWidth(3, 75);   // Size
    ui->tableWidget->setColumnWidth(4, 100);   // Date
}

void MainWindow::fillTableItems(std::vector<QFileData> &filesData) {
    // Assuming you have a vector of data called 'fileData'
    for (const auto& item : filesData) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        // Create a QWidget to hold the QCheckBox
        QWidget *checkboxWidget = new QWidget();
        QCheckBox *checkBox = new QCheckBox();
        QHBoxLayout *layout = new QHBoxLayout(checkboxWidget);

        // Center the checkbox within the widget
        layout->addWidget(checkBox);
        layout->setAlignment(Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0);

        // Set the checkbox state to unchecked
        checkBox->setChecked(false);

        QTableWidgetItem *filenameItem = new QTableWidgetItem(item.originalName);
        QTableWidgetItem *pathItem = new QTableWidgetItem(item.path);
        QTableWidgetItem *sizeItem = new QTableWidgetItem(QString::number(item.size));
        QTableWidgetItem *dateItem = new QTableWidgetItem(item.date);

        filenameItem->setFlags(filenameItem->flags() & ~Qt::ItemIsEditable);
        pathItem->setFlags(pathItem->flags() & ~Qt::ItemIsEditable);
        sizeItem->setFlags(sizeItem->flags() & ~Qt::ItemIsEditable);
        dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);

        ui->tableWidget->setCellWidget(row, 0, checkboxWidget);
        ui->tableWidget->setItem(row, 1, filenameItem);
        ui->tableWidget->setItem(row, 2, pathItem);
        ui->tableWidget->setItem(row, 3, sizeItem);
        ui->tableWidget->setItem(row, 4, dateItem);
    }
}

void MainWindow::fillStorageComboBox() {
    ui->pickedDir->clear();
    QList<QStorageInfo> drives = QStorageInfo::mountedVolumes();

    foreach (const QStorageInfo &storage, drives) {
        if (storage.isValid() && storage.isReady() && !storage.isReadOnly()) {
            ui->pickedDir->addItem(storage.rootPath());
        }
    }
}
