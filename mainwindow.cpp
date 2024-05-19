#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "fileData.h"
#include "binRecovery.h"

#include <QComboBox>
#include <QStorageInfo>
#include <QCheckBox>

#define HEADER_LABELS_SIZE 5

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fillStorageComboBox();
    fillTableHeaders();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_searchButton_clicked()
{
    std::vector<FileData> filesData;
    std::vector<QFileData> qfilesData;
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
    // std::vector<FileData> selectedFiles;
    // for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
    //     if (ui->tableWidget->item(row, 0)->checkState() == Qt::Checked) {
    //         FileData data;
    //         data.filename = ui->tableWidget->item(row, 1)->text();
    //         data.path = ui->tableWidget->item(row, 2)->text();
    //         data.size = ui->tableWidget->item(row, 3)->text().toLongLong();
    //         data.date = QDateTime::fromString(ui->tableWidget->item(row, 4)->text());
    //         selectedFiles.push_back(data);
    //     }
    // }
}


void MainWindow::on_infoButton_clicked()
{

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
