//==============================================================================
// Autonomous Vehicle Library
//
// Description: A QObject representing a vehicle. Contains a TCP connection to
// the vehicle, the vehicle's mission, and and vehicle status information.
//==============================================================================

#include "vehicle_status.h"

// Vehicle command packets
#include "comms/avl_commands.h"

// Util functions
#include "util/byte.h"
#include "util/vector.h"

// Debug output
#include <QDebug>

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//------------------------------------------------------------------------------
// Name:        VehicleStatus constructor
// Description: Default constructor.
//------------------------------------------------------------------------------
VehicleStatus::VehicleStatus()
{

}

//------------------------------------------------------------------------------
// Name:        VehicleStatus constructor
// Description: Creates a VehicleStatus class from an AVL status packet.
// Arguments:   - packet: status packet
//------------------------------------------------------------------------------
VehicleStatus::VehicleStatus(avl::Packet packet)
{

    // Attempt to parse the status packet fields
    try
    {

        // Vector to store field data bytes for parsing
        std::vector<uint8_t> field_data;

        // Parse the comms channel field
        if (packet.has_field(COMMS_CHANNEL_DESC))
        {
            field_data = packet.get_field(COMMS_CHANNEL_DESC).get_data();
            switch (field_data.at(0))
            {
                case COMMS_CHANNEL_RADIO: comms_channel = "RADIO"; break;
                case COMMS_CHANNEL_ACOMMS: comms_channel = "ACOMMS"; break;
                case COMMS_CHANNEL_IRIDIUM: comms_channel = "IRIDIUM"; break;
                default: comms_channel = "UNKNOWN";
            }
        }

        // Parse the vehicle ID field
        if (packet.has_field(VEHICLE_ID_DESC))
        {
            field_data = packet.get_field(VEHICLE_ID_DESC).get_data();
            vehicle_id = static_cast<int>(field_data.at(0));
        }

        // Parse the mode field
        if (packet.has_field(STATUS_MODE_DESC))
        {
            field_data = packet.get_field(STATUS_MODE_DESC).get_data();
            mode = QString::fromStdString(std::string(field_data.begin(), field_data.end()));
        }

        // Parse the operational status field
        if (packet.has_field(STATUS_OPERATIONAL_STATUS_DESC))
        {
            field_data = packet.get_field(STATUS_OPERATIONAL_STATUS_DESC).get_data();
            operational_status = QString::fromStdString(std::string(field_data.begin(), field_data.end()));
        }

        // Parse the micromodem synced status field
        if (packet.has_field(STATUS_UMODEM_SYNCED_DESC))
        {
            field_data = packet.get_field(STATUS_UMODEM_SYNCED_DESC).get_data();
            whoi_synced = static_cast<bool>(field_data.at(0));
        }

        // Parse the attitude field
        if (packet.has_field(STATUS_ATTITUDE_DESC))
        {
            field_data = packet.get_field(STATUS_ATTITUDE_DESC).get_data();
            roll =  avl::from_bytes<double>(avl::subvector(field_data,0,8));
            pitch = avl::from_bytes<double>(avl::subvector(field_data,8,8));
            yaw =   avl::from_bytes<double>(avl::subvector(field_data,16,8));
        }

        // Parse the velocity field
        if (packet.has_field(STATUS_VELOCITY_DESC))
        {
            field_data = packet.get_field(STATUS_VELOCITY_DESC).get_data();
            vx = avl::from_bytes<double>(avl::subvector(field_data,0,8));
            vy = avl::from_bytes<double>(avl::subvector(field_data,8,8));
            vz = avl::from_bytes<double>(avl::subvector(field_data,16,8));
        }

        // Parse the position field
        if (packet.has_field(STATUS_POSITION_DESC))
        {
            field_data = packet.get_field(STATUS_POSITION_DESC).get_data();
            lat = avl::from_bytes<double>(avl::subvector(field_data,0,8));
            lon = avl::from_bytes<double>(avl::subvector(field_data,8,8));
            alt = avl::from_bytes<double>(avl::subvector(field_data,16,8));
        }

        // Parse the depth field
        if (packet.has_field(STATUS_DEPTH_DESC))
        {
            field_data = packet.get_field(STATUS_DEPTH_DESC).get_data();
            depth = avl::from_bytes<double>(field_data);
        }

        // Parse the altitude field
        if (packet.has_field(STATUS_HEIGHT_DESC))
        {
            field_data = packet.get_field(STATUS_HEIGHT_DESC).get_data();
            height = avl::from_bytes<double>(field_data);
        }

        // Parse the rpm field
        if (packet.has_field(STATUS_RPM_DESC))
        {
            field_data = packet.get_field(STATUS_RPM_DESC).get_data();
            rpm = avl::from_bytes<double>(field_data);
        }

        // Parse the voltage field
        if (packet.has_field(STATUS_VOLTAGE_DESC))
        {
            field_data = packet.get_field(STATUS_VOLTAGE_DESC).get_data();
            voltage = avl::from_bytes<double>(field_data);
        }

        // Parse the GPS sats field
        if (packet.has_field(STATUS_GPS_SATS_DESC))
        {
            field_data = packet.get_field(STATUS_GPS_SATS_DESC).get_data();
            num_gps_sats = static_cast<int>(avl::from_bytes<uint8_t>(field_data));
        }

        // Parse the Iridium strength field
        if (packet.has_field(STATUS_GPS_SATS_DESC))
        {
            field_data = packet.get_field(STATUS_IRIDIUM_STRENGTH_DESC).get_data();
            iridium_strength = static_cast<int>(avl::from_bytes<uint8_t>(field_data));
        }

        // Parse the task field
        if (packet.has_field(STATUS_TASK_DESC))
        {
            field_data = packet.get_field(STATUS_TASK_DESC).get_data();
            current_task = static_cast<int>(avl::from_bytes<uint8_t>(avl::subvector(field_data,0,1)));
            total_tasks = static_cast<int>(avl::from_bytes<uint8_t>(avl::subvector(field_data,1,1)));
            task_percent = avl::from_bytes<double>(avl::subvector(field_data,2,8));
        }

    }
    catch (const std::exception& ex)
    {
        qDebug() << "VehicleStatus constructor: ignoring improperly formatted STATUS packet (" << ex.what() << ")";
    }
    catch (...)
    {
        qDebug() << "VehicleStatus constructor: ignoring improperly formatted STATUS packet (unknown exception)";
    }

}

//--------------------------------------------------------------------------
// Name:        VehicleStatus destructor
// Description: Default destructor.
//--------------------------------------------------------------------------
VehicleStatus::~VehicleStatus()
{

}
