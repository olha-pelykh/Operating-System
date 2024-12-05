/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QCheckBox *cpuCheckBox13;
    QLabel *infoLabel;
    QCheckBox *cpuCheckBox8;
    QPushButton *StartProcessClicked;
    QCheckBox *cpuCheckBox2;
    QPushButton *SetPriority;
    QCheckBox *cpuCheckBox0;
    QPushButton *SuspendProcess;
    QCheckBox *cpuCheckBox15;
    QCheckBox *cpuCheckBox14;
    QCheckBox *cpuCheckBox9;
    QCheckBox *cpuCheckBox4;
    QPushButton *fileButton;
    QCheckBox *cpuCheckBox5;
    QCheckBox *cpuCheckBox3;
    QComboBox *comboBox;
    QCheckBox *cpuCheckBox11;
    QLineEdit *PathEdit;
    QCheckBox *cpuCheckBox7;
    QComboBox *processComboBox;
    QPushButton *ResumeProcess;
    QPushButton *TerminateProcess;
    QCheckBox *cpuCheckBox6;
    QCheckBox *cpuCheckBox12;
    QPushButton *SetAffinity;
    QCheckBox *cpuCheckBox10;
    QCheckBox *cpuCheckBox1;
    QComboBox *priorityComboBox;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        cpuCheckBox13 = new QCheckBox(centralwidget);
        cpuCheckBox13->setObjectName("cpuCheckBox13");
        cpuCheckBox13->setGeometry(QRect(270, 140, 61, 18));
        infoLabel = new QLabel(centralwidget);
        infoLabel->setObjectName("infoLabel");
        infoLabel->setGeometry(QRect(490, 350, 221, 101));
        cpuCheckBox8 = new QCheckBox(centralwidget);
        cpuCheckBox8->setObjectName("cpuCheckBox8");
        cpuCheckBox8->setGeometry(QRect(210, 120, 58, 18));
        StartProcessClicked = new QPushButton(centralwidget);
        StartProcessClicked->setObjectName("StartProcessClicked");
        StartProcessClicked->setGeometry(QRect(379, 70, 101, 21));
        cpuCheckBox2 = new QCheckBox(centralwidget);
        cpuCheckBox2->setObjectName("cpuCheckBox2");
        cpuCheckBox2->setGeometry(QRect(90, 160, 58, 18));
        SetPriority = new QPushButton(centralwidget);
        SetPriority->setObjectName("SetPriority");
        SetPriority->setGeometry(QRect(380, 220, 101, 18));
        cpuCheckBox0 = new QCheckBox(centralwidget);
        cpuCheckBox0->setObjectName("cpuCheckBox0");
        cpuCheckBox0->setGeometry(QRect(90, 120, 58, 18));
        SuspendProcess = new QPushButton(centralwidget);
        SuspendProcess->setObjectName("SuspendProcess");
        SuspendProcess->setGeometry(QRect(379, 100, 101, 20));
        cpuCheckBox15 = new QCheckBox(centralwidget);
        cpuCheckBox15->setObjectName("cpuCheckBox15");
        cpuCheckBox15->setGeometry(QRect(270, 180, 61, 18));
        cpuCheckBox14 = new QCheckBox(centralwidget);
        cpuCheckBox14->setObjectName("cpuCheckBox14");
        cpuCheckBox14->setGeometry(QRect(270, 160, 61, 18));
        cpuCheckBox9 = new QCheckBox(centralwidget);
        cpuCheckBox9->setObjectName("cpuCheckBox9");
        cpuCheckBox9->setGeometry(QRect(210, 140, 58, 18));
        cpuCheckBox4 = new QCheckBox(centralwidget);
        cpuCheckBox4->setObjectName("cpuCheckBox4");
        cpuCheckBox4->setGeometry(QRect(150, 120, 58, 18));
        fileButton = new QPushButton(centralwidget);
        fileButton->setObjectName("fileButton");
        fileButton->setGeometry(QRect(380, 260, 80, 21));
        QFont font;
        font.setPointSize(10);
        fileButton->setFont(font);
        cpuCheckBox5 = new QCheckBox(centralwidget);
        cpuCheckBox5->setObjectName("cpuCheckBox5");
        cpuCheckBox5->setGeometry(QRect(150, 140, 58, 18));
        cpuCheckBox3 = new QCheckBox(centralwidget);
        cpuCheckBox3->setObjectName("cpuCheckBox3");
        cpuCheckBox3->setGeometry(QRect(90, 180, 58, 18));
        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(160, 260, 191, 22));
        comboBox->setFont(font);
        cpuCheckBox11 = new QCheckBox(centralwidget);
        cpuCheckBox11->setObjectName("cpuCheckBox11");
        cpuCheckBox11->setGeometry(QRect(210, 180, 58, 18));
        PathEdit = new QLineEdit(centralwidget);
        PathEdit->setObjectName("PathEdit");
        PathEdit->setGeometry(QRect(130, 70, 201, 20));
        cpuCheckBox7 = new QCheckBox(centralwidget);
        cpuCheckBox7->setObjectName("cpuCheckBox7");
        cpuCheckBox7->setGeometry(QRect(150, 180, 58, 18));
        processComboBox = new QComboBox(centralwidget);
        processComboBox->setObjectName("processComboBox");
        processComboBox->setGeometry(QRect(500, 70, 161, 22));
        ResumeProcess = new QPushButton(centralwidget);
        ResumeProcess->setObjectName("ResumeProcess");
        ResumeProcess->setGeometry(QRect(379, 130, 101, 20));
        TerminateProcess = new QPushButton(centralwidget);
        TerminateProcess->setObjectName("TerminateProcess");
        TerminateProcess->setGeometry(QRect(380, 160, 101, 18));
        cpuCheckBox6 = new QCheckBox(centralwidget);
        cpuCheckBox6->setObjectName("cpuCheckBox6");
        cpuCheckBox6->setGeometry(QRect(150, 160, 58, 18));
        cpuCheckBox12 = new QCheckBox(centralwidget);
        cpuCheckBox12->setObjectName("cpuCheckBox12");
        cpuCheckBox12->setGeometry(QRect(270, 120, 61, 18));
        SetAffinity = new QPushButton(centralwidget);
        SetAffinity->setObjectName("SetAffinity");
        SetAffinity->setGeometry(QRect(380, 190, 101, 18));
        cpuCheckBox10 = new QCheckBox(centralwidget);
        cpuCheckBox10->setObjectName("cpuCheckBox10");
        cpuCheckBox10->setGeometry(QRect(210, 160, 58, 18));
        cpuCheckBox1 = new QCheckBox(centralwidget);
        cpuCheckBox1->setObjectName("cpuCheckBox1");
        cpuCheckBox1->setGeometry(QRect(90, 140, 58, 18));
        priorityComboBox = new QComboBox(centralwidget);
        priorityComboBox->setObjectName("priorityComboBox");
        priorityComboBox->setGeometry(QRect(160, 220, 191, 22));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        cpuCheckBox13->setText(QCoreApplication::translate("MainWindow", "CPU 13", nullptr));
        infoLabel->setText(QString());
        cpuCheckBox8->setText(QCoreApplication::translate("MainWindow", "CPU 8", nullptr));
        StartProcessClicked->setText(QCoreApplication::translate("MainWindow", "Create Process", nullptr));
        cpuCheckBox2->setText(QCoreApplication::translate("MainWindow", "CPU 2", nullptr));
        SetPriority->setText(QCoreApplication::translate("MainWindow", "Set Priority", nullptr));
        cpuCheckBox0->setText(QCoreApplication::translate("MainWindow", "CPU 0", nullptr));
        SuspendProcess->setText(QCoreApplication::translate("MainWindow", "Suspend Process", nullptr));
        cpuCheckBox15->setText(QCoreApplication::translate("MainWindow", "CPU 15", nullptr));
        cpuCheckBox14->setText(QCoreApplication::translate("MainWindow", "CPU 14", nullptr));
        cpuCheckBox9->setText(QCoreApplication::translate("MainWindow", "CPU 9", nullptr));
        cpuCheckBox4->setText(QCoreApplication::translate("MainWindow", "CPU 4", nullptr));
        fileButton->setText(QCoreApplication::translate("MainWindow", "Set", nullptr));
        cpuCheckBox5->setText(QCoreApplication::translate("MainWindow", "CPU 5", nullptr));
        cpuCheckBox3->setText(QCoreApplication::translate("MainWindow", "CPU 3", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Create new file", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Set only read permission", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Set write permission", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Set SID Normal", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("MainWindow", "Set SID Administrator", nullptr));
        comboBox->setItemText(5, QCoreApplication::translate("MainWindow", "Set SID Group", nullptr));

        cpuCheckBox11->setText(QCoreApplication::translate("MainWindow", "CPU 11", nullptr));
        cpuCheckBox7->setText(QCoreApplication::translate("MainWindow", "CPU 7", nullptr));
        ResumeProcess->setText(QCoreApplication::translate("MainWindow", "Resume Process", nullptr));
        TerminateProcess->setText(QCoreApplication::translate("MainWindow", "Terminate Process", nullptr));
        cpuCheckBox6->setText(QCoreApplication::translate("MainWindow", "CPU 6", nullptr));
        cpuCheckBox12->setText(QCoreApplication::translate("MainWindow", "CPU 12", nullptr));
        SetAffinity->setText(QCoreApplication::translate("MainWindow", "Set Affinity", nullptr));
        cpuCheckBox10->setText(QCoreApplication::translate("MainWindow", "CPU 10", nullptr));
        cpuCheckBox1->setText(QCoreApplication::translate("MainWindow", "CPU 1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
