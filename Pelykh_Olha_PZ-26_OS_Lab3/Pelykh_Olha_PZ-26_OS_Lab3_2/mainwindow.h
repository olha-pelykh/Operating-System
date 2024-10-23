#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QProcess>
#include <QDebug>
#include <windows.h>
#include <QString>
#include <QFileInfo>
#include <QCheckBox>
#include <Windows.h>
#include <TlHelp32.h>

struct ProcessInfo {
    DWORD processId;
    QString processName;
    DWORD priority;
    DWORD cpuAffinity;

    enum State {
        Running = 0,
        Suspended = 1,
        Terminated = 2,
        Unknown = 3
    };

    State state;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_StartProcessClicked_clicked();

    void on_SuspendProcess_clicked();

    void on_ResumeProcess_clicked();

    void on_TerminateProcess_clicked();

    void on_SetAffinity_clicked();

    void on_SetPriority_clicked();

    void on_processComboBox_activated(int index);

private:
    Ui::MainWindow *ui;

     QVector<ProcessInfo> processes;

    bool createNewProcess(const QString& executablePath, const QString& arguments, ProcessInfo& pInfo);
    bool suspendProcess(DWORD processId);
    bool resumeProcess(DWORD processId);
    bool setProcessPriority(DWORD processId, DWORD priority);
    bool setProcessAffinity(DWORD processId, DWORD cpuAffinity);
    bool terminateProcess(DWORD processId);
    QString getPriorityClassText(DWORD priority);
    void populateProcessComboBox();
    void updateProcessLabel(const QString &selectedProcessName);
    void updateProcessState(DWORD processId, ProcessInfo::State newState);
};

#endif
