#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <map>
#include <QStringList>
#include <windows.h>
#include <QFile>
#include <QTimer>
#include <iostream>


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
    HANDLE hEvent;
    std::atomic<int> threadCount;
    ~MainWindow();

private slots:
    void on_pushButton_Create_clicked();
    void TimerSlot();

    void on_pushButton_Status_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    LARGE_INTEGER frequency, startTime, endTime;
    Ui::MainWindow *ui;
    std::map<int,HANDLE> Threads;
    QTimer* timer;
    int currentindex;



};
#endif // MAINWINDOW_H
