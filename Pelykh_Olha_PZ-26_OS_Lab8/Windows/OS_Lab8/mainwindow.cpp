#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <iostream>

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

HANDLE MainWindow::hFile = INVALID_HANDLE_VALUE;

MainWindow::~MainWindow()
{
    delete ui;
}

void RestrictFileAccessToAdministrators(const QString &fileNamee);

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
        logProcessEvent("Created", pInfo.processId);
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
    logProcessEvent("Suspended", processId);
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
    logProcessEvent("Resumed", processId);
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
    QString PriProc = "Cnanged process to " + getPriorityClassText(priority);
    logProcessEvent(PriProc, processId);
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
    logProcessEvent("Terminated", processId);
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

void MainWindow::logProcessEvent(const QString& action, DWORD pid) {

    static QString filename = "C:\\Users\\User\\Music\\OS\\Pelykh_Olha_PZ-26_OS_Lab3_2\\fileChangesInfo.txt";

    // Ensure the file handle is already opened and accessible
    if (!hFile || hFile == INVALID_HANDLE_VALUE) {
        qDebug() << "File i s not open. Cannot log the event.";
        return;
    }

    // Check for write permissions
    DWORD attributes = GetFileAttributesA(filename.toStdString().c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES || (attributes & FILE_ATTRIBUTE_READONLY)) {
        qDebug() << "File is read-only. Cannot write log entries.";
        return;
    }

    // Move the file pointer to the end of the file
    SetFilePointer(hFile, 0, NULL, FILE_END);

    // Prepare the log entry
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString logEntry = QString("PID: %1\tAction: %2\tTime: %3\n")
                           .arg(pid)
                           .arg(action)
                           .arg(timestamp);

    // Write the log entry to the file
    DWORD written;
    BOOL writeResult = WriteFile(hFile, logEntry.toUtf8().data(), logEntry.toUtf8().size(), &written, NULL);

    if (!writeResult || written != logEntry.toUtf8().size()) {
        qDebug() << "Failed to write log entry to the file.";
    } else {
        qDebug() << "Log entry written successfully:" << logEntry;
    }
}

void MainWindow::on_fileButton_clicked()
{
    QString selectedOption = ui->comboBox->currentText();
    QString fileName = "C:\\Users\\User\\Music\\OS\\Pelykh_Olha_PZ-26_OS_Lab3_2\\fileChangesInfo.txt";
    if (selectedOption == "Create new file") {
        QString fileName = "C:\\Users\\User\\Music\\OS\\Pelykh_Olha_PZ-26_OS_Lab3_2\\fileChangesInfo.txt";
        hFile = CreateFileA(
            fileName.toStdString().c_str(),  // File name
            GENERIC_READ | GENERIC_WRITE,   // Desired access
            0,                              // Share mode
            nullptr,                        // Security attributes
            CREATE_ALWAYS,                  // Creation disposition
            FILE_ATTRIBUTE_NORMAL,          // Flags and attributes
            nullptr                         // Template file
            );

        if (hFile == INVALID_HANDLE_VALUE) {
            DWORD errorCode = GetLastError();
            QMessageBox::critical(this, "Error",
                                  QString("Failed to create or open the file! Error Code: %1").arg(errorCode));
        } else {
            QMessageBox::information(this, "Success", "File created/opened successfully!");
        }
    }
    else if (selectedOption == "Set only read permission") {
        if (!hFile || hFile == INVALID_HANDLE_VALUE) {
            QMessageBox::warning(this, "Error", "No file is open. Please create/open a file first.");
        } else {
            QString fileName = "C:\\Users\\User\\Music\\OS\\Pelykh_Olha_PZ-26_OS_Lab3_2\\fileChangesInfo.txt";
            if (!SetFileAttributesA(fileName.toStdString().c_str(), FILE_ATTRIBUTE_READONLY)) {
                DWORD errorCode = GetLastError();
                QMessageBox::critical(this, "Error",
                                      QString("Failed to set read-only permissions! Error Code: %1").arg(errorCode));
            } else {
                QMessageBox::information(this, "Success", "Read-only permissions set.");
            }
        }
    }
    else if (selectedOption == "Set write permission") {
        if (!hFile || hFile == INVALID_HANDLE_VALUE) {
            QMessageBox::warning(this, "Error", "No file is open. Please create/open a file first.");
        } else {
            QString fileName = "C:\\Users\\User\\Music\\OS\\Pelykh_Olha_PZ-26_OS_Lab3_2\\fileChangesInfo.txt";
            if (!SetFileAttributesA(fileName.toStdString().c_str(), FILE_ATTRIBUTE_NORMAL)) {
                DWORD errorCode = GetLastError();
                QMessageBox::critical(this, "Error",
                                      QString("Failed to set write permissions! Error Code: %1").arg(errorCode));
            } else {
                QMessageBox::information(this, "Success", "Write permissions set.");
            }
        }
    }
    else if (selectedOption == "Set SID Normal") {
        // Зміна прав доступу для Normal користувача
        SetFileAccessControl(fileName, "S-1-5-32-544");  // SID для звичайного користувача
    }
    else if (selectedOption == "Set SID Administrator") {
        // Зміна прав доступу для Administrator
        SetFileAccessControl(fileName, "S-1-5-32-544");  // SID для адміністратора
    }
    else if (selectedOption == "Set SID Group") {
        // Зміна прав доступу для Group
        SetFileAccessControl(fileName, "S-1-5-32-555");  // SID для групи
    }
}


