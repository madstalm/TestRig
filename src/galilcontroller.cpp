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

double GalilController::queryValue(const QString &mgExpression)
{
    QString resp;
    if (!sendCommand("MG " + mgExpression, &resp))
        return 0.0;
    return resp.toDouble();
}

// ---------------------------------------------------------------------------
// Motor enable / disable
// ---------------------------------------------------------------------------

bool GalilController::enableMotor()
{
    return sendCommand("SHA");
}

bool GalilController::disableMotor()
{
    return sendCommand("MOA");
}

bool GalilController::isMotorEnabled()
{
    double val = queryValue("_MOA");
    return (val == 0.0);
}

// ---------------------------------------------------------------------------
// Motion parameters
// ---------------------------------------------------------------------------

bool GalilController::setSpeed(double mmPerSec)
{
    long microsteps = static_cast<long>(mmPerSec * MICROSTEPS_PER_MM);
    return sendCommand(QString("SPA=%1").arg(microsteps));
}

bool GalilController::setAcceleration(double mmPerSecSq)
{
    long microsteps = static_cast<long>(mmPerSecSq * MICROSTEPS_PER_MM);
    return sendCommand(QString("ACA=%1").arg(microsteps));
}

bool GalilController::setDeceleration(double mmPerSecSq)
{
    long microsteps = static_cast<long>(mmPerSecSq * MICROSTEPS_PER_MM);
    return sendCommand(QString("DCA=%1").arg(microsteps));
}

// ---------------------------------------------------------------------------
// Motion commands
// ---------------------------------------------------------------------------

bool GalilController::moveRelative(double mm)
{
    long microsteps = static_cast<long>(mm * MICROSTEPS_PER_MM);
    if (!sendCommand(QString("PRA=%1").arg(microsteps))) return false;
    return sendCommand("BGA");
}

bool GalilController::moveAbsolute(double mm)
{
    long microsteps = static_cast<long>(mm * MICROSTEPS_PER_MM);
    if (!sendCommand(QString("PAA=%1").arg(microsteps))) return false;
    return sendCommand("BGA");
}

bool GalilController::stop()
{
    return sendCommand("STA");
}

bool GalilController::abort()
{
    return sendCommand("AB");
}

bool GalilController::home()
{
    return sendCommand("HMA") && sendCommand("BGA");
}

bool GalilController::definePositionZero()
{
    return sendCommand("DPA=0");
}

// ---------------------------------------------------------------------------
// State queries
// ---------------------------------------------------------------------------

double GalilController::getPosition()
{
    // Convert microsteps back to mm
    return queryValue("_TPA") / MICROSTEPS_PER_MM;
}

bool GalilController::isMoving()
{
    return (queryValue("_BGA") != 0.0);
}

bool GalilController::isForwardLimitActive()
{
    return (queryValue("_LFA") == 0.0);
}

bool GalilController::isReverseLimitActive()
{
    return (queryValue("_LRA") == 0.0);
}

