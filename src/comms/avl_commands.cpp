//==============================================================================
// Autonomous Vehicle Library
//
// Description: Defines a mapping of packet descriptors and field descriptors
//              which implement the AVL binary communication protocol. Also
//              provides helper functions for creating packets.
//==============================================================================

// Core includes
#include <comms/avl_commands.h>
#include <comms/field.h>
#include <comms/packet.h>
#include <util/vector.h>
#include <util/byte.h>

using namespace avl;

//==============================================================================
//                            FUNCTION DEFINITIONS
//==============================================================================

//------------------------------------------------------------------------------
// Name:        RESPONSE_PACKET
// Description: Creates an empty RESPONSE packet.
// Returns:     RESPONSE packet.
//------------------------------------------------------------------------------
Packet RESPONSE_PACKET()
{
    Packet packet;
    packet.set_descriptor(RESPONSE_PACKET_DESC);
    return packet;
}

//------------------------------------------------------------------------------
// Name:        STATUS_PACKET
// Description: Creates an empty STATUS packet.
// Returns:     STATUS packet.
//------------------------------------------------------------------------------
Packet STATUS_PACKET()
{
    Packet packet;
    packet.set_descriptor(STATUS_PACKET_DESC);
    return packet;
}

//------------------------------------------------------------------------------
// Name:        ACTION_PACKET
// Description: Creates an empty ACTION packet.
// Returns:     ACTION packet.
//------------------------------------------------------------------------------
Packet ACTION_PACKET()
{
    Packet packet;
    packet.set_descriptor(ACTION_PACKET_DESC);
    return packet;
}

//------------------------------------------------------------------------------
// Name:        MISSION_PACKET
// Description: Creates an empty MISSION packet.
// Returns:     MISSION packet.
//------------------------------------------------------------------------------
Packet MISSION_PACKET()
{
    Packet packet;
    packet.set_descriptor(MISSION_PACKET_DESC);
    return packet;
}

//------------------------------------------------------------------------------
// Name:        TASK_PACKET
// Description: Creates an empty TASK packet.
// Returns:     TASK packet.
//------------------------------------------------------------------------------
Packet TASK_PACKET()
{
    Packet packet;
    packet.set_descriptor(TASK_PACKET_DESC);
    return packet;
}

//------------------------------------------------------------------------------
// Name:        HELM_PACKET
// Description: Creates an empty HELM packet.
// Returns:     HELM packet.
//------------------------------------------------------------------------------
Packet HELM_PACKET()
{
    Packet packet;
    packet.set_descriptor(HELM_PACKET_DESC);
    return packet;
}

//------------------------------------------------------------------------------
// Name:        ACOUSTIC_PING_PACKET
// Description: Creates an empty ACOUSTIC_PING packet.
// Returns:     ACOUSTIC_PING packet.
//------------------------------------------------------------------------------
Packet ACOUSTIC_PING_PACKET()
{
    Packet packet;
    packet.set_descriptor(ACOUSTIC_PING_PACKET_DESC);
    return packet;
}

//------------------------------------------------------------------------------
// Name:        PARAMETER_PACKET
// Description: Creates an empty PARAMETER packet.
// Returns:     PARAMETER packet.
//------------------------------------------------------------------------------
Packet PARAMETER_PACKET()
{
    Packet packet;
    packet.set_descriptor(PARAMETER_PACKET_DESC);
    return packet;
}

//------------------------------------------------------------------------------
// Name:        PARAMETER_LIST_PACKET
// Description: Creates an empty PARAMETER_LIST packet.
// Returns:     PARAMETER_LIST packet.
//------------------------------------------------------------------------------
Packet PARAMETER_LIST_PACKET()
{
    Packet packet;
    packet.set_descriptor(PARAMETER_LIST_PACKET_DESC);
    return packet;
}

//------------------------------------------------------------------------------
// Name:        COMMS_CHANNEL
// Description: Creates a COMMS_CHANNEL field.
// Returns:     COMMS_CHANNEL field.
//------------------------------------------------------------------------------
avl::Field COMMS_CHANNEL(uint8_t channel)
{
    return avl::Field(COMMS_CHANNEL_DESC, {channel});
}


