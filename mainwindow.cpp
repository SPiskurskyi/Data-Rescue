#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "fileData.h"

#include <QComboBox>
#include <QStorageInfo>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fillStorageComboBox();
    fillTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_searchButton_clicked()
{

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

void MainWindow::fillTable() {
    // Set header labels
    QStringList headerLabels;
    headerLabels << "Selected" << "Filename" << "Path" << "Size" << "Date";
    ui->tableWidget->setColumnCount(headerLabels.size());
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);

    std::vector<QFileData> fileData = generateRandomFileData(10);

    // Assuming you have a vector of data called 'fileData'
    for (const auto& item : fileData) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        QTableWidgetItem *selectItem = new QTableWidgetItem();
        selectItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled); // Make checkbox user checkable
        selectItem->setCheckState(Qt::Unchecked); // Unchecked by default

        QTableWidgetItem *filenameItem = new QTableWidgetItem(item.filename);
        QTableWidgetItem *pathItem = new QTableWidgetItem(item.path);
        QTableWidgetItem *sizeItem = new QTableWidgetItem(QString::number(item.size));
        QTableWidgetItem *dateItem = new QTableWidgetItem(item.date.toString());

        filenameItem->setFlags(filenameItem->flags() & ~Qt::ItemIsEditable);
        pathItem->setFlags(pathItem->flags() & ~Qt::ItemIsEditable);
        sizeItem->setFlags(sizeItem->flags() & ~Qt::ItemIsEditable);
        dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);

        ui->tableWidget->setItem(row, 0, selectItem);
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
