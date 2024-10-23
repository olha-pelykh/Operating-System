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
    QPushButton *StartProcessClicked;
    QPushButton *SuspendProcess;
    QPushButton *ResumeProcess;
    QComboBox *processComboBox;
    QLineEdit *PathEdit;
    QPushButton *TerminateProcess;
    QPushButton *SetAffinity;
    QPushButton *SetPriority;
    QComboBox *priorityComboBox;
    QCheckBox *cpuCheckBox0;
    QCheckBox *cpuCheckBox1;
    QCheckBox *cpuCheckBox2;
    QCheckBox *cpuCheckBox3;
    QCheckBox *cpuCheckBox4;
    QCheckBox *cpuCheckBox6;
    QCheckBox *cpuCheckBox5;
    QCheckBox *cpuCheckBox7;
    QCheckBox *cpuCheckBox15;
    QCheckBox *cpuCheckBox8;
    QCheckBox *cpuCheckBox10;
    QCheckBox *cpuCheckBox13;
    QCheckBox *cpuCheckBox9;
    QCheckBox *cpuCheckBox14;
    QCheckBox *cpuCheckBox12;
    QCheckBox *cpuCheckBox11;
    QLabel *infoLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        StartProcessClicked = new QPushButton(centralwidget);
        StartProcessClicked->setObjectName("StartProcessClicked");
        StartProcessClicked->setGeometry(QRect(399, 70, 101, 21));
        SuspendProcess = new QPushButton(centralwidget);
        SuspendProcess->setObjectName("SuspendProcess");
        SuspendProcess->setGeometry(QRect(399, 100, 101, 20));
        ResumeProcess = new QPushButton(centralwidget);
        ResumeProcess->setObjectName("ResumeProcess");
        ResumeProcess->setGeometry(QRect(399, 130, 101, 20));
        processComboBox = new QComboBox(centralwidget);
        processComboBox->setObjectName("processComboBox");
        processComboBox->setGeometry(QRect(520, 70, 161, 22));
        PathEdit = new QLineEdit(centralwidget);
        PathEdit->setObjectName("PathEdit");
        PathEdit->setGeometry(QRect(150, 70, 201, 20));
        TerminateProcess = new QPushButton(centralwidget);
        TerminateProcess->setObjectName("TerminateProcess");
        TerminateProcess->setGeometry(QRect(400, 160, 101, 18));
        SetAffinity = new QPushButton(centralwidget);
        SetAffinity->setObjectName("SetAffinity");
        SetAffinity->setGeometry(QRect(400, 190, 101, 18));
        SetPriority = new QPushButton(centralwidget);
        SetPriority->setObjectName("SetPriority");
        SetPriority->setGeometry(QRect(400, 220, 101, 18));
        priorityComboBox = new QComboBox(centralwidget);
        priorityComboBox->setObjectName("priorityComboBox");
        priorityComboBox->setGeometry(QRect(180, 220, 191, 22));
        cpuCheckBox0 = new QCheckBox(centralwidget);
        cpuCheckBox0->setObjectName("cpuCheckBox0");
        cpuCheckBox0->setGeometry(QRect(110, 120, 58, 18));
        cpuCheckBox1 = new QCheckBox(centralwidget);
        cpuCheckBox1->setObjectName("cpuCheckBox1");
        cpuCheckBox1->setGeometry(QRect(110, 140, 58, 18));
        cpuCheckBox2 = new QCheckBox(centralwidget);
        cpuCheckBox2->setObjectName("cpuCheckBox2");
        cpuCheckBox2->setGeometry(QRect(110, 160, 58, 18));
        cpuCheckBox3 = new QCheckBox(centralwidget);
        cpuCheckBox3->setObjectName("cpuCheckBox3");
        cpuCheckBox3->setGeometry(QRect(110, 180, 58, 18));
        cpuCheckBox4 = new QCheckBox(centralwidget);
        cpuCheckBox4->setObjectName("cpuCheckBox4");
        cpuCheckBox4->setGeometry(QRect(170, 120, 58, 18));
        cpuCheckBox6 = new QCheckBox(centralwidget);
        cpuCheckBox6->setObjectName("cpuCheckBox6");
        cpuCheckBox6->setGeometry(QRect(170, 160, 58, 18));
        cpuCheckBox5 = new QCheckBox(centralwidget);
        cpuCheckBox5->setObjectName("cpuCheckBox5");
        cpuCheckBox5->setGeometry(QRect(170, 140, 58, 18));
        cpuCheckBox7 = new QCheckBox(centralwidget);
        cpuCheckBox7->setObjectName("cpuCheckBox7");
        cpuCheckBox7->setGeometry(QRect(170, 180, 58, 18));
        cpuCheckBox15 = new QCheckBox(centralwidget);
        cpuCheckBox15->setObjectName("cpuCheckBox15");
        cpuCheckBox15->setGeometry(QRect(290, 180, 61, 18));
        cpuCheckBox8 = new QCheckBox(centralwidget);
        cpuCheckBox8->setObjectName("cpuCheckBox8");
        cpuCheckBox8->setGeometry(QRect(230, 120, 58, 18));
        cpuCheckBox10 = new QCheckBox(centralwidget);
        cpuCheckBox10->setObjectName("cpuCheckBox10");
        cpuCheckBox10->setGeometry(QRect(230, 160, 58, 18));
        cpuCheckBox13 = new QCheckBox(centralwidget);
        cpuCheckBox13->setObjectName("cpuCheckBox13");
        cpuCheckBox13->setGeometry(QRect(290, 140, 61, 18));
        cpuCheckBox9 = new QCheckBox(centralwidget);
        cpuCheckBox9->setObjectName("cpuCheckBox9");
        cpuCheckBox9->setGeometry(QRect(230, 140, 58, 18));
        cpuCheckBox14 = new QCheckBox(centralwidget);
        cpuCheckBox14->setObjectName("cpuCheckBox14");
        cpuCheckBox14->setGeometry(QRect(290, 160, 61, 18));
        cpuCheckBox12 = new QCheckBox(centralwidget);
        cpuCheckBox12->setObjectName("cpuCheckBox12");
        cpuCheckBox12->setGeometry(QRect(290, 120, 61, 18));
        cpuCheckBox11 = new QCheckBox(centralwidget);
        cpuCheckBox11->setObjectName("cpuCheckBox11");
        cpuCheckBox11->setGeometry(QRect(230, 180, 58, 18));
        infoLabel = new QLabel(centralwidget);
        infoLabel->setObjectName("infoLabel");
        infoLabel->setGeometry(QRect(510, 350, 221, 101));
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
        StartProcessClicked->setText(QCoreApplication::translate("MainWindow", "Create Process", nullptr));
        SuspendProcess->setText(QCoreApplication::translate("MainWindow", "Suspend Process", nullptr));
        ResumeProcess->setText(QCoreApplication::translate("MainWindow", "Resume Process", nullptr));
        TerminateProcess->setText(QCoreApplication::translate("MainWindow", "Terminate Process", nullptr));
        SetAffinity->setText(QCoreApplication::translate("MainWindow", "Set Affinity", nullptr));
        SetPriority->setText(QCoreApplication::translate("MainWindow", "Set Priority", nullptr));
        cpuCheckBox0->setText(QCoreApplication::translate("MainWindow", "CPU 0", nullptr));
        cpuCheckBox1->setText(QCoreApplication::translate("MainWindow", "CPU 1", nullptr));
        cpuCheckBox2->setText(QCoreApplication::translate("MainWindow", "CPU 2", nullptr));
        cpuCheckBox3->setText(QCoreApplication::translate("MainWindow", "CPU 3", nullptr));
        cpuCheckBox4->setText(QCoreApplication::translate("MainWindow", "CPU 4", nullptr));
        cpuCheckBox6->setText(QCoreApplication::translate("MainWindow", "CPU 6", nullptr));
        cpuCheckBox5->setText(QCoreApplication::translate("MainWindow", "CPU 5", nullptr));
        cpuCheckBox7->setText(QCoreApplication::translate("MainWindow", "CPU 7", nullptr));
        cpuCheckBox15->setText(QCoreApplication::translate("MainWindow", "CPU 15", nullptr));
        cpuCheckBox8->setText(QCoreApplication::translate("MainWindow", "CPU 8", nullptr));
        cpuCheckBox10->setText(QCoreApplication::translate("MainWindow", "CPU 10", nullptr));
        cpuCheckBox13->setText(QCoreApplication::translate("MainWindow", "CPU 13", nullptr));
        cpuCheckBox9->setText(QCoreApplication::translate("MainWindow", "CPU 9", nullptr));
        cpuCheckBox14->setText(QCoreApplication::translate("MainWindow", "CPU 14", nullptr));
        cpuCheckBox12->setText(QCoreApplication::translate("MainWindow", "CPU 12", nullptr));
        cpuCheckBox11->setText(QCoreApplication::translate("MainWindow", "CPU 11", nullptr));
        infoLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