//------------------------------------------------------------------------------
// Name:        VEHICLE_ID
// Description: Creates a VEHICLE_ID field.
// Returns:     VEHICLE_ID field.
//------------------------------------------------------------------------------
Field VEHICLE_ID(uint8_t id)
{
    return Field(VEHICLE_ID_DESC, {id});
}

//------------------------------------------------------------------------------
// Name:        RESPONSE_PACKET_DESCRIPTOR
// Description: Creates a RESPONSE packet PACKET_DESCRIPTOR field.
// Arguments:   - packet_descriptor: packet descriptor of the packet being
//                responded to
// Returns:     RESPONSE packet PACKET_DESCRIPTOR field.
//------------------------------------------------------------------------------
Field RESPONSE_PACKET_DESCRIPTOR(uint8_t packet_descriptor)
{
    return Field(RESPONSE_PACKET_DESCRIPTOR_DESC, avl::to_bytes(packet_descriptor));
}

//------------------------------------------------------------------------------
// Name:        RESPONSE_FIELD_DESCRIPTOR
// Description: Creates a RESPONSE packet FIELD_DESCRIPTOR field.
// Arguments:   - packet_field_descriptor: field descriptor of the packet being
//                responded to
// Returns:     RESPONSE packet FIELD_DESCRIPTOR field.
//------------------------------------------------------------------------------
Field RESPONSE_FIELD_DESCRIPTOR(uint8_t field_descriptor)
{
    return Field(RESPONSE_FIELD_DESCRIPTOR_DESC, avl::to_bytes(field_descriptor));
}

//------------------------------------------------------------------------------
// Name:        RESPONSE_DATA
// Description: Creates a RESPONSE packet DATA field.
// Arguments:   - data: response data
// Returns:     RESPONSE packet DATA field.
//------------------------------------------------------------------------------
Field RESPONSE_DATA(std::vector<uint8_t> data)
{
    return Field(RESPONSE_DATA_DESC, data);
}

//------------------------------------------------------------------------------
// Name:        STATUS_MODE
// Description: Creates a STATUS packet MODE field.
// Arguments:   - mode: mode string
// Returns:     STATUS packet MODE field.
//------------------------------------------------------------------------------
Field STATUS_MODE(std::string mode)
{
    std::vector<uint8_t> string_vector(mode.begin(), mode.end());
    return Field(STATUS_MODE_DESC, string_vector);
}

//------------------------------------------------------------------------------
// Name:        STATUS_OPERATIONAL_STATUS
// Description: Creates a STATUS packet OPERATIONAL_STATUS field.
// Arguments:   - operational_status: operational status string
// Returns:     STATUS packet OPERATIONAL_STATUS field.
//------------------------------------------------------------------------------
Field STATUS_OPERATIONAL_STATUS(std::string operational_status)
{
    std::vector<uint8_t> string_vector(operational_status.begin(),
                                       operational_status.end());
    return Field(STATUS_OPERATIONAL_STATUS_DESC, string_vector);
}

//------------------------------------------------------------------------------
// Name:        STATUS_ATTITUDE
// Description: Creates a STATUS packet ATTITUDE field.
// Arguments:   - roll: roll angle in degrees
//              - pitch: pitch angle in degrees
//              - yaw: yaw angle in degrees
// Returns:     STATUS packet ATTITUDE field.
//------------------------------------------------------------------------------
Field STATUS_ATTITUDE(double roll, double pitch, double yaw)
{
    std::vector<uint8_t> payload = avl::to_bytes(roll);
    avl::append(payload, avl::to_bytes(pitch));
    avl::append(payload, avl::to_bytes(yaw));
    return Field(STATUS_ATTITUDE_DESC, payload);
}

//------------------------------------------------------------------------------
// Name:        STATUS_VELOCITY
// Description: Creates a STATUS packet VELOCITY field.
// Arguments:   - vx: x-axis velocity in m/s
//              - vy: y-axis velocity in m/s
//              - vz: z-axis velocity in m/s
// Returns:     STATUS packet VELOCITY field.
//------------------------------------------------------------------------------
Field STATUS_VELOCITY(double vx, double vy, double vz)
{
    std::vector<uint8_t> payload = avl::to_bytes(vx);
    avl::append(payload, avl::to_bytes(vy));
    avl::append(payload, avl::to_bytes(vz));
    return Field(STATUS_VELOCITY_DESC, payload);
}

