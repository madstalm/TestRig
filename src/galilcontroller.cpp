#include "galilcontroller.h"

#include <gclib.h>
#include <gclibo.h>

#include <cstring>
#include <cstdlib>

// Buffer size for gclib responses
static constexpr unsigned int GCLIB_BUFSIZE = 1024;

GalilController::GalilController()
    : m_handle(nullptr)
    , m_connected(false)
{}

GalilController::~GalilController()
{
    disconnect();
}

// ---------------------------------------------------------------------------
// Connection
// ---------------------------------------------------------------------------

bool GalilController::connect(const QString &address)
{
    if (m_connected)
        disconnect();

    GReturn rc = GOpen(address.toLocal8Bit().constData(), &m_handle);
    if (rc != G_NO_ERROR) {
        char errBuf[GCLIB_BUFSIZE] = {};
        GError(rc, errBuf, sizeof(errBuf));
        m_lastError = QString("GOpen failed: %1").arg(errBuf);
        m_handle = nullptr;
        return false;
    }

    m_connected = true;
    m_lastError.clear();
    return true;
}

void GalilController::disconnect()
{
    if (!m_connected)
        return;

    GClose(m_handle);
    m_handle = nullptr;
    m_connected = false;
}

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

bool GalilController::sendCommand(const QString &cmd, QString *response)
{
    if (!m_connected) {
        m_lastError = "Not connected.";
        return false;
    }

    char buf[GCLIB_BUFSIZE] = {};
    GReturn rc = GCommand(m_handle,
                          cmd.toLocal8Bit().constData(),
                          buf,
                          sizeof(buf),
                          nullptr);

    if (rc != G_NO_ERROR) {
        char errBuf[GCLIB_BUFSIZE] = {};
        GError(rc, errBuf, sizeof(errBuf));
        m_lastError = QString("Command \"%1\" failed: %2").arg(cmd, errBuf);
        return false;
    }

    if (response)
        *response = QString(buf).trimmed();

    m_lastError.clear();
    return true;
}

double GalilController::queryValue(const QString &mgExpression, bool *ok)
{
    QString resp;
    if (!sendCommand("MG " + mgExpression, &resp)) {
        if (ok) *ok = false;
        return 0.0;
    }
    if (ok) *ok = true;
    return resp.toDouble();
}

// ---------------------------------------------------------------------------
// Axis -> Galil Connector map
// ---------------------------------------------------------------------------

const std::map<Axis, char> GalilController::axisConnectorMap = {
    { Axis::X,        'A' },
    { Axis::Y,        'B' },
    { Axis::Z,        'C' },
    { Axis::H,        'D' },
};

// ---------------------------------------------------------------------------
// Motor enable / disable
// ---------------------------------------------------------------------------

bool GalilController::enableMotor(Axis axis)
{
    return sendCommand(QString("SH") + connectorFor(axis));
}

bool GalilController::disableMotor(Axis axis)
{
    return sendCommand(QString("MO") + connectorFor(axis));
}

bool GalilController::isMotorEnabled(Axis axis)
{
    bool ok = false;
    double val = queryValue(QString("_MO") + connectorFor(axis), &ok);
    if (!ok)
        return false;   // failed query → assume not enabled (safer default)
    return (val != 0.0);   // _MOA=1.0000 means motor is OFF for our axis, _MOA=0.0000 means motor is ON
}

void GalilController::ensureMotorEnabled(Axis axis)
{
    if (!isMotorEnabled(axis)) {
        enableMotor(axis);
    }    
}

// ---------------------------------------------------------------------------
// Motion parameters
// ---------------------------------------------------------------------------

bool GalilController::setSpeed(Axis axis, double mmPerSec)
{
    long microsteps = static_cast<long>(mmPerSec * MICROSTEPS_PER_MM);
    return sendCommand(QString("SP") + connectorFor(axis) + QString("=%1").arg(microsteps));
}

bool GalilController::setAcceleration(Axis axis, double mmPerSecSq)
{
    long microsteps = static_cast<long>(mmPerSecSq * MICROSTEPS_PER_MM);
    return sendCommand(QString("AC") + connectorFor(axis) + QString("=%1").arg(microsteps));
}

bool GalilController::setDeceleration(Axis axis, double mmPerSecSq)
{
    long microsteps = static_cast<long>(mmPerSecSq * MICROSTEPS_PER_MM);
    return sendCommand(QString("DC") + connectorFor(axis) + QString("=%1").arg(microsteps));
}

// ---------------------------------------------------------------------------
// Motion commands
// ---------------------------------------------------------------------------

bool GalilController::moveRelative(Axis axis, double mm)
{
    ensureMotorEnabled(axis);
    long microsteps = static_cast<long>(mm * MICROSTEPS_PER_MM);
    if (!sendCommand(QString("PR") + connectorFor(axis) + QString("=%1").arg(microsteps))) return false;
    return sendCommand(QString("BG") + connectorFor(axis));
}

bool GalilController::moveAbsolute(Axis axis, double mm)
{
    ensureMotorEnabled(axis);
    long microsteps = static_cast<long>(mm * MICROSTEPS_PER_MM);
    if (!sendCommand(QString("PA") + connectorFor(axis) + QString("=%1").arg(microsteps))) return false;
    return sendCommand(QString("BG") + connectorFor(axis));
}

bool GalilController::stop(Axis axis)
{
    return sendCommand(QString("ST") + connectorFor(axis));
}

bool GalilController::abort()
{
    return sendCommand("AB");
}

bool GalilController::home(Axis axis)
{
    ensureMotorEnabled(axis);
    return sendCommand(QString("HM") + connectorFor(axis)) && sendCommand(QString("BG") + connectorFor(axis));
}

bool GalilController::definePositionZero(Axis axis)
{
    return sendCommand(QString("DP") + connectorFor(axis) + QString("=0"));
}

// ---------------------------------------------------------------------------
// State queries
// ---------------------------------------------------------------------------

double GalilController::getPosition(Axis axis)
{
    // Convert microsteps back to mm
    return queryValue(QString("_TP") + connectorFor(axis)) / MICROSTEPS_PER_MM;
}

bool GalilController::isMoving(Axis axis)
{
    return (queryValue(QString("_BG") + connectorFor(axis)) != 0.0);
}

bool GalilController::isForwardLimitActive(Axis axis)
{
    return (queryValue(QString("_LF") + connectorFor(axis)) == 0.0);
}

bool GalilController::isReverseLimitActive(Axis axis)
{
    return (queryValue(QString("_LR") + connectorFor(axis)) == 0.0);
}

