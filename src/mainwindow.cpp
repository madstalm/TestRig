#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pollTimer(new QTimer(this))
{
    ui->setupUi(this);
    setWindowTitle("Test Rig Axis Control");

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
    connect(ui->axisSelection,      QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onAxisSelectionChanged);
    connect(ui->solenoidToggle,     &QCheckBox::toggled, this, &MainWindow::onSolenoidToggleToggled);
    connect(ui->solenoidPulseBtn,   &QPushButton::clicked, this, &MainWindow::onSolenoidPulseClicked);

    ui->axisSelection->addItem("X-Axis", QVariant::fromValue(static_cast<int>(Axis::X)));
    ui->axisSelection->addItem("Y-Axis", QVariant::fromValue(static_cast<int>(Axis::Y)));
    ui->axisSelection->addItem("Z-Axis", QVariant::fromValue(static_cast<int>(Axis::Z)));
    ui->axisSelection->addItem("Reservoir", QVariant::fromValue(static_cast<int>(Axis::H)));

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
        setSolenoidState(false);
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
    Axis axis = selectedAxis();

    if (m_galil.isMotorEnabled(axis)) {
        if (m_galil.disableMotor(axis))
            log("Motor disabled.");
        else
            log("Error disabling motor: " + m_galil.getLastError());
    } else {
        if (m_galil.enableMotor(axis))
            log("Motor enabled.");
        else
            log("Error enabling motor: " + m_galil.getLastError());
    }
}

void MainWindow::onApplyParametersClicked()
{
    Axis axis = selectedAxis();

    double speed = ui->speedSpin->value();
    double accel = ui->accelSpin->value();
    double decel = ui->decelSpin->value();

    bool ok = m_galil.setSpeed(axis, speed)
           && m_galil.setAcceleration(axis, accel)
           && m_galil.setDeceleration(axis, decel);

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
    Axis axis = selectedAxis();
    double mm = -ui->stepSizeSpin->value();
    if (!m_galil.moveRelative(axis, mm))
        log("Jog error: " + m_galil.getLastError());
    else
        log(QString("Jogging %1 mm.").arg(mm));
}

void MainWindow::onJogPosClicked()
{
    Axis axis = selectedAxis();
    double mm = ui->stepSizeSpin->value();
    if (!m_galil.moveRelative(axis, mm))
        log("Jog error: " + m_galil.getLastError());
    else
        log(QString("Jogging +%1 mm.").arg(mm));
}

void MainWindow::onStopClicked()
{
    Axis axis = selectedAxis();
    if (!m_galil.stop(axis))
        log("Stop error: " + m_galil.getLastError());
    else
        log("Stop commanded.");
}

void MainWindow::onMoveAbsClicked()
{
    Axis axis = selectedAxis();
    double target = ui->targetPosSpin->value();
    if (!m_galil.moveAbsolute(axis, target))
        log("Move error: " + m_galil.getLastError());
    else
        log(QString("Moving to absolute position %1 mm.").arg(target));
}

void MainWindow::onHomeClicked()
{
    Axis axis = selectedAxis();
    if (!m_galil.home(axis))
        log("Home error: " + m_galil.getLastError());
    else
        log("Homing started.");
}

void MainWindow::onZeroHereClicked()
{
    Axis axis = selectedAxis();
    if (!m_galil.definePositionZero(axis))
        log("Zero error: " + m_galil.getLastError());
    else
        log("Current position defined as 0.");
}

// ---------------------------------------------------------------------------
// Axis selection
// ---------------------------------------------------------------------------

void MainWindow::onAxisSelectionChanged(int /*index*/)
{
    // Refresh status immediately rather than waiting for the next poll tick,
    // so the displayed position/limits/enable-state always match the
    // currently selected axis without a visible delay.
    if (m_galil.isConnected())
        pollStatus();
}

void MainWindow::onSolenoidToggleToggled(bool checked)
{
    setSolenoidState(checked);
}

void MainWindow::onSolenoidPulseClicked()
{
    if (!m_galil.isConnected()) {
        log("Solenoid pulse ignored: not connected.");
        return;
    }

    if (!m_galil.setSolenoidState(true)) {
        log("Pulse failed: " + m_galil.getLastError());
        return;
    }

    ui->solenoidToggle->setChecked(true);
    QTimer::singleShot(250, this, [this]() {
        setSolenoidState(false);
    });
    log("Solenoid pulsed.");
}

// ---------------------------------------------------------------------------
// Polling
// ---------------------------------------------------------------------------

void MainWindow::pollStatus()
{
    Axis axis = selectedAxis();

    // Position in mm, 3 decimal places (= ~0.04 µm resolution at 256 µstep)
    double pos = m_galil.getPosition(axis);
    ui->positionLabel->setText(QString::number(pos, 'f', 3));

    // Motion busy
    bool moving = m_galil.isMoving(axis);
    ui->motionStatusLabel->setText(moving ? "Moving" : "Idle");

    // Limit switches
    bool fwd = m_galil.isForwardLimitActive(axis);
    bool rev = m_galil.isReverseLimitActive(axis);

    ui->fwdLimitLabel->setText(fwd ? "ACTIVE" : "OK");
    ui->revLimitLabel->setText(rev ? "ACTIVE" : "OK");

    ui->fwdLimitLabel->setStyleSheet(fwd ? "color: red; font-weight: bold;" : "");
    ui->revLimitLabel->setStyleSheet(rev ? "color: red; font-weight: bold;" : "");

    // Motor enable button label
    bool enabled = m_galil.isMotorEnabled(axis);
    ui->enableMotorBtn->setText(enabled ? "Disable Motor" : "Enable Motor");
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

Axis MainWindow::selectedAxis() const
{
    return static_cast<Axis>(ui->axisSelection->currentData().toInt());
}

void MainWindow::setConnectedState(bool connected)
{
    ui->connectBtn->setText(connected ? "Disconnect" : "Connect");
    ui->ipEdit->setEnabled(!connected);
    ui->axisGroup->setEnabled(connected);
    ui->solenoidGroup->setEnabled(connected);

    if (!connected)
        ui->solenoidToggle->setChecked(false);
}

void MainWindow::setSolenoidState(bool open)
{
    if (!m_galil.isConnected()) {
        ui->solenoidToggle->setChecked(false);
        return;
    }

    if (!m_galil.setSolenoidState(open)) {
        log(QString("Solenoid %1 failed: %2")
                .arg(open ? "open" : "close")
                .arg(m_galil.getLastError()));
        ui->solenoidToggle->setChecked(!open);
        return;
    }

    ui->solenoidToggle->setChecked(open);
}

void MainWindow::log(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->logEdit->append(QString("[%1] %2").arg(timestamp, message));
}