void MainWindow::SetFileAccessControl(const QString &fileName, const QString &sid) {
    // Convert QString to LPCWSTR (wide string)
    LPCWSTR lpFileName = (LPCWSTR)fileName.toStdWString().c_str();

    // SID conversion
    PSID pSid = NULL;
    if (!ConvertStringSidToSidW(sid.toStdWString().c_str(), &pSid)) {
        DWORD dwError = GetLastError();
        QMessageBox::warning(this, "Error", QString("Failed to convert SID. Error code: %1").arg(dwError));
        return;
    }

    // Initialize security information structures
    PSECURITY_DESCRIPTOR pSD = NULL;
    PACL pDACL = NULL;
    PACL pSACL = NULL;
    PSID pOwner = NULL;
    PSID pGroup = NULL;

    // Retrieve the security information for the file
    DWORD dwRes = GetNamedSecurityInfoW(lpFileName, SE_FILE_OBJECT,
                                        OWNER_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION,
                                        &pOwner, &pGroup, &pDACL, &pSACL, &pSD);

    if (dwRes != ERROR_SUCCESS) {
        QMessageBox::warning(this, "Error", QString("Failed to get file security info. Error code: %1").arg(dwRes));
        return;
    }

    // Prepare the DACL to update access control
    ACL_SIZE_INFORMATION aclSizeInfo;
    GetAclInformation(pDACL, &aclSizeInfo, sizeof(aclSizeInfo), AclSizeInformation);

    EXPLICIT_ACCESSW ea;
    ZeroMemory(&ea, sizeof(EXPLICIT_ACCESSW));

    // Setting the permissions for the SID, e.g., full access
    ea.grfAccessPermissions = GENERIC_READ | GENERIC_WRITE;
    ea.grfAccessMode = GRANT_ACCESS;
    ea.grfInheritance = NO_INHERITANCE;
    ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea.Trustee.ptstrName = (LPWSTR)pSid;

    PACL pNewDACL = NULL;
    dwRes = SetEntriesInAclW(1, &ea, pDACL, &pNewDACL);
    if (dwRes != ERROR_SUCCESS) {
        QMessageBox::warning(this, "Error", QString("Failed to update ACL. Error code: %1").arg(dwRes));
        return;
    }

    // Set the new DACL to the file
    dwRes = SetNamedSecurityInfoW((LPWSTR)lpFileName, SE_FILE_OBJECT,
                                  DACL_SECURITY_INFORMATION, NULL, NULL, pNewDACL, NULL);

    if (dwRes != ERROR_SUCCESS) {
        QMessageBox::warning(this, "Error", QString("Failed to set security info. Error code: %1").arg(dwRes));
    } else {
        QMessageBox::information(this, "Success", "File access control updated successfully.");
    }

    // Clean up
    if (pSid) {
        LocalFree(pSid);
    }
    if (pSD) {
        LocalFree(pSD);
    }
    if (pNewDACL) {
        LocalFree(pNewDACL);
    }
}

