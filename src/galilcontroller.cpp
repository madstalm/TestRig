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
    // "MG " + expression causes the controller to print the numeric result
    if (!sendCommand("MG " + mgExpression, &resp))
        return 0.0;
    return resp.toDouble();
}

// ---------------------------------------------------------------------------
// Motor enable / disable
// ---------------------------------------------------------------------------

bool GalilController::enableMotor()
{
    // SH = Servo Here: energises the motor and holds position
    return sendCommand("SHA");
}

bool GalilController::disableMotor()
{
    // MO = Motor Off: de-energises the motor
    return sendCommand("MOA");
}

bool GalilController::isMotorEnabled()
{
    // _MOA returns 0 if the motor is OFF, 1 if it is on
    // Invert: MO bit = 1 means off, so we negate
    double val = queryValue("_MOA");
    // _MOA == 0 → motor ON; _MOA == 1 → motor OFF
    return (val == 0.0);
}

// ---------------------------------------------------------------------------
// Motion parameters
// ---------------------------------------------------------------------------

bool GalilController::setSpeed(int stepsPerSec)
{
    return sendCommand(QString("SPA=%1").arg(stepsPerSec));
}

bool GalilController::setAcceleration(int stepsPerSecSq)
{
    return sendCommand(QString("ACA=%1").arg(stepsPerSecSq));
}

bool GalilController::setDeceleration(int stepsPerSecSq)
{
    return sendCommand(QString("DCA=%1").arg(stepsPerSecSq));
}

// ---------------------------------------------------------------------------
// Motion commands
// ---------------------------------------------------------------------------

bool GalilController::moveRelative(long steps)
{
    // PR sets the relative distance; BG starts motion
    if (!sendCommand(QString("PRA=%1").arg(steps))) return false;
    return sendCommand("BGA");
}

bool GalilController::moveAbsolute(long position)
{
    // PA sets an absolute target; BG starts motion
    if (!sendCommand(QString("PAA=%1").arg(position))) return false;
    return sendCommand("BGA");
}

bool GalilController::stop()
{
    // ST decelerates to a stop using the configured deceleration ramp
    return sendCommand("STA");
}

bool GalilController::abort()
{
    // AB halts all motion immediately with no deceleration
    return sendCommand("AB");
}

bool GalilController::home()
{
    // FI = Find Index (uses the home input on axis A).
    // If no home switch is wired, use HM instead, which searches limits.
    // Change to "HM A" and "BG A" if you use the home input differently.
    return sendCommand("HMA") && sendCommand("BGA");
}

bool GalilController::definePositionZero()
{
    // DP = Define Position: tells the controller the current position is 0
    return sendCommand("DPA=0");
}

// ---------------------------------------------------------------------------
// State queries
// ---------------------------------------------------------------------------

long GalilController::getPosition()
{
    // _TPA = Tell Position of Axis A
    return static_cast<long>(queryValue("_TPA"));
}

bool GalilController::isMoving()
{
    // _BGA = Busy flag for Axis A; 1 = moving, 0 = stopped
    return (queryValue("_BGA") != 0.0);
}

bool GalilController::isForwardLimitActive()
{
    // _LFA = Forward Limit input state for Axis A
    // On Galil, 0 = limit switch is ACTIVE (tripped), 1 = not active.
    // We return true when the limit is tripped, i.e. _LFA == 0.
    return (queryValue("_LFA") == 0.0);
}

bool GalilController::isReverseLimitActive()
{
    // _LRA = Reverse Limit input state for Axis A
    return (queryValue("_LRA") == 0.0);
}
