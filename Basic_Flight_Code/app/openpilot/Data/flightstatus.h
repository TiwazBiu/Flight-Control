#ifndef FLIGHTSTATUS_H
#define FLIGHTSTATUS_H
#include <core_common.h>
struct FlightStatusData
{
    enum Armed
    {
        FLIGHTSTATUS_ARMED_DISARMED,
        FLIGHTSTATUS_ARMED_ARMING,
        FLIGHTSTATUS_ARMED_ARMED
    }Armed;
    enum FlightMode
    {
        FLIGHTSTATUS_FLIGHTMODE_MANUAL,
        FLIGHTSTATUS_FLIGHTMODE_STABILIZED1,
        FLIGHTSTATUS_FLIGHTMODE_STABILIZED2,
        FLIGHTSTATUS_FLIGHTMODE_STABILIZED3,
        FLIGHTSTATUS_FLIGHTMODE_AUTOTUNE,
        FLIGHTSTATUS_FLIGHTMODE_ALTITUDEHOLD,
        FLIGHTSTATUS_FLIGHTMODE_VELOCITYCONTROL,
        FLIGHTSTATUS_FLIGHTMODE_POSITIONHOLD,
        FLIGHTSTATUS_FLIGHTMODE_RETURNTOHOME,
        FLIGHTSTATUS_FLIGHTMODE_PATHPLANNER
    }FlightMode;
    enum ControlSource
    {
        FLIGHTSTATUS_CONTROLSOURCE_GEOFENCE,
        FLIGHTSTATUS_CONTROLSOURCE_FAILSAFE,
        FLIGHTSTATUS_CONTROLSOURCE_TRANSMITTER,
        FLIGHTSTATUS_CONTROLSOURCE_TABLET
    }ControlSource;
};
typedef struct FlightStatusData FlightStatusData;
#endif // FLIGHTSTATUS_H
