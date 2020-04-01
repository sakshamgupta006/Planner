//==============================================================================
// Autonomous Vehicle Library
//
// Description: A QML accessible structure containing trajectory information.
//==============================================================================

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

// Q_GADGET macros
#include <QObject>

// NAN value
#include <cmath>

//==============================================================================
//                              STRUCT DECLARATION
//==============================================================================

struct Trajectory
{

    Q_GADGET

public:

    // Property configuration
    Q_PROPERTY(double duration MEMBER duration)
    Q_PROPERTY(double roll     MEMBER roll)
    Q_PROPERTY(double pitch    MEMBER pitch)
    Q_PROPERTY(double yaw      MEMBER yaw)
    Q_PROPERTY(double vx       MEMBER vx)
    Q_PROPERTY(double vy       MEMBER vy)
    Q_PROPERTY(double vz       MEMBER vz)
    Q_PROPERTY(double lat      MEMBER lat)
    Q_PROPERTY(double lon      MEMBER lon)
    Q_PROPERTY(double alt      MEMBER alt)
    Q_PROPERTY(double depth    MEMBER depth)
    Q_PROPERTY(double height   MEMBER height)
    Q_PROPERTY(double rpm      MEMBER rpm)
    Q_PROPERTY(bool dive       MEMBER dive)

    // Member variables
    double duration = std::nan("");
    double roll =     std::nan("");
    double pitch =    std::nan("");
    double yaw =      std::nan("");
    double vx =       std::nan("");
    double vy =       std::nan("");
    double vz =       std::nan("");
    double lat =      std::nan("");
    double lon =      std::nan("");
    double alt =      std::nan("");
    double depth =    std::nan("");
    double height =   std::nan("");
    double rpm =      std::nan("");
    bool dive = false;

};

Q_DECLARE_METATYPE(Trajectory)

#endif // TRAJECTORY_H
