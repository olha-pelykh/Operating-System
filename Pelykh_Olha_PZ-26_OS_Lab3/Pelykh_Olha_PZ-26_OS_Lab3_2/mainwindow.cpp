#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->priorityComboBox->addItem("Realtime", REALTIME_PRIORITY_CLASS);
    ui->priorityComboBox->addItem("High", HIGH_PRIORITY_CLASS);
    ui->priorityComboBox->addItem("Above Normal", ABOVE_NORMAL_PRIORITY_CLASS);
    ui->priorityComboBox->addItem("Normal", NORMAL_PRIORITY_CLASS);
    ui->priorityComboBox->addItem("Below Normal", BELOW_NORMAL_PRIORITY_CLASS);
    ui->priorityComboBox->addItem("Idle", IDLE_PRIORITY_CLASS);
    connect(ui->processComboBox, SIGNAL(activated(int)), this, SLOT(on_processComboBox_activated(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}



bool MainWindow::createNewProcess(const QString& executablePath, const QString& arguments, ProcessInfo& pInfo) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    QString command = QString("\"%1\" %2").arg(executablePath.trimmed(), arguments);
    std::wstring commandLine = command.toStdWString();

    qDebug() << "Command Line:" << QString::fromStdWString(commandLine);

    if (!CreateProcess(NULL, const_cast<wchar_t*>(commandLine.c_str()), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        DWORD errorCode = GetLastError();
        qDebug() << "Failed to create process. Error Code:" << errorCode;
        return false;
    }

    pInfo.processId = pi.dwProcessId;
    pInfo.processName = QFileInfo(executablePath).fileName();

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pi.dwProcessId);
    if (hProcess != NULL) {
        pInfo.priority = GetPriorityClass(hProcess);
        DWORD_PTR processAffinity;
        DWORD_PTR systemAffinity;
        if (GetProcessAffinityMask(hProcess, &processAffinity, &systemAffinity)) {
            pInfo.cpuAffinity = static_cast<DWORD>(processAffinity);
        } else {
            qDebug() << "Failed to get CPU affinity for process";
            pInfo.cpuAffinity = 0;
        }
        CloseHandle(hProcess);
    } else {
        qDebug() << "Failed to open process for query";
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

bool MainWindow::suspendProcess(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        qDebug() << "Failed to open process for suspend:" << processId;
        return false;
    }
    typedef LONG(NTAPI *NtSuspendProcess)(IN HANDLE ProcessHandle);
    NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(GetModuleHandle(L"ntdll"), "NtSuspendProcess");
    if (pfnNtSuspendProcess == NULL) {
        qDebug() << "Failed to get NtSuspendProcess function address";
        CloseHandle(hProcess);
        return false;
    }
    LONG status = pfnNtSuspendProcess(hProcess);
    CloseHandle(hProcess);
    return status == 0;
}

bool MainWindow::resumeProcess(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_SUSPEND_RESUME, FALSE, processId);
    if (hProcess == NULL) {
        qDebug() << "Failed to open process for resume:" << processId;
        return false;
    }
    typedef LONG(NTAPI *NtResumeProcess)(IN HANDLE ProcessHandle);
    NtResumeProcess pfnNtResumeProcess = (NtResumeProcess)GetProcAddress(GetModuleHandle(L"ntdll"), "NtResumeProcess");
    if (pfnNtResumeProcess == NULL) {
        qDebug() << "Failed to get NtResumeProcess function address";
        CloseHandle(hProcess);
        return false;
    }
    LONG status = pfnNtResumeProcess(hProcess);
    CloseHandle(hProcess);
    return status == 0;
}

bool MainWindow::setProcessPriority(DWORD processId, DWORD priority) {
    HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, processId);
    if (hProcess == NULL) {
        qDebug() << "Failed to open process for setting priority:" << processId;
        return false;
    }
    if (!SetPriorityClass(hProcess, priority)) {
        qDebug() << "Failed to set process priority:" << processId;
        CloseHandle(hProcess);
        return false;
    }
    CloseHandle(hProcess);
    return true;
}

