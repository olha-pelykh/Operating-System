#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <atomic>

int Priority[]={THREAD_PRIORITY_IDLE,THREAD_PRIORITY_LOWEST,THREAD_PRIORITY_BELOW_NORMAL,THREAD_PRIORITY_NORMAL,THREAD_PRIORITY_ABOVE_NORMAL,THREAD_PRIORITY_HIGHEST,THREAD_PRIORITY_TIME_CRITICAL};
struct Parameters{
    int start;
    int end;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    threadCount(0)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(TimerSlot()));
    timer->start(10);


    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);  // Створюємо подію, яку можна сигналізувати
    if (hEvent == NULL) {
        std::cerr << "Не вдалося створити подію!" << std::endl;
        return;
    }

}
QStringList text;
QStringList mytext;
HANDLE mutex;
CRITICAL_SECTION cs1;

std::atomic<bool> activeWaitSignal(false);

MainWindow::~MainWindow()
{
    if (hEvent != NULL) {
        CloseHandle(hEvent);  // Закриваємо подію
    }
    delete ui;
}

double f(double x) {
    return x * sin(x / 2.0);
}

double trapezoidalRule(Parameters* p) {
    double a = 0.0;
    double b = 100.0;
    int n = 10000;
    double h = (b - a) / n;
    double sum = f(a) + f(b);
    for (int i = 1; i < n; i++) {
        sum += 2 * f(a + i * h);
    }
    Sleep(100000);
    return (h / 2) * sum;
}


DWORD WINAPI ThreadFunctionNothing(LPVOID lpParam) {
    Parameters* p = static_cast<Parameters*>(lpParam);
    trapezoidalRule(p);
    return 0;
}


DWORD WINAPI ThreadFunctionActiveWaiting(LPVOID lpParam) {
    Parameters* p = static_cast<Parameters*>(lpParam);

    trapezoidalRule(p);

    // Починаємо активне очікування
    while (!activeWaitSignal) {
        // Активне очікування: перевіряємо змінну без блокування (тільки перевірка)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << "Потік завершив активне очікування!" << std::endl;
    return 0;
}

DWORD WINAPI ThreadFunctionEvents(LPVOID lpParam) {
    MainWindow* mainWindow = static_cast<MainWindow*>(lpParam);
    Parameters* p = static_cast<Parameters*>(lpParam);

    trapezoidalRule(p);

    // Чекання на подію
    DWORD waitResult = WaitForSingleObject(mainWindow->hEvent, INFINITE);

    // Після того, як подія сигналізована
    if (waitResult == WAIT_OBJECT_0) {
        std::cout << "Потік отримав сигнал і продовжує роботу!" << std::endl;
    }

    return 0;
}


void MainWindow::on_pushButton_Create_clicked()
{
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&startTime);
    int CountOfthread = ui->spinBox->value();
    int range = text.size()/CountOfthread;
    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (hEvent == NULL) {
        std::cerr << "Не вдалося створити подію!" << std::endl;
        return;
    }

    // Початкове значення лічильника потоків
    threadCount.store(0);

    for(int i=0 ;i<CountOfthread;i++){
        Parameters p;
        p.start=i*range;
        p.end =range + i*range;
        if(ui->radioButton_3->isChecked()){
            ui->textBrowser->append("Nothing");
            HANDLE th;
            th = CreateThread(NULL, 0, ThreadFunctionNothing, &p, 0, NULL);
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(QString::number(GetThreadId(th))));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem("running"));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem("Normal"));
            Threads.emplace(GetThreadId(th),th);
            qDebug()<<"start"+QString::number(p.start);
            qDebug()<<"end"+QString::number(p.end);
        }

        else if(ui->radioButton->isChecked()){
            ui->textBrowser->append("Active Waiting");
            HANDLE th;
            th = CreateThread(NULL, 0, ThreadFunctionActiveWaiting, &p, 0, NULL);

            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(QString::number(GetThreadId(th))));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem("running"));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem("Normal"));
            Threads.emplace(GetThreadId(th), th);

            qDebug() << "start" + QString::number(p.start);
            qDebug() << "end" + QString::number(p.end);
        }

        else if (ui->radioButton_2->isChecked()){
            ui->textBrowser->append("Events");

            // Створення потоку, який буде чекати на подію
            HANDLE th;
            th = CreateThread(NULL, 0, ThreadFunctionEvents, &p, 0, NULL);
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::number(GetThreadId(th))));
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem("running"));
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem("Normal"));
            Threads.emplace(GetThreadId(th), th);
            threadCount.fetch_add(1);  // Збільшуємо лічильник створених потоків

            qDebug() << "start" + QString::number(p.start);
            qDebug() << "end" + QString::number(p.end);
        }
    }

    if (threadCount.load() == CountOfthread) {
        SetEvent(hEvent);  // Сигналізуємо подію
        qDebug() << "Подія була активована автоматично!";
    }
}

void MainWindow::TimerSlot()
{
    DWORD exitCode;
    int index;
    for(auto it = Threads.begin();it!=Threads.end();){
        if(GetExitCodeThread(it->second,&exitCode)){
            if(exitCode == STILL_ACTIVE){
                it++;
            }
            else{

                for(int j=0;j<ui->tableWidget->rowCount();j++ ){
                    if(ui->tableWidget->item(j,0)->text().toInt() == it->first){
                        index = j;
                        break;
                    }
                }

                CloseHandle(it->second);
                it = Threads.erase(it);
                if(Threads.size() == 0){
                    for(int i = 0; i < mytext.size();i++ ){
                        ui->textBrowser->append(mytext[i]);
                    }
                    QueryPerformanceCounter(&endTime);
                    double Time = static_cast<double>(endTime.QuadPart - startTime.QuadPart) / frequency.QuadPart;
                    ui->label_2->setText("Час виконання процесів:"+QString::number(Time)+"s");
                    DeleteCriticalSection(&cs1);
                    CloseHandle(mutex);
                }
                ui->tableWidget->removeRow(index);
            }
        }
    }

}

void MainWindow::on_pushButton_Status_clicked()
{
    if(ui->pushButton_Status->text()== "Suspend"){
        SuspendThread(Threads[ui->tableWidget->item(currentindex,0)->text().toInt()]);
        ui->pushButton_Status->setText("Resume");
        ui->tableWidget->item(currentindex,1)->setText("suspended");
    }
    else{
        ResumeThread(Threads[ui->tableWidget->item(currentindex,0)->text().toInt()]);
        ui->pushButton_Status->setText("Suspend");
        ui->tableWidget->item(currentindex,1)->setText("running");
    }
}


void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    currentindex = row;
    if(ui->tableWidget->item(row,1)->text()=="running"){
        ui->pushButton_Status->setText("Suspend");
    }
    else{
        ui->pushButton_Status->setText("Resume");
    }
}


void MainWindow::on_pushButton_clicked()
{
    SetThreadPriority(Threads[ui->tableWidget->item(currentindex,0)->text().toInt()],Priority[ui->comboBox->currentIndex()]);
    ui->tableWidget->item(currentindex,2)->setText(ui->comboBox->currentText());
}


void MainWindow::on_pushButton_2_clicked()
{
    TerminateThread(Threads[ui->tableWidget->item(currentindex,0)->text().toInt()], 0);  // Небезпечне завершення потоку
    CloseHandle(Threads[ui->tableWidget->item(currentindex,0)->text().toInt()]);
    Threads.erase(ui->tableWidget->item(currentindex,0)->text().toInt());
    ui->tableWidget->removeRow(currentindex);

}

