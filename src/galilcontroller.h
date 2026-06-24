#pragma once

#include <QString>
#include <gclib.h>

class GalilController
{
public:
    GalilController();
    ~GalilController();

    // Connection
    bool connect(const QString &address);
    void disconnect();
    bool isConnected() const { return m_connected; }

    // Motor enable / disable
    bool enableMotor();
    bool disableMotor();

    // Motion parameters (units: steps/s and steps/s²)
    bool setSpeed(int stepsPerSec);
    bool setAcceleration(int stepsPerSecSq);
    bool setDeceleration(int stepsPerSecSq);

    // Motion commands — all return false on gclib error
    bool moveRelative(long steps);
    bool moveAbsolute(long position);
    bool stop();           // decelerated stop
    bool abort();          // immediate stop (use for E-stop)
    bool home();           // searches for home input, then limits
    bool definePositionZero(); // set current position as 0

    // State queries — return sensible defaults if not connected
    long   getPosition();
    bool   isMoving();
    bool   isForwardLimitActive();
    bool   isReverseLimitActive();
    bool   isMotorEnabled();

    QString getLastError() const { return m_lastError; }

private:
    // Send a command; optionally captures the response string.
    // Returns true on G_NO_ERROR.
    bool sendCommand(const QString &cmd, QString *response = nullptr);

    // Send "MG <expr>" and parse the returned float.
    // Returns 0.0 and sets m_lastError on failure.
    double queryValue(const QString &mgExpression);

    GCon    m_handle;
    bool    m_connected;
    QString m_lastError;
};
