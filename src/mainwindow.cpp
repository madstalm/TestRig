#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pollTimer(new QTimer(this))
{
    ui->setupUi(this);
    setWindowTitle("Test Rig X-Axis Control");

    // Wire up buttons
    connect(ui->connectBtn,         &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(ui->enableMotorBtn,     &QPushButton::clicked, this, &MainWindow::onEnableMotorClicked);
    connect(ui->applyParamsBtn,     &QPushButton::clicked, this, &MainWindow::onApplyParametersClicked);
    connect(ui->jogNegBtn,          &QPushButton::clicked, this, &MainWindow::onJogNegClicked);
    connect(ui->jogPosBtn,          &QPushButton::clicked, this, &MainWindow::onJogPosClicked);
    connect(ui->stopBtn,            &QPushButton::clicked, this, &MainWindow::onStopClicked);
    connect(ui->moveAbsBtn,         &QPushButton::clicked, this, &MainWindow::onMoveAbsClicked);
    connect(ui->homeBtn,            &QPushButton::clicked, this, &MainWindow::onHomeClicked);
    connect(ui->zeroHereBtn,        &QPushButton::clicked, this, &MainWindow::onZeroHereClicked);

    // Status polling every 200 ms
    connect(m_pollTimer, &QTimer::timeout, this, &MainWindow::pollStatus);

    setConnectedState(false);
    log("Application started. Enter the controller IP and click Connect.");
}

MainWindow::~MainWindow()
{
    m_pollTimer->stop();
    delete ui;
}

// ---------------------------------------------------------------------------
// Connection
// ---------------------------------------------------------------------------

void MainWindow::onConnectClicked()
{
    if (m_galil.isConnected()) {
        m_pollTimer->stop();
        m_galil.disconnect();
        setConnectedState(false);
        log("Disconnected.");
        return;
    }

    QString address = ui->ipEdit->text().trimmed();
    if (address.isEmpty()) {
        log("Error: IP address is empty.");
        return;
    }

    log(QString("Connecting to %1 ...").arg(address));
    if (m_galil.connect(address)) {
        setConnectedState(true);
        m_pollTimer->start(200);
        log("Connected.");
    } else {
        log("Connection failed: " + m_galil.getLastError());
    }
}

// ---------------------------------------------------------------------------
// Motor control
// ---------------------------------------------------------------------------

void MainWindow::onEnableMotorClicked()
{
    if (m_galil.isMotorEnabled()) {
        if (m_galil.disableMotor())
            log("Motor disabled.");
        else
            log("Error disabling motor: " + m_galil.getLastError());
    } else {
        if (m_galil.enableMotor())
            log("Motor enabled.");
        else
            log("Error enabling motor: " + m_galil.getLastError());
    }
}

void MainWindow::onApplyParametersClicked()
{
    double speed = ui->speedSpin->value();
    double accel = ui->accelSpin->value();
    double decel = ui->decelSpin->value();

    bool ok = m_galil.setSpeed(speed)
           && m_galil.setAcceleration(accel)
           && m_galil.setDeceleration(decel);

    if (ok)
        log(QString("Parameters set: speed=%1 mm/s, accel=%2 mm/s², decel=%3 mm/s²")
                .arg(speed).arg(accel).arg(decel));
    else
        log("Error setting parameters: " + m_galil.getLastError());
}

// ---------------------------------------------------------------------------
// Motion
// ---------------------------------------------------------------------------

void MainWindow::onJogNegClicked()
{
    double mm = -ui->stepSizeSpin->value();
    if (!m_galil.moveRelative(mm))
        log("Jog error: " + m_galil.getLastError());
    else
        log(QString("Jogging %1 mm.").arg(mm));
}

void MainWindow::onJogPosClicked()
{
    double mm = ui->stepSizeSpin->value();
    if (!m_galil.moveRelative(mm))
        log("Jog error: " + m_galil.getLastError());
    else
        log(QString("Jogging +%1 mm.").arg(mm));
}

void MainWindow::onStopClicked()
{
    if (!m_galil.stop())
        log("Stop error: " + m_galil.getLastError());
    else
        log("Stop commanded.");
}

void MainWindow::onMoveAbsClicked()
{
    double target = ui->targetPosSpin->value();
    if (!m_galil.moveAbsolute(target))
        log("Move error: " + m_galil.getLastError());
    else
        log(QString("Moving to absolute position %1 mm.").arg(target));
}

void MainWindow::onHomeClicked()
{
    if (!m_galil.home())
        log("Home error: " + m_galil.getLastError());
    else
        log("Homing started.");
}

void MainWindow::onZeroHereClicked()
{
    if (!m_galil.definePositionZero())
        log("Zero error: " + m_galil.getLastError());
    else
        log("Current position defined as 0.");
}

// ---------------------------------------------------------------------------
// Polling
// ---------------------------------------------------------------------------

void MainWindow::pollStatus()
{
    // Position in mm, 3 decimal places (= ~0.04 µm resolution at 256 µstep)
    double pos = m_galil.getPosition();
    ui->positionLabel->setText(QString::number(pos, 'f', 3));

    // Motion busy
    bool moving = m_galil.isMoving();
    ui->motionStatusLabel->setText(moving ? "Moving" : "Idle");

    // Limit switches
    bool fwd = m_galil.isForwardLimitActive();
    bool rev = m_galil.isReverseLimitActive();

    ui->fwdLimitLabel->setText(fwd ? "ACTIVE" : "OK");
    ui->revLimitLabel->setText(rev ? "ACTIVE" : "OK");

    ui->fwdLimitLabel->setStyleSheet(fwd ? "color: red; font-weight: bold;" : "");
    ui->revLimitLabel->setStyleSheet(rev ? "color: red; font-weight: bold;" : "");

    // Motor enable button label
    bool enabled = m_galil.isMotorEnabled();
    ui->enableMotorBtn->setText(enabled ? "Disable Motor" : "Enable Motor");
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

void MainWindow::setConnectedState(bool connected)
{
    ui->connectBtn->setText(connected ? "Disconnect" : "Connect");
    ui->ipEdit->setEnabled(!connected);
    ui->axisGroup->setEnabled(connected);
}

void MainWindow::log(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->logEdit->append(QString("[%1] %2").arg(timestamp, message));
}