//------------------------------------------------------------------------------
// Name:        STATUS_POSITION
// Description: Creates a STATUS packet POSITION field.
// Arguments:   - lat: latitude in degrees
//              - lon: longitude in degrees
//              - alt: altitude in meters
// Returns:     STATUS packet POSITION field.
//------------------------------------------------------------------------------
Field STATUS_POSITION(double lat, double lon, double alt)
{
    std::vector<uint8_t> payload = avl::to_bytes(lat);
    avl::append(payload, avl::to_bytes(lon));
    avl::append(payload, avl::to_bytes(alt));
    return Field(STATUS_POSITION_DESC, payload);
}

//------------------------------------------------------------------------------
// Name:        STATUS_DEPTH
// Description: Creates a STATUS packet DEPTH field.
// Arguments:   - depth: depth in meters
// Returns:     STATUS packet DEPTH field.
//------------------------------------------------------------------------------
Field STATUS_DEPTH(double depth)
{
    return Field(STATUS_DEPTH_DESC, avl::to_bytes(depth));
}

//------------------------------------------------------------------------------
// Name:        STATUS_HEIGHT
// Description: Creates a STATUS packet HEIGHT field.
// Arguments:   - height: height in meters
// Returns:     STATUS packet HEIGHT field.
//------------------------------------------------------------------------------
Field STATUS_HEIGHT(double height)
{
    return Field(STATUS_HEIGHT_DESC, avl::to_bytes(height));
}

//------------------------------------------------------------------------------
// Name:        STATUS_RPM
// Description: Creates a STATUS packet RPM field.
// Arguments:   - rpm: rotational rate in RPM
// Returns:     STATUS packet RPM field.
//------------------------------------------------------------------------------
Field STATUS_RPM(double rpm)
{
    return Field(STATUS_RPM_DESC, avl::to_bytes(rpm));
}

//------------------------------------------------------------------------------
// Name:        STATUS_VOLTAGE
// Description: Creates a STATUS packet VOLTAGE field.
// Arguments:   - voltage: voltage in volts
// Returns:     STATUS packet VOLTAGE field.
//------------------------------------------------------------------------------
Field STATUS_VOLTAGE(double voltage)
{
    return Field(STATUS_VOLTAGE_DESC, avl::to_bytes(voltage));
}

//------------------------------------------------------------------------------
// Name:        STATUS_MAG_FLUX
// Description: Creates a STATUS packet MAG_FLUX field.
// Arguments:   - mx: x-axis magnetic flux in Gauss
//              - my: y-axis magnetic flux in Gauss
//              - mz: z-axis magnetic flux in Gauss
// Returns:     STATUS packet MAG_FLUX field.
//------------------------------------------------------------------------------
Field STATUS_MAG_FLUX(double mx, double my, double mz)
{
    std::vector<uint8_t> payload = avl::to_bytes(mx);
    avl::append(payload, avl::to_bytes(my));
    avl::append(payload, avl::to_bytes(mz));
    return Field(STATUS_MAG_FLUX_DESC, payload);
}

//------------------------------------------------------------------------------
// Name:        STATUS_UMODEM_SYNCED
// Description: Creates a STATUS packet UMODEM_SYNCED field.
// Arguments:   - synced: true if synced, false if not synced
// Returns:     STATUS packet UMODEM_SYNCED field.
//------------------------------------------------------------------------------
Field STATUS_UMODEM_SYNCED(bool synced)
{
    return Field(STATUS_UMODEM_SYNCED_DESC, avl::to_bytes(synced));
}

//------------------------------------------------------------------------------
// Name:        STATUS_GPS_SATS
// Description: Creates a STATUS packet GPS_SATS field.
// Arguments:   - num_sats: number of GPS sats in use
// Returns:     STATUS packet GPS_SATS field.
//------------------------------------------------------------------------------
Field STATUS_GPS_SATS(uint8_t num_sats)
{
    return Field(STATUS_GPS_SATS_DESC, {num_sats});
}

//------------------------------------------------------------------------------
// Name:        STATUS_IRIDIUM_STRENGTH
// Description: Creates a STATUS packet IRIDIUM_STRENGTH field.
// Arguments:   - strength: Iridium signal strength in bars
// Returns:     STATUS packet IRIDIUM_STRENGTH field.
//------------------------------------------------------------------------------
Field STATUS_IRIDIUM_STRENGTH(uint8_t strength)
{
    return Field(STATUS_IRIDIUM_STRENGTH_DESC, {strength});
}

