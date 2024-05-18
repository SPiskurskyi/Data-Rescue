#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_searchButton_clicked();

    void on_recoverButton_clicked();

    void on_infoButton_clicked();

    void fillStorageComboBox();

    void fillTable();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