bool MainWindow::setProcessAffinity(DWORD processId, DWORD cpuAffinity) {
    HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, processId);
    if (hProcess == NULL) {
        qDebug() << "Failed to open process for setting affinity:" << processId;
        return false;
    }
    if (!SetProcessAffinityMask(hProcess, cpuAffinity)) {
        qDebug() << "Failed to set process affinity:" << processId;
        CloseHandle(hProcess);
        return false;
    }
    CloseHandle(hProcess);
    return true;
}

bool MainWindow::terminateProcess(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
    if (hProcess == NULL) {
        qDebug() << "Failed to open process for termination:" << processId;
        return false;
    }
    if (!TerminateProcess(hProcess, 0)) {
        qDebug() << "Failed to terminate process:" << processId;
        CloseHandle(hProcess);
        return false;
    }
    CloseHandle(hProcess);
    return true;
}

QString MainWindow::getPriorityClassText(DWORD priority) {
    switch (priority) {
    case IDLE_PRIORITY_CLASS:
        return "Idle";
    case BELOW_NORMAL_PRIORITY_CLASS:
        return "Below Normal";
    case NORMAL_PRIORITY_CLASS:
        return "Normal";
    case ABOVE_NORMAL_PRIORITY_CLASS:
        return "Above Normal";
    case HIGH_PRIORITY_CLASS:
        return "High";
    case REALTIME_PRIORITY_CLASS:
        return "Realtime";
    default:
        return "Unknown";
    }
}

void MainWindow::populateProcessComboBox() {
    ui->processComboBox->clear();

    for (const ProcessInfo& processInfo : processes) {
        ui->processComboBox->addItem(processInfo.processName);
    }
}

void MainWindow::updateProcessLabel(const QString &selectedProcessName) {
    for (const ProcessInfo& processInfo : processes) {
        if (processInfo.processName == selectedProcessName) {
            ui->infoLabel->setText(
                QString("ID: %1, Priority: %2, State: %3, CPU Affinity: %4")
                    .arg(processInfo.processId)
                    .arg(processInfo.priority)
                    .arg(processInfo.state)
                    .arg(processInfo.cpuAffinity)
                );
            return;
        }
    }
    ui->infoLabel->setText("Process not found");
}

void MainWindow::updateProcessState(DWORD processId, ProcessInfo::State newState) {
    for (ProcessInfo& processInfo : processes) {
        if (processInfo.processId == processId) {
            processInfo.state = newState;
            qDebug() << "Updated process" << processInfo.processName << "to state:" << processInfo.state;
            return;
        }
    }
    qDebug() << "Process with ID:" << processId << "not found.";
}

void MainWindow::on_StartProcessClicked_clicked() {
    QString executablePath = ui->PathEdit->text();
    QString arguments = "";
    ProcessInfo processInfo;

    if (createNewProcess(executablePath, arguments, processInfo)) {
        qDebug() << "Process started successfully!";
        qDebug() << "Process ID:" << processInfo.processId;
        qDebug() << "Process Name:" << processInfo.processName;
        qDebug() << "Priority:" << processInfo.priority;

        processes.append(processInfo);

        populateProcessComboBox();

    } else {
        qDebug() << "Failed to start process.";
    }
}

void MainWindow::on_SuspendProcess_clicked() {
    if (processes.isEmpty()) {
        qDebug() << "No processes available to suspend.";
        return;
    }

    QString selectedProcessName = ui->processComboBox->currentText();
    for (const ProcessInfo& processInfo : processes) {
        if (processInfo.processName == selectedProcessName) {
            DWORD processId = processInfo.processId;
            if (suspendProcess(processId)) {
                qDebug() << "Process with ID:" << processId << "suspended successfully!";
                updateProcessState(processId, ProcessInfo::Suspended);
            } else {
                qDebug() << "Failed to suspend process with ID:" << processId;
            }
            return;
        }
    }
    qDebug() << "Process not found in the list.";
}

