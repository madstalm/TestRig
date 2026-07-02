#pragma once

#include <QMainWindow>
#include <QTimer>
#include "galilcontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Connection
    void onConnectClicked();

    // Motor control
    void onEnableMotorClicked();
    void onApplyParametersClicked();

    // Motion
    void onJogNegClicked();
    void onJogPosClicked();
    void onStopClicked();
    void onMoveAbsClicked();
    void onHomeClicked();
    void onZeroHereClicked();

    // Axis polling
    void onAxisSelectionChanged(int);

    // Polling timer
    void pollStatus();

private:
    Axis selectedAxis() const;
    void setConnectedState(bool connected);
    void log(const QString &message);

    Ui::MainWindow *ui;
    GalilController m_galil;
    QTimer *m_pollTimer;
};