//------------------------------------------------------------------------------
// Name:        STATUS_TASK
// Description: Creates a STATUS packet TASK field.
// Arguments:   - task_num: task number current being executed
//              - num_tasks: total number of tasks to execute
//              - percent: task completion percentage
// Returns:     STATUS packet TASK_PERCENT field.
//------------------------------------------------------------------------------
Field STATUS_TASK(uint8_t task_num, uint8_t num_tasks, double percent)
{
    std::vector<uint8_t> payload = {task_num, num_tasks};
    avl::append(payload, avl::to_bytes(percent));
    return Field(STATUS_IRIDIUM_STRENGTH_DESC, payload);
}

//------------------------------------------------------------------------------
// Name:        ACTION_PING
// Description: Creates an ACTION packet PING field.
// Returns:     ACTION packet PING field.
//------------------------------------------------------------------------------
Field ACTION_PING()
{
    return Field(ACTION_PING_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_EMERGENCY_STOP
// Description: Creates an ACTION packet EMERGENCY_STOP field.
// Returns:     ACTION packet EMERGENCY_STOP field.
//------------------------------------------------------------------------------
Field ACTION_EMERGENCY_STOP()
{
    return Field(ACTION_EMERGENCY_STOP_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_POWER_CYCLE
// Description: Creates an ACTION packet POWER_CYCLE field.
// Returns:     ACTION packet POWER_CYCLE field.
//------------------------------------------------------------------------------
Field ACTION_POWER_CYCLE()
{
    return Field(ACTION_POWER_CYCLE_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_RESTART_ROS
// Description: Creates an ACTION packet RESTART_ROS field.
// Returns:     ACTION packet RESTART_ROS field.
//------------------------------------------------------------------------------
Field ACTION_RESTART_ROS()
{
    return Field(ACTION_RESTART_ROS_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_RESET_SAFETY
// Description: Creates an ACTION packet RESET_SAFETY field.
// Returns:     ACTION packet RESET_SAFETY field.
//------------------------------------------------------------------------------
Field ACTION_RESET_SAFETY()
{
    return Field(ACTION_RESET_SAFETY_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_SET_MODE
// Description: Creates an ACTION packet SET_MODE field.
// Arguments:   - mode: mode to put vehicle in
// Returns:     ACTION packet SET_MODE field.
//------------------------------------------------------------------------------
Field ACTION_SET_MODE(std::string mode)
{
    std::vector<uint8_t> string_vector(mode.begin(), mode.end());
    return Field(ACTION_SET_MODE_DESC, string_vector);
}

//------------------------------------------------------------------------------
// Name:        ACTION_SET_MAG_STREAM
// Description: Creates an ACTION packet SET_MAG_STREAM field.
// Returns:     ACTION packet SET_MAG_STREAM field.
//------------------------------------------------------------------------------
Field ACTION_SET_MAG_STREAM(bool enable)
{
    return Field(ACTION_SET_MAG_STREAM_DESC, {(uint8_t)enable});
}

//------------------------------------------------------------------------------
// Name:        ACTION_SET_MAG_CAL
// Description: Creates an ACTION packet SET_MAG_CAL field.
// Arguments:   - A: 9 row-major elements of the soft iron calibration matrix
//              - b: 3 elements of the hard iron calibration vector
// Returns:     ACTION packet SET_MAG_CAL field.
//------------------------------------------------------------------------------
Field ACTION_SET_MAG_CAL(std::vector<double> A, std::vector<double> b)
{
    avl::append(A, b);
    std::vector<uint8_t> payload;
    for (size_t i = 0; i < A.size(); i++)
        avl::append(payload, avl::to_bytes(A.at(i)));
    return Field(ACTION_SET_MAG_CAL_DESC, payload);
}

//------------------------------------------------------------------------------
// Name:        ACTION_TARE_PRESSURE
// Description: Creates an ACTION packet TARE_PRESSURE field.
// Returns:     ACTION packet TARE_PRESSURE field.
//------------------------------------------------------------------------------
Field ACTION_TARE_PRESSURE()
{
    return Field(ACTION_TARE_PRESSURE_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_START_LBL_PINGS
// Description: Creates an ACTION packet START_LBL_PINGS field.
// Returns:     ACTION packet START_LBL_PINGS field.
//------------------------------------------------------------------------------
Field ACTION_START_LBL_PINGS()
{
    return Field(ACTION_START_LBL_PINGS_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_START_OWTT_PINGS
// Description: Creates an ACTION packet START_OWTT_PINGS field.
// Returns:     ACTION packet START_OWTT_PINGS field.
//------------------------------------------------------------------------------
Field ACTION_START_OWTT_PINGS()
{
    return Field(ACTION_START_OWTT_PINGS_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_STOP_ACOUSTIC_PINGS
// Description: Creates an ACTION packet STOP_ACOUSTIC_PINGS field.
// Returns:     ACTION packet STOP_ACOUSTIC_PINGS field.
//------------------------------------------------------------------------------
Field ACTION_STOP_ACOUSTIC_PINGS()
{
    return Field(ACTION_STOP_ACOUSTIC_PINGS_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_ENABLE_BACK_SEAT_DRIVER
// Description: Creates an ACTION packet ACTION_ENABLE_BACK_SEAT_DRIVER field.
// Returns:     ACTION packet ACTION_ENABLE_BACK_SEAT_DRIVER field.
//------------------------------------------------------------------------------
Field ACTION_ENABLE_BACK_SEAT_DRIVER()
{
    return Field(ACTION_ENABLE_BACK_SEAT_DRIVER_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_DISABLE_BACK_SEAT_DRIVER
// Description: Creates an ACTION packet ACTION_DISABLE_BACK_SEAT_DRIVER field.
// Returns:     ACTION packet ACTION_DISABLE_BACK_SEAT_DRIVER field.
//------------------------------------------------------------------------------
Field ACTION_DISABLE_BACK_SEAT_DRIVER()
{
    return Field(ACTION_DISABLE_BACK_SEAT_DRIVER_DESC);
}

//------------------------------------------------------------------------------
// Name:         ACTION_SET_GEOFENCE
// Description: Creates an ACTION packet ACTION_SET_GEOFENCE field.
// Returns:     ACTION packet ACTION_SET_GEOFENCE field.
//------------------------------------------------------------------------------
Field ACTION_SET_GEOFENCE(std::vector<double> lats, std::vector<double> lons)
{
    std::vector<uint8_t> payload;
    for (size_t i =0; i<lats.size(); i++)
    {
      avl::append(payload, avl::to_bytes(lats.at(i)));
      avl::append(payload, avl::to_bytes(lons.at(i)));
    }
    return Field(ACTION_SET_GEOFENCE_DESC, payload);

}

//------------------------------------------------------------------------------
// Name:        ACTION_ENABLE_STROBE
// Description: Creates an ACTION packet ACTION_ENABLE_STROBE field.
// Returns:     ACTION packet ACTION_ENABLE_STROBE field.
//------------------------------------------------------------------------------
Field ACTION_ENABLE_STROBE()
{
    return Field(ACTION_ENABLE_STROBE_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_DISABLE_STROBE
// Description: Creates an ACTION packet ACTION_DISABLE_STROBE field.
// Returns:     ACTION packet ACTION_DISABLE_STROBE field.
//------------------------------------------------------------------------------
Field ACTION_DISABLE_STROBE()
{
    return Field(ACTION_DISABLE_STROBE_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_ENABLE_SONAR
// Description: Creates an ACTION packet ENABLE_SONAR field.
// Returns:     ACTION packet ENABLE_SONAR field.
//------------------------------------------------------------------------------
Field ACTION_ENABLE_SONAR()
{
    return Field(ACTION_ENABLE_SONAR_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_DISABLE_SONAR
// Description: Creates an ACTION packet DISABLE_SONAR field.
// Returns:     ACTION packet DISABLE_SONAR field.
//------------------------------------------------------------------------------
Field ACTION_DISABLE_SONAR()
{
    return Field(ACTION_DISABLE_SONAR_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_START_SONAR_RECORDING
// Description: Creates an ACTION packet START_SONAR_RECORDING field.
// Returns:     ACTION packet START_SONAR_RECORDING field.
//------------------------------------------------------------------------------
Field ACTION_START_SONAR_RECORDING()
{
    return Field(ACTION_START_SONAR_RECORDING_DESC);
}

//------------------------------------------------------------------------------
// Name:        ACTION_STOP_SONAR_RECORDING
// Description: Creates an ACTION packet STOP_SONAR_RECORDING field.
// Returns:     ACTION packet STOP_SONAR_RECORDING field.
//------------------------------------------------------------------------------
Field ACTION_STOP_SONAR_RECORDING()
{
    return Field(ACTION_STOP_SONAR_RECORDING_DESC);
}

//------------------------------------------------------------------------------
// Name:        MISSION_START
// Description: Creates a MISSION packet START field.
// Returns:     MISSION packet START field.
//------------------------------------------------------------------------------
Field MISSION_START()
{
    return Field(MISSION_START_DESC);
}

//------------------------------------------------------------------------------
// Name:        MISSION_STOP
// Description: Creates a MISSION packet STOP field.
// Returns:     MISSION packet STOP field.
//------------------------------------------------------------------------------
Field MISSION_STOP()
{
    return Field(MISSION_STOP_DESC);
}

//------------------------------------------------------------------------------
// Name:        MISSION_CLEAR
// Description: Creates a MISSION packet CLEAR field.
// Returns:     MISSION packet CLEAR field.
//------------------------------------------------------------------------------
Field MISSION_CLEAR()
{
    return Field(MISSION_CLEAR_DESC);
}

//------------------------------------------------------------------------------
// Name:        MISSION_ADVANCE
// Description: Creates a MISSION packet ADVANCE field.
// Returns:     MISSION packet ADVANCE field.
//------------------------------------------------------------------------------
Field MISSION_ADVANCE()
{
    return Field(MISSION_ADVANCE_DESC);
}

//------------------------------------------------------------------------------
// Name:        MISSION_SET
// Description: Creates a MISSION packet SET field.
// Arguments:   - task_packet: task packet containing the task to set.
// Returns:     MISSION packet SET field.
//------------------------------------------------------------------------------
Field MISSION_SET(Packet task)
{
    return Field(MISSION_SET_DESC, task.get_bytes());
}

//------------------------------------------------------------------------------
// Name:        MISSION_APPEND
// Description: Creates a MISSION packet APPEND field.
// Arguments:   - tasks: vector of tasks to be appended
// Returns:     MISSION packet APPEND field.
//------------------------------------------------------------------------------
Field MISSION_APPEND(std::vector<Packet> tasks)
{
    std::vector<uint8_t> payload;
    for(int i = 0; i < tasks.size(); i++)
        avl::append(payload, tasks.at(i).get_bytes());
    return Field(MISSION_APPEND_DESC, payload);
}


//------------------------------------------------------------------------------
// Name:        MISSION_READ_CURRENT
// Description: Creates a MISSION packet READ_CURRENT field.
// Returns:     MISSION packet READ_CURRENT field.
//------------------------------------------------------------------------------
Field MISSION_READ_CURRENT()
{
    return Field(MISSION_READ_CURRENT_DESC);
}

//------------------------------------------------------------------------------
// Name:        MISSION_READ_ALL
// Description: Creates a MISSION packet READ_ALL field.
// Returns:     MISSION packet READ_ALL field.
//------------------------------------------------------------------------------
Field MISSION_READ_ALL()
{
    return Field(MISSION_READ_ALL_DESC);
}

//------------------------------------------------------------------------------
// Name:        TASK_DURATION
// Description: Creates a TASK packet DURATION field.
// Arguments:   - duration: mission duration in seconds
// Returns:     TASK packet DURATION field.
//------------------------------------------------------------------------------
Field TASK_DURATION(double duration)
{
    return Field(TASK_DURATION_DESC, avl::to_bytes(duration));
}

//------------------------------------------------------------------------------
// Name:        TASK_TYPE
// Description: Creates a TASK packet TYPE field.
// Arguments:   - type: Task type descriptor
// Returns:     TASK packet TYPE field.
//------------------------------------------------------------------------------
Field TASK_TYPE(uint8_t type)
{
    return Field(TASK_TYPE_DESC, avl::to_bytes(type));
}

//------------------------------------------------------------------------------
// Name:        TASK_ATTITUDE
// Description: Creates a TASK packet ATTITUDE field.
// Arguments:   - roll: roll angle in degrees
//              - pitch: pitch angle in degrees
//              - yaw: yaw angle in degrees
// Returns:     TASK packet ATTITUDE field.
//------------------------------------------------------------------------------
Field TASK_ATTITUDE(double roll, double pitch, double yaw)
{
    std::vector<uint8_t> payload = avl::to_bytes(roll);
    avl::append(payload, avl::to_bytes(pitch));
    avl::append(payload, avl::to_bytes(yaw));
    return Field(TASK_ATTITUDE_DESC, payload);
}

//------------------------------------------------------------------------------
// Name:        TASK_VELOCITY
// Description: Creates a TASK packet VELOCITY field.
// Arguments:   - vx: x-axis velocity in m/s
//              - vy: y-axis velocity in m/s
//              - vz: z-axis velocity in m/s
// Returns:     TASK packet VELOCITY field.
//------------------------------------------------------------------------------
Field TASK_VELOCITY(double vx, double vy, double vz)
{
    std::vector<uint8_t> payload = avl::to_bytes(vx);
    avl::append(payload, avl::to_bytes(vy));
    avl::append(payload, avl::to_bytes(vz));
    return Field(TASK_VELOCITY_DESC, payload);
}

//------------------------------------------------------------------------------
// Name:        TASK_DEPTH
// Description: Creates a TASK packet DEPTH field.
// Arguments:   - depth: depth in meters
// Returns:     TASK packet DEPTH field.
//------------------------------------------------------------------------------
Field TASK_DEPTH(double depth)
{
    return Field(TASK_DEPTH_DESC, avl::to_bytes(depth));
}

//------------------------------------------------------------------------------
// Name:        TASK_HEIGHT
// Description: Creates a TASK packet HEIGHT field.
// Arguments:   - height: height in meters
// Returns:     TASK packet HEIGHT field.
//------------------------------------------------------------------------------
Field TASK_HEIGHT(double height)
{
    return Field(TASK_HEIGHT_DESC, avl::to_bytes(height));
}

//------------------------------------------------------------------------------
// Name:        TASK_RPM
// Description: Creates a TASK packet RPM field.
// Arguments:   - rpm: rotational rate in RPM
// Returns:     TASK packet RPM field.
//------------------------------------------------------------------------------
Field TASK_RPM(double rpm)
{
    return Field(TASK_RPM_DESC, avl::to_bytes(rpm));
}

//------------------------------------------------------------------------------
// Name:        TASK_DIVE
// Description: Creates a TASK packet DIVE field.
// Arguments:   - dive: True if dive mission
// Returns:     TASK packet DIVE field.
//------------------------------------------------------------------------------
Field TASK_DIVE(bool dive)
{
    return Field(TASK_DIVE_DESC, avl::to_bytes(dive));
}

//------------------------------------------------------------------------------
// Name:        TASK_POINTS
// Description: Creates a TASK packet POINTS field.
// Arguments:   - points: set of points <lat0, lon0, yaw0, lat1, lon1, yaw1 ...>
//                in degrees
// Returns:     TASK packet POINTS field.
//------------------------------------------------------------------------------
Field TASK_POINTS(std::vector<double> points)
{
    std::vector<uint8_t> payload;
    for (size_t i = 0; i < points.size(); i++)
    {
        avl::append(payload, avl::to_bytes(points.at(i)));
    }
    return Field(TASK_POINTS_DESC, payload);
}

//------------------------------------------------------------------------------
// Name:        TASK_COMMAND
// Description: Creates a TASK packet COMMAND field.
// Arguments:   - command: COMMAND descriptor
// Returns:     TASK packet COMMAND field.
//------------------------------------------------------------------------------
Field TASK_COMMAND(uint8_t command)
{
    return Field(TASK_COMMAND_DESC, avl::to_bytes(command));
}

//------------------------------------------------------------------------------
// Name:        HELM_THROTTLE
// Description: Creates a HELM packet THROTTLE field.
// Arguments:   - percent: throttle percentage
// Returns:     HELM packet THROTTLE field.
//------------------------------------------------------------------------------
Field HELM_THROTTLE(double percent)
{
    return Field(HELM_THROTTLE_DESC, avl::to_bytes(percent));
}

//------------------------------------------------------------------------------
// Name:        HELM_RUDDER
// Description: Creates a HELM packet RUDDER field.
// Arguments:   - angle: rudder angle in degrees
// Returns:     HELM packet RUDDER field.
//------------------------------------------------------------------------------
Field HELM_RUDDER(double angle)
{
    return Field(HELM_RUDDER_DESC, avl::to_bytes(angle));
}

//------------------------------------------------------------------------------
// Name:        HELM_ELEVATOR
// Description: Creates a HELM packet ELEVATOR field.
// Arguments:   - angle: elevator angle in degrees
// Returns:     HELM packet ELEVATOR field.
//------------------------------------------------------------------------------
Field HELM_ELEVATOR(double angle)
{
    return Field(HELM_ELEVATOR_DESC, avl::to_bytes(angle));
}

//------------------------------------------------------------------------------
// Name:        ACOUSTIC_PING_DEPARTURE_TIME
// Description: Creates an ACOUSTIC_PING packet DEPARTURE_TIME field.
// Arguments:   - t: departure time in seconds since epoch
// Returns:     ACOUSTIC_PING packet DEPARTURE_TIME field.
//------------------------------------------------------------------------------
Field ACOUSTIC_PING_DEPARTURE_TIME(double t)
{
    return Field(ACOUSTIC_PING_DEPARTURE_TIME_DESC, avl::to_bytes(t));
}

//------------------------------------------------------------------------------
// Name:        ACOUSTIC_PING_ORIGIN_POSITION
// Description: Creates a ACOUSTIC_PING packet ORIGIN_POSITION field.
// Arguments:   - lat: message origin latitude in degrees
//              - lon: message origin longitude in degrees
//              - alt: message origin altitude in meters
// Returns:     ACOUSTIC_PING packet ORIGIN_POSITION field.
//------------------------------------------------------------------------------
Field ACOUSTIC_PING_ORIGIN_POSITION(double lat, double lon, double alt)
{
    std::vector<uint8_t> payload = avl::to_bytes(lat);
    avl::append(payload, avl::to_bytes(lon));
    return Field(ACOUSTIC_PING_ORIGIN_POSITION_DESC, payload);
}

//------------------------------------------------------------------------------
// Name:        PARAMETER_NAME
// Description: Creates a PARAMETER packet NAME field.
// Arguments:   - name: parameter name ex: "/status_node/port"
// Returns:     PARAMETER packet NAME field.
//------------------------------------------------------------------------------
Field PARAMETER_NAME(std::string name)
{
    std::vector<uint8_t> string_vector(name.begin(), name.end());
    return Field(PARAMETER_NAME_DESC, string_vector);
}

//------------------------------------------------------------------------------
// Name:        PARAMETER_TYPE
// Description: Creates a PARAMETER packet TYPE field.
// Arguments:   - type: type name ex: "double"
// Returns:     PARAMETER packet TYPE field.
//------------------------------------------------------------------------------
Field PARAMETER_TYPE(std::string type)
{
    std::vector<uint8_t> string_vector(type.begin(), type.end());
    return Field(PARAMETER_TYPE_DESC, string_vector);
}

//------------------------------------------------------------------------------
// Name:        PARAMETER_LIST
// Description: Creates a PARAMETER_LIST packet LIST field.
// Arguments:   - parameters: Vector of PARAMETER packets
// Returns:     PARAMETER_LIST packet LIST field.
//------------------------------------------------------------------------------
Field PARAMETER_LIST(std::vector<Packet> parameters)
{
    std::vector<uint8_t> payload;
    for(size_t i = 0; i < 1; i++)
        avl::append(payload, parameters.at(i).get_bytes());
    return Field(PARAMETER_LIST_DESC, payload);
}

//------------------------------------------------------------------------------
// Name:        PARAMETER_LIST_SIZE
// Description: Creates a PARAMETER_LIST packet SIZE field.
// Returns:     PARAMETER_LIST packet SIZE field.
//------------------------------------------------------------------------------
Field PARAMETER_LIST_SIZE(int size)
{
    return Field(PARAMETER_LIST_SIZE_DESC, avl::to_bytes(size));
}

//------------------------------------------------------------------------------
// Name:        PARAMETER_LIST_REQUEST
// Description: Creates a PARAMETER_LIST packet REQUEST field.
// Returns:     PARAMETER_LIST packet REQUEST field.
//------------------------------------------------------------------------------
Field PARAMETER_LIST_REQUEST()
{
    return Field(PARAMETER_LIST_REQUEST_DESC);
}

// Add Comments


