//==============================================================================
// Autonomous Vehicle Library
//
// Description: A QObject representing a vehicle status from a status packet.
//==============================================================================

#ifndef VEHICLESTATUS_H
#define VEHICLESTATUS_H

// QObject base class
#include <QObject>

// QString class
#include <QString>

// Vehicle command packets
#include "comms/avl_commands.h"

// NAN value
#include <cmath>

//==============================================================================
//                              STRUCT DECLARATION
//==============================================================================

class VehicleStatus
{

    Q_GADGET

    // Property configuration
    Q_PROPERTY(QString comms_channel      MEMBER comms_channel)
    Q_PROPERTY(int vehicle_id             MEMBER vehicle_id)
    Q_PROPERTY(QString mode               MEMBER mode)
    Q_PROPERTY(QString operational_status MEMBER operational_status)
    Q_PROPERTY(bool whoi_synced           MEMBER whoi_synced)
    Q_PROPERTY(double roll                MEMBER roll)
    Q_PROPERTY(double pitch               MEMBER pitch)
    Q_PROPERTY(double yaw                 MEMBER yaw)
    Q_PROPERTY(double vx                  MEMBER vx)
    Q_PROPERTY(double vy                  MEMBER vy)
    Q_PROPERTY(double vz                  MEMBER vz)
    Q_PROPERTY(double lat                 MEMBER lat)
    Q_PROPERTY(double lon                 MEMBER lon)
    Q_PROPERTY(double alt                 MEMBER alt)
    Q_PROPERTY(double depth               MEMBER depth)
    Q_PROPERTY(double height              MEMBER height)
    Q_PROPERTY(double rpm                 MEMBER rpm)
    Q_PROPERTY(double voltage             MEMBER voltage)
    Q_PROPERTY(int num_gps_sats           MEMBER num_gps_sats)
    Q_PROPERTY(int iridium_strength       MEMBER iridium_strength)
    Q_PROPERTY(int current_task           MEMBER current_task)
    Q_PROPERTY(int total_tasks            MEMBER total_tasks)
    Q_PROPERTY(double task_percent        MEMBER task_percent)

public:

    // Member variables
    QString comms_channel = "RADIO";
    int vehicle_id = 0;
    QString mode = "NONE";
    QString operational_status = "NONE";
    bool whoi_synced = false;
    double roll =    std::nan("");
    double pitch =   std::nan("");
    double yaw =     std::nan("");
    double vx =      std::nan("");
    double vy =      std::nan("");
    double vz =      std::nan("");
    double lat =     std::nan("");
    double lon =     std::nan("");
    double alt =     std::nan("");
    double depth =   std::nan("");
    double height =  std::nan("");
    double rpm =     std::nan("");
    double voltage = std::nan("");
    int num_gps_sats = 0;
    int iridium_strength = 0;
    int current_task = 0;
    int total_tasks = 0;
    double task_percent = 0.0;

public:

    //--------------------------------------------------------------------------
    // Name:        VehicleStatus constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    VehicleStatus();

    //--------------------------------------------------------------------------
    // Name:        VehicleStatus constructor
    // Description: Creates a VehicleStatus class from an AVL status packet.
    // Arguments:   - packet: status packet
    //--------------------------------------------------------------------------
    VehicleStatus(avl::Packet packet);

    //--------------------------------------------------------------------------
    // Name:        VehicleStatus destructor
    // Description: Default destructor.
    //--------------------------------------------------------------------------
    virtual ~VehicleStatus();

};

Q_DECLARE_METATYPE(VehicleStatus)

#endif // VEHICLESTATUS_H
