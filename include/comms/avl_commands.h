//==============================================================================
// Autonomous Vehicle Library
//
// Description: Defines a mapping of packet descriptors and field descriptors
//              which implement the AVL binary communication protocol. Also
//              provides helper functions for creating packets.
//==============================================================================

#ifndef AVL_COMMANDS_H
#define AVL_COMMANDS_H

// Core includes
#include <comms/field.h>
#include <comms/packet.h>
#include <util/byte.h>

//==============================================================================
//                              AVL MODE MAPPING
//==============================================================================

const uint8_t MODE_MANUAL =     0x00;
const uint8_t MODE_AUTONOMOUS = 0x01;

//==============================================================================
//                           COMMS CHANNEL MAPPING
//==============================================================================

const uint8_t COMMS_CHANNEL_RADIO =   0x00;
const uint8_t COMMS_CHANNEL_ACOMMS =  0x01;
const uint8_t COMMS_CHANNEL_IRIDIUM = 0x02;

//==============================================================================
//                             TASK TYPES MAPPING
//==============================================================================

const uint8_t TASK_TYPE_PRIMITIVE = 0x00;
const uint8_t TASK_TYPE_WAYPOINT =  0x01;
const uint8_t TASK_TYPE_PATH =      0x02;
const uint8_t TASK_TYPE_ZONE =      0x03;

//==============================================================================
//                            AVL COMMAND MAPPING
//==============================================================================

// Packet types
const uint8_t RESPONSE_PACKET_DESC =            0x00;
const uint8_t STATUS_PACKET_DESC =              0x01;
const uint8_t ACTION_PACKET_DESC =              0x02;
const uint8_t HELM_PACKET_DESC =                0x03;
const uint8_t ACOUSTIC_PING_PACKET_DESC =       0x04;
const uint8_t MISSION_PACKET_DESC =             0x05;
const uint8_t TASK_PACKET_DESC =                0x07;
const uint8_t PARAMETER_PACKET_DESC =           0x08;
const uint8_t PARAMETER_LIST_PACKET_DESC =      0x09;

// Global packet field descriptors
const uint8_t COMMS_CHANNEL_DESC = 0xFE;
const uint8_t VEHICLE_ID_DESC = 0xFF;

// RESPONSE packet field descriptors
const uint8_t RESPONSE_PACKET_DESCRIPTOR_DESC = 0x00;
const uint8_t RESPONSE_FIELD_DESCRIPTOR_DESC =  0x01;
const uint8_t RESPONSE_DATA_DESC =              0x02;

// STATUS packet field descriptors
const uint8_t STATUS_MODE_DESC =               0x00;
const uint8_t STATUS_OPERATIONAL_STATUS_DESC = 0x01;
const uint8_t STATUS_ATTITUDE_DESC =           0x02;
const uint8_t STATUS_VELOCITY_DESC =           0x03;
const uint8_t STATUS_POSITION_DESC =           0x04;
const uint8_t STATUS_DEPTH_DESC =              0x05;
const uint8_t STATUS_HEIGHT_DESC =             0x06;
const uint8_t STATUS_RPM_DESC =                0x07;
const uint8_t STATUS_VOLTAGE_DESC =            0x08;
const uint8_t STATUS_MAG_FLUX_DESC =           0x09;
const uint8_t STATUS_UMODEM_SYNCED_DESC =      0x0A;
const uint8_t STATUS_GPS_SATS_DESC =           0x0B;
const uint8_t STATUS_IRIDIUM_STRENGTH_DESC =   0x0C;
const uint8_t STATUS_TASK_DESC =               0x0D;

