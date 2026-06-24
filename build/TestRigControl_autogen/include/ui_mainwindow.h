/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *connectionGroup;
    QHBoxLayout *connectionLayout;
    QLabel *ipLabel;
    QLineEdit *ipEdit;
    QPushButton *connectBtn;
    QSpacerItem *connectionSpacer;
    QGroupBox *axisGroup;
    QVBoxLayout *axisLayout;
    QHBoxLayout *statusLayout;
    QLabel *label;
    QLabel *positionLabel;
    QLabel *label1;
    QSpacerItem *spacerItem;
    QLabel *label2;
    QLabel *motionStatusLabel;
    QSpacerItem *spacerItem1;
    QLabel *label3;
    QLabel *fwdLimitLabel;
    QLabel *label4;
    QLabel *revLimitLabel;
    QSpacerItem *spacerItem2;
    QPushButton *enableMotorBtn;
    QGroupBox *paramsGroup;
    QHBoxLayout *paramsLayout;
    QLabel *label5;
    QSpinBox *speedSpin;
    QLabel *label6;
    QSpinBox *accelSpin;
    QLabel *label7;
    QSpinBox *decelSpin;
    QPushButton *applyParamsBtn;
    QGroupBox *jogGroup;
    QHBoxLayout *jogLayout;
    QLabel *label8;
    QSpinBox *stepSizeSpin;
    QSpacerItem *spacerItem3;
    QPushButton *jogNegBtn;
    QPushButton *stopBtn;
    QPushButton *jogPosBtn;
    QGroupBox *absMoveGroup;
    QHBoxLayout *absMoveLayout;
    QLabel *label9;
    QSpinBox *targetPosSpin;
    QPushButton *moveAbsBtn;
    QSpacerItem *spacerItem4;
    QPushButton *homeBtn;
    QPushButton *zeroHereBtn;
    QGroupBox *logGroup;
    QVBoxLayout *logLayout;
    QTextEdit *logEdit;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(700, 650);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        connectionGroup = new QGroupBox(centralwidget);
        connectionGroup->setObjectName("connectionGroup");
        connectionLayout = new QHBoxLayout(connectionGroup);
        connectionLayout->setObjectName("connectionLayout");
        ipLabel = new QLabel(connectionGroup);
        ipLabel->setObjectName("ipLabel");

        connectionLayout->addWidget(ipLabel);

        ipEdit = new QLineEdit(connectionGroup);
        ipEdit->setObjectName("ipEdit");
        ipEdit->setMaximumWidth(160);

        connectionLayout->addWidget(ipEdit);

        connectBtn = new QPushButton(connectionGroup);
        connectBtn->setObjectName("connectBtn");

        connectionLayout->addWidget(connectBtn);

        connectionSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        connectionLayout->addItem(connectionSpacer);


        verticalLayout->addWidget(connectionGroup);

        axisGroup = new QGroupBox(centralwidget);
        axisGroup->setObjectName("axisGroup");
        axisGroup->setEnabled(false);
        axisLayout = new QVBoxLayout(axisGroup);
        axisLayout->setObjectName("axisLayout");
        statusLayout = new QHBoxLayout();
        statusLayout->setObjectName("statusLayout");
        label = new QLabel(axisGroup);
        label->setObjectName("label");

        statusLayout->addWidget(label);

        positionLabel = new QLabel(axisGroup);
        positionLabel->setObjectName("positionLabel");
        positionLabel->setMinimumWidth(80);
        positionLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        statusLayout->addWidget(positionLabel);

        label1 = new QLabel(axisGroup);
        label1->setObjectName("label1");

        statusLayout->addWidget(label1);

        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        statusLayout->addItem(spacerItem);

        label2 = new QLabel(axisGroup);
        label2->setObjectName("label2");

        statusLayout->addWidget(label2);

        motionStatusLabel = new QLabel(axisGroup);
        motionStatusLabel->setObjectName("motionStatusLabel");

        statusLayout->addWidget(motionStatusLabel);

        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        statusLayout->addItem(spacerItem1);

        label3 = new QLabel(axisGroup);
        label3->setObjectName("label3");

        statusLayout->addWidget(label3);

        fwdLimitLabel = new QLabel(axisGroup);
        fwdLimitLabel->setObjectName("fwdLimitLabel");

        statusLayout->addWidget(fwdLimitLabel);

        label4 = new QLabel(axisGroup);
        label4->setObjectName("label4");

        statusLayout->addWidget(label4);

        revLimitLabel = new QLabel(axisGroup);
        revLimitLabel->setObjectName("revLimitLabel");

        statusLayout->addWidget(revLimitLabel);

        spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        statusLayout->addItem(spacerItem2);

        enableMotorBtn = new QPushButton(axisGroup);
        enableMotorBtn->setObjectName("enableMotorBtn");

        statusLayout->addWidget(enableMotorBtn);


        axisLayout->addLayout(statusLayout);

        paramsGroup = new QGroupBox(axisGroup);
        paramsGroup->setObjectName("paramsGroup");
        paramsLayout = new QHBoxLayout(paramsGroup);
        paramsLayout->setObjectName("paramsLayout");
        label5 = new QLabel(paramsGroup);
        label5->setObjectName("label5");

        paramsLayout->addWidget(label5);

        speedSpin = new QSpinBox(paramsGroup);
        speedSpin->setObjectName("speedSpin");
        speedSpin->setMinimum(1);
        speedSpin->setMaximum(500000);
        speedSpin->setValue(5000);
        speedSpin->setSingleStep(1000);

        paramsLayout->addWidget(speedSpin);

        label6 = new QLabel(paramsGroup);
        label6->setObjectName("label6");

        paramsLayout->addWidget(label6);

        accelSpin = new QSpinBox(paramsGroup);
        accelSpin->setObjectName("accelSpin");
        accelSpin->setMinimum(1);
        accelSpin->setMaximum(5000000);
        accelSpin->setValue(50000);
        accelSpin->setSingleStep(10000);

        paramsLayout->addWidget(accelSpin);

        label7 = new QLabel(paramsGroup);
        label7->setObjectName("label7");

        paramsLayout->addWidget(label7);

        decelSpin = new QSpinBox(paramsGroup);
        decelSpin->setObjectName("decelSpin");
        decelSpin->setMinimum(1);
        decelSpin->setMaximum(5000000);
        decelSpin->setValue(50000);
        decelSpin->setSingleStep(10000);

        paramsLayout->addWidget(decelSpin);

        applyParamsBtn = new QPushButton(paramsGroup);
        applyParamsBtn->setObjectName("applyParamsBtn");

        paramsLayout->addWidget(applyParamsBtn);


        axisLayout->addWidget(paramsGroup);

        jogGroup = new QGroupBox(axisGroup);
        jogGroup->setObjectName("jogGroup");
        jogLayout = new QHBoxLayout(jogGroup);
        jogLayout->setObjectName("jogLayout");
        label8 = new QLabel(jogGroup);
        label8->setObjectName("label8");

        jogLayout->addWidget(label8);

        stepSizeSpin = new QSpinBox(jogGroup);
        stepSizeSpin->setObjectName("stepSizeSpin");
        stepSizeSpin->setMinimum(1);
        stepSizeSpin->setMaximum(1000000);
        stepSizeSpin->setValue(100);
        stepSizeSpin->setSingleStep(100);

        jogLayout->addWidget(stepSizeSpin);

        spacerItem3 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        jogLayout->addItem(spacerItem3);

        jogNegBtn = new QPushButton(jogGroup);
        jogNegBtn->setObjectName("jogNegBtn");

        jogLayout->addWidget(jogNegBtn);

        stopBtn = new QPushButton(jogGroup);
        stopBtn->setObjectName("stopBtn");

        jogLayout->addWidget(stopBtn);

        jogPosBtn = new QPushButton(jogGroup);
        jogPosBtn->setObjectName("jogPosBtn");

        jogLayout->addWidget(jogPosBtn);


        axisLayout->addWidget(jogGroup);

        absMoveGroup = new QGroupBox(axisGroup);
        absMoveGroup->setObjectName("absMoveGroup");
        absMoveLayout = new QHBoxLayout(absMoveGroup);
        absMoveLayout->setObjectName("absMoveLayout");
        label9 = new QLabel(absMoveGroup);
        label9->setObjectName("label9");

        absMoveLayout->addWidget(label9);

        targetPosSpin = new QSpinBox(absMoveGroup);
        targetPosSpin->setObjectName("targetPosSpin");
        targetPosSpin->setMinimum(-2000000);
        targetPosSpin->setMaximum(2000000);
        targetPosSpin->setValue(0);
        targetPosSpin->setSingleStep(100);

        absMoveLayout->addWidget(targetPosSpin);

        moveAbsBtn = new QPushButton(absMoveGroup);
        moveAbsBtn->setObjectName("moveAbsBtn");

        absMoveLayout->addWidget(moveAbsBtn);

        spacerItem4 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        absMoveLayout->addItem(spacerItem4);

        homeBtn = new QPushButton(absMoveGroup);
        homeBtn->setObjectName("homeBtn");

        absMoveLayout->addWidget(homeBtn);

        zeroHereBtn = new QPushButton(absMoveGroup);
        zeroHereBtn->setObjectName("zeroHereBtn");

        absMoveLayout->addWidget(zeroHereBtn);


        axisLayout->addWidget(absMoveGroup);


        verticalLayout->addWidget(axisGroup);

        logGroup = new QGroupBox(centralwidget);
        logGroup->setObjectName("logGroup");
        logLayout = new QVBoxLayout(logGroup);
        logLayout->setObjectName("logLayout");
        logEdit = new QTextEdit(logGroup);
        logEdit->setObjectName("logEdit");
        logEdit->setReadOnly(true);

        logLayout->addWidget(logEdit);


        verticalLayout->addWidget(logGroup);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Test Rig X-Axis Control", nullptr));
        connectionGroup->setTitle(QCoreApplication::translate("MainWindow", "Connection", nullptr));
        ipLabel->setText(QCoreApplication::translate("MainWindow", "Controller IP:", nullptr));
        ipEdit->setText(QCoreApplication::translate("MainWindow", "192.168.42.100", nullptr));
        connectBtn->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        axisGroup->setTitle(QCoreApplication::translate("MainWindow", "X Axis (Axis A)", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Position:", nullptr));
        positionLabel->setText(QCoreApplication::translate("MainWindow", "---", nullptr));
        label1->setText(QCoreApplication::translate("MainWindow", "steps", nullptr));
        label2->setText(QCoreApplication::translate("MainWindow", "Status:", nullptr));
        motionStatusLabel->setText(QCoreApplication::translate("MainWindow", "---", nullptr));
        label3->setText(QCoreApplication::translate("MainWindow", "Fwd Limit:", nullptr));
        fwdLimitLabel->setText(QCoreApplication::translate("MainWindow", "---", nullptr));
        label4->setText(QCoreApplication::translate("MainWindow", "Rev Limit:", nullptr));
        revLimitLabel->setText(QCoreApplication::translate("MainWindow", "---", nullptr));
        enableMotorBtn->setText(QCoreApplication::translate("MainWindow", "Enable Motor", nullptr));
        paramsGroup->setTitle(QCoreApplication::translate("MainWindow", "Motion Parameters", nullptr));
        label5->setText(QCoreApplication::translate("MainWindow", "Speed (steps/s):", nullptr));
        label6->setText(QCoreApplication::translate("MainWindow", "Accel (steps/s\302\262):", nullptr));
        label7->setText(QCoreApplication::translate("MainWindow", "Decel (steps/s\302\262):", nullptr));
        applyParamsBtn->setText(QCoreApplication::translate("MainWindow", "Apply", nullptr));
        jogGroup->setTitle(QCoreApplication::translate("MainWindow", "Jog", nullptr));
        label8->setText(QCoreApplication::translate("MainWindow", "Step size (steps):", nullptr));
        jogNegBtn->setText(QCoreApplication::translate("MainWindow", "\342\227\200 Jog \342\210\222", nullptr));
        stopBtn->setText(QCoreApplication::translate("MainWindow", "\342\226\240 STOP", nullptr));
        jogPosBtn->setText(QCoreApplication::translate("MainWindow", "Jog + \342\226\266", nullptr));
        absMoveGroup->setTitle(QCoreApplication::translate("MainWindow", "Absolute Move", nullptr));
        label9->setText(QCoreApplication::translate("MainWindow", "Target position (steps):", nullptr));
        moveAbsBtn->setText(QCoreApplication::translate("MainWindow", "Go", nullptr));
        homeBtn->setText(QCoreApplication::translate("MainWindow", "Home", nullptr));
        zeroHereBtn->setText(QCoreApplication::translate("MainWindow", "Zero Here", nullptr));
        logGroup->setTitle(QCoreApplication::translate("MainWindow", "Log", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