void MainWindow::on_ResumeProcess_clicked() {
    if (processes.isEmpty()) {
        qDebug() << "No process to resume.";
        return;
    }

    QString selectedProcessName = ui->processComboBox->currentText();
    for (const ProcessInfo& processInfo : processes) {
        if (processInfo.processName == selectedProcessName) {
            DWORD processId = processInfo.processId;
            if (resumeProcess(processId)) {
                qDebug() << "Process resumed successfully:" << processInfo.processName;
                updateProcessState(processId, ProcessInfo::Running);
            } else {
                qDebug() << "Failed to resume process:" << processId;
            }
            return;
        }
    }
    qDebug() << "Process not found in the list.";
}

void MainWindow::on_TerminateProcess_clicked() {
    if (processes.isEmpty()) {
        qDebug() << "No processes available to terminate.";
        return;
    }

    QString selectedProcessName = ui->processComboBox->currentText();

    for (int i = 0; i < processes.size(); ++i) {
        if (processes[i].processName == selectedProcessName) {
            DWORD processId = processes[i].processId;

            if (terminateProcess(processId)) {
                qDebug() << "Process with ID:" << processId << "terminated successfully!";
                processes.removeAt(i);
                updateProcessState(processId, ProcessInfo::Terminated);
                populateProcessComboBox();
            } else {
                qDebug() << "Failed to terminate process with ID:" << processId;
            }
            return;
        }
    }

    qDebug() << "Process not found in the list.";
}

void MainWindow::on_SetAffinity_clicked() {
    if (processes.isEmpty()) {
        qDebug() << "No processes available to set affinity.";
        return;
    }

    QString selectedProcessName = ui->processComboBox->currentText();
    DWORD processId = 0;

    for (const ProcessInfo& processInfo : processes) {
        if (processInfo.processName == selectedProcessName) {
            processId = processInfo.processId;
            break;
        }
    }

    if (processId == 0) {
        qDebug() << "Selected process not found.";
        return;
    }

    DWORD_PTR cpuAffinity = 0;

    for (int i = 0; i < 16; ++i) {
        QCheckBox* checkBox = findChild<QCheckBox*>(QString("cpuCheckBox%1").arg(i));
        if (checkBox && checkBox->isChecked()) {
            cpuAffinity |= (1 << i);
        }
    }

    if (setProcessAffinity(processId, cpuAffinity)) {
        qDebug() << "Affinity set successfully for process ID:" << processId;
    } else {
        qDebug() << "Failed to set affinity for process ID:" << processId;
    }
}

void MainWindow::on_SetPriority_clicked() {
    if (processes.isEmpty()) {
        qDebug() << "No processes available to set priority.";
        return;
    }

    QString selectedProcessName = ui->processComboBox->currentText();

    for (const ProcessInfo& processInfo : processes) {
        if (processInfo.processName == selectedProcessName) {
            DWORD processId = processInfo.processId;

            DWORD priority = static_cast<DWORD>(ui->priorityComboBox->currentData().toUInt());

            if (setProcessPriority(processId, priority)) {
                qDebug() << "Priority for process ID:" << processId << "set successfully!";
            } else {
                qDebug() << "Failed to set priority for process ID:" << processId;
            }
            return;
        }
    }

    qDebug() << "Process not found in the list.";
}

void MainWindow::on_processComboBox_activated(int index) {
    if (index >= 0 && index < processes.size()) {
        const ProcessInfo& selectedProcess = processes[index];
        ui->infoLabel->setText(
            QString(" ID: %1\n Priority: %2\n State: %3\n")
                .arg(selectedProcess.processId)
                .arg(selectedProcess.priority)
                .arg(selectedProcess.state)
            );
    } else {
        ui->infoLabel->setText("Invalid selection");
    }
}