// ACTION packet field descriptors
const uint8_t ACTION_PING_DESC =                     0x00;
const uint8_t ACTION_EMERGENCY_STOP_DESC =           0x01;
const uint8_t ACTION_POWER_CYCLE_DESC =              0x02;
const uint8_t ACTION_RESTART_ROS_DESC =              0x03;
const uint8_t ACTION_RESET_SAFETY_DESC =             0x04;
const uint8_t ACTION_SET_MODE_DESC =                 0x05;
const uint8_t ACTION_SET_MAG_STREAM_DESC =           0x06;
const uint8_t ACTION_SET_MAG_CAL_DESC =              0x07;
const uint8_t ACTION_TARE_PRESSURE_DESC =            0x08;
const uint8_t ACTION_START_LBL_PINGS_DESC =          0x09;
const uint8_t ACTION_START_OWTT_PINGS_DESC =         0x0A;
const uint8_t ACTION_STOP_ACOUSTIC_PINGS_DESC =      0x0B;
const uint8_t ACTION_ENABLE_BACK_SEAT_DRIVER_DESC =  0x0C;
const uint8_t ACTION_DISABLE_BACK_SEAT_DRIVER_DESC = 0x0D;
const uint8_t ACTION_SET_GEOFENCE_DESC =             0x0E;
const uint8_t ACTION_ENABLE_STROBE_DESC =            0x0F;
const uint8_t ACTION_DISABLE_STROBE_DESC =           0x10;
const uint8_t ACTION_ENABLE_SONAR_DESC =             0x11;
const uint8_t ACTION_DISABLE_SONAR_DESC =            0x12;
const uint8_t ACTION_START_SONAR_RECORDING_DESC =    0x13;
const uint8_t ACTION_STOP_SONAR_RECORDING_DESC =     0x14;


// MISSION packet field descriptors
const uint8_t MISSION_START_DESC =        0x01;
const uint8_t MISSION_STOP_DESC =         0x02;
const uint8_t MISSION_CLEAR_DESC =        0x03;
const uint8_t MISSION_ADVANCE_DESC =      0x04;
const uint8_t MISSION_SET_DESC =          0x05;
const uint8_t MISSION_APPEND_DESC =       0x06;
const uint8_t MISSION_READ_CURRENT_DESC = 0x07;
const uint8_t MISSION_READ_ALL_DESC =     0x08;

// TASK field descriptors
const uint8_t TASK_DURATION_DESC = 0x00;
const uint8_t TASK_TYPE_DESC =     0x01;
const uint8_t TASK_ATTITUDE_DESC = 0x02;
const uint8_t TASK_VELOCITY_DESC = 0x03;
const uint8_t TASK_DEPTH_DESC =    0x04;
const uint8_t TASK_HEIGHT_DESC =   0x05;
const uint8_t TASK_RPM_DESC =      0x06;
const uint8_t TASK_DIVE_DESC =     0x07;
const uint8_t TASK_POINTS_DESC =   0x08;
const uint8_t TASK_COMMAND_DESC =  0x09;

// HELM packet field descriptors
const uint8_t HELM_THROTTLE_DESC = 0x00;
const uint8_t HELM_RUDDER_DESC =   0x01;
const uint8_t HELM_ELEVATOR_DESC = 0x02;

// ACOUSTIC_PING packet field descriptors
const uint8_t ACOUSTIC_PING_DEPARTURE_TIME_DESC =  0x00;
const uint8_t ACOUSTIC_PING_ORIGIN_POSITION_DESC = 0x01;

// PARAMETER packet field descriptors
const uint8_t PARAMETER_NAME_DESC =    0x00;
const uint8_t PARAMETER_VALUE_DESC =   0x01;
const uint8_t PARAMETER_TYPE_DESC =    0x02;

// PARAMETER_LIST packet field descriptors
const uint8_t PARAMETER_LIST_DESC =         0x00;
const uint8_t PARAMETER_LIST_REQUEST_DESC = 0x01;
const uint8_t PARAMETER_LIST_SIZE_DESC =    0x02;

//==============================================================================
//                            FUNCTION DECLARATIONS
//==============================================================================

// Packet creation helper functions
avl::Packet RESPONSE_PACKET();
avl::Packet STATUS_PACKET();
avl::Packet ACTION_PACKET();
avl::Packet MISSION_PACKET();
avl::Packet TASK_PACKET();
avl::Packet HELM_PACKET();
avl::Packet ACOUSTIC_PING_PACKET();
avl::Packet PARAMETER_PACKET();
avl::Packet PARAMETER_LIST_PACKET();

// Global packet field creation helper functions
avl::Field COMMS_CHANNEL(uint8_t channel);
avl::Field VEHICLE_ID(uint8_t id);

// RESPONSE packet field creation helper functions
avl::Field RESPONSE_PACKET_DESCRIPTOR(uint8_t packet_descriptor);
avl::Field RESPONSE_FIELD_DESCRIPTOR(uint8_t field_descriptor);
avl::Field RESPONSE_DATA(std::vector<uint8_t> data);

