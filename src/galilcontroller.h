#pragma once

#include <QString>
#include <gclib.h>
#include <map>

enum class Axis {
    X,
    Y,
    Z,
    H,
};

class GalilController
{
public:
    GalilController();
    ~GalilController();

    // Conversion constant: 200 full steps/rev × 256 microsteps × (1 rev / 2 mm lead)
    static constexpr double MICROSTEPS_PER_MM = 25600.0;

    // Connection
    bool connect(const QString &address);
    void disconnect();
    bool isConnected() const { return m_connected; }

    // Motor enable / disable
    bool enableMotor(Axis axis);
    bool disableMotor(Axis axis);
    void ensureMotorEnabled(Axis axis);

    // Motion parameters (units: mm/s and mm/s²)
    bool setSpeed(Axis axis, double mmPerSec);
    bool setAcceleration(Axis axis, double mmPerSecSq);
    bool setDeceleration(Axis axis, double mmPerSecSq);

    // Motion commands — all return false on gclib error
    bool moveRelative(Axis axis, double mm);
    bool moveAbsolute(Axis axis, double mm);
    bool stop(Axis axis);           // decelerated stop
    bool abort();          // immediate stop (use for E-stop)
    bool home(Axis axis);           // searches for home input, then limits
    bool definePositionZero(Axis axis); // set current position as 0

    // State queries — return sensible defaults if not connected
    double getPosition(Axis axis);  // returns mm
    bool   isMoving(Axis axis);
    bool   isForwardLimitActive(Axis axis);
    bool   isReverseLimitActive(Axis axis);
    bool   isMotorEnabled(Axis axis);

    QString getLastError() const { return m_lastError; }

private:
    // Send a command; optionally captures the response string.
    // Returns true on G_NO_ERROR.
    bool sendCommand(const QString &cmd, QString *response = nullptr);

    // Send "MG <expr>" and parse the returned float.
    // Sets *ok = false (if provided) on failure; returns 0.0 in that case.
    double queryValue(const QString &mgExpression, bool *ok = nullptr);

    GCon    m_handle;
    bool    m_connected;
    QString m_lastError;

    char connectorFor(Axis axis) const { return axisConnectorMap.at(axis); }
    static const std::map<Axis, char> axisConnectorMap;
};