// STATUS packet field creation helper functions
avl::Field STATUS_MODE(std::string mode);
avl::Field STATUS_OPERATIONAL_STATUS(std::string operational_status);
avl::Field STATUS_ATTITUDE(double roll, double pitch, double yaw);
avl::Field STATUS_VELOCITY(double vx, double vy, double vz);
avl::Field STATUS_POSITION(double lat, double lon, double alt);
avl::Field STATUS_DEPTH(double depth);
avl::Field STATUS_HEIGHT(double height);
avl::Field STATUS_RPM(double rpm);
avl::Field STATUS_VOLTAGE(double voltage);
avl::Field STATUS_MAG_FLUX(double mx, double my, double mz);
avl::Field STATUS_UMODEM_SYNCED(bool synced);
avl::Field STATUS_GPS_SATS(uint8_t num_sats);
avl::Field STATUS_IRIDIUM_STRENGTH(uint8_t strength);

// ACTION packet field creation helper functions
avl::Field ACTION_PING();
avl::Field ACTION_EMERGENCY_STOP();
avl::Field ACTION_POWER_CYCLE();
avl::Field ACTION_RESTART_ROS();
avl::Field ACTION_RESET_SAFETY();
avl::Field ACTION_SET_MODE(std::string mode);
avl::Field ACTION_SET_MAG_STREAM(bool enable);
avl::Field ACTION_SET_MAG_CAL(std::vector<double> A, std::vector<double> b);
avl::Field ACTION_TARE_PRESSURE();
avl::Field ACTION_START_LBL_PINGS();
avl::Field ACTION_START_OWTT_PINGS();
avl::Field ACTION_STOP_ACOUSTIC_PINGS();
avl::Field ACTION_ENABLE_BACK_SEAT_DRIVER();
avl::Field ACTION_DISABLE_BACK_SEAT_DRIVER();
avl::Field ACTION_SET_GEOFENCE(std::vector<double> lats, std::vector<double> lons);
avl::Field ACTION_ENABLE_STROBE();
avl::Field ACTION_DISABLE_STROBE();
avl::Field ACTION_ENABLE_SONAR();
avl::Field ACTION_DISABLE_SONAR();
avl::Field ACTION_START_SONAR_RECORDING();
avl::Field ACTION_STOP_SONAR_RECORDING();

// MISSION packet field creation helper functions
avl::Field MISSION_START();
avl::Field MISSION_STOP();
avl::Field MISSION_CLEAR();
avl::Field MISSION_ADVANCE();
avl::Field MISSION_SET(avl::Packet task);
avl::Field MISSION_APPEND(std::vector<avl::Packet> tasks);
avl::Field MISSION_READ_CURRENT();
avl::Field MISSION_READ_ALL();

// TASK packet field creation helper functions
avl::Field TASK_DURATION(double duration);
avl::Field TASK_TYPE(uint8_t type);
avl::Field TASK_ATTITUDE(double roll, double pitch, double yaw);
avl::Field TASK_VELOCITY(double vx, double vy, double vz);
avl::Field TASK_DEPTH(double depth);
avl::Field TASK_HEIGHT(double height);
avl::Field TASK_RPM(double rpm);
avl::Field TASK_DIVE(bool dive);
avl::Field TASK_POINTS(std::vector<double> points);
avl::Field TASK_COMMAND(uint8_t command);

// HELM packet field creation helper functions
avl::Field HELM_THROTTLE(double percent);
avl::Field HELM_RUDDER(double angle);
avl::Field HELM_ELEVATOR(double angle);

// ACOUSTIC_PING packet field creation helper functions
avl::Field ACOUSTIC_PING_DEPARTURE_TIME(double t);
avl::Field ACOUSTIC_PING_ORIGIN_POSITION(double lat, double lon, double alt);

// PARAMETER packet field descriptors
avl::Field PARAMETER_NAME(std::string name);
template<typename T>
inline avl::Field PARAMETER_VALUE(T value)
{
    std::vector<uint8_t> payload = avl::to_bytes(value);
    return avl::Field(PARAMETER_VALUE_DESC, payload);
}
inline avl::Field PARAMETER_VALUE(std::string value)
{
    std::vector<uint8_t> string_vector(value.begin(), value.end());
    return avl::Field(PARAMETER_VALUE_DESC, string_vector);
}
avl::Field PARAMETER_TYPE(std::string type);

// PARAMETER_LIST packet field descriptors
avl::Field PARAMETER_LIST(std::vector<avl::Packet> parameters);
avl::Field PARAMETER_LIST_REQUEST();
avl::Field PARAMETER_LIST_SIZE(int size);

#endif // AVL_COMMANDS_H
