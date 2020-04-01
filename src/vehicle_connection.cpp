//==============================================================================
// Autonomous Vehicle Library
//
// Description: A QObject representing a vehicle. Contains a TCP connection to
// the vehicle, the vehicle's mission, and and vehicle status information.
//==============================================================================

#include "vehicle_connection.h"

// Vehicle command packets
#include "comms/avl_commands.h"

#include "comms_channel.h"

#include "task_type.h"

#include <QPointF>

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//------------------------------------------------------------------------------
// Name:        VehicleConnection constructor
// Description: Default constructor.
//------------------------------------------------------------------------------
VehicleConnection::VehicleConnection(QObject* parent) : QObject(parent)
{
    // Create the TCP socket
    tcp_socket = new QTcpSocket();

    // Set the keep alive option to detect loss of connection
    tcp_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    // Connect the TCP socket's signals to this class's slots
    connect(tcp_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this,       &VehicleConnection::tcp_connection_error);
    connect(tcp_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,       SLOT(tcp_state_changed(QAbstractSocket::SocketState)));
    connect(tcp_socket, SIGNAL(readyRead()),
            this,       SLOT(tcp_read_data_ready()));

}

//------------------------------------------------------------------------------
// Name:        VehicleConnection destructor
// Description: Default destructor.
//------------------------------------------------------------------------------
VehicleConnection::~VehicleConnection()
{

}

//------------------------------------------------------------------------------
// Name:        open
// Description: Opens the connection to the vehicle, retrying until a
//              connection is established.
//------------------------------------------------------------------------------
void VehicleConnection::open(QString ip_address, quint16 port)
{
    if (tcp_socket->state() == QAbstractSocket::UnconnectedState)
    {
        m_ip_address = ip_address;
        m_port = port;
        retry_connection = true;
        tcp_socket->connectToHost(m_ip_address, m_port);
    }
}

//------------------------------------------------------------------------------
// Name:        close
// Description: Closes the connection to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::close()
{
    retry_connection = false;
    tcp_socket->abort();
}

//------------------------------------------------------------------------------
// Name:        is_connected
// Description: Returns the status of the vehicle connection.
// Returns:     True if connection is open, false otherwise
//------------------------------------------------------------------------------
bool VehicleConnection::is_connected()
{
    return tcp_socket->state() == QAbstractSocket::ConnectedState;
}

//----------------------------------------------------------------------------
// Name:        get_connection_status
// Description: Returns a string representing the status of the vehicle
//              connection.
// Returns:     String representing the status of the vehicle connection.
//----------------------------------------------------------------------------
QString VehicleConnection::get_connection_status()
{
    return connection_status;
}

//------------------------------------------------------------------------------
// Name:        send_emergency_stop
// Description: Sends an emergency stop command to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_emergency_stop(CommsChannel::Value comms_channel,
                                            int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_EMERGENCY_STOP());
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_enable_helm_mode
// Description: Sends an enable helm mode command to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_enable_helm_mode(CommsChannel::Value comms_channel,
                                              int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_SET_MODE("MANUAL"));
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_disable_helm_mode
// Description: Sends a disable helm mode command to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_disable_helm_mode(CommsChannel::Value comms_channel,
                                               int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_SET_MODE("AUTONOMOUS"));
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------`
// Name:        send_start_mission
// Description: Sends a start mission command to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_start_mission(CommsChannel::Value comms_channel,
                                           int vehicle_id)
{
    avl::Packet packet = MISSION_PACKET();
    packet.add_field(MISSION_START());
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_advance_mission
// Description: Sends an advance mission command to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_advance_mission(CommsChannel::Value comms_channel,
                                             int vehicle_id)
{
    avl::Packet packet = MISSION_PACKET();
    packet.add_field(MISSION_ADVANCE());
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_stop_mission
// Description: Sends a stop mission command to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_stop_mission(CommsChannel::Value comms_channel,
                                          int vehicle_id)
{
    avl::Packet packet = MISSION_PACKET();
    packet.add_field(MISSION_STOP());
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_clear_mission
// Description: Sends a clear mission command to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_clear_mission(CommsChannel::Value comms_channel,
                                           int vehicle_id)
{
    avl::Packet packet = MISSION_PACKET();
    packet.add_field(MISSION_CLEAR());
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_read_mission
// Description: Sends a read mission command to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_read_mission(CommsChannel::Value comms_channel,
                                          int vehicle_id)
{
    avl::Packet packet = MISSION_PACKET();
    packet.add_field(MISSION_READ_ALL());
    write_packet(packet, comms_channel, vehicle_id);
}

//--------------------------------------------------------------------------
// Name:        send_read_params
// Description: Sends a read param command to the vehicle.
//--------------------------------------------------------------------------
void VehicleConnection::send_read_params(CommsChannel::Value comms_channel,
                                         int vehicle_id)
{
    avl::Packet packet = PARAMETER_LIST_PACKET();
    packet.add_field(PARAMETER_LIST_REQUEST_DESC);
    write_packet(packet, comms_channel, vehicle_id);
}

//--------------------------------------------------------------------------
// Name:        send_write_params
// Description: Sends a param list to the vehicle.
//--------------------------------------------------------------------------
void VehicleConnection::send_write_params(Params* parameters,
                                          CommsChannel::Value comms_channel,
                                          int vehicle_id)
{
    avl::Packet packet_list = PARAMETER_LIST_PACKET();
    packet_list.add_field(PARAMETER_LIST(parameters->get_params()));
    packet_list.add_field(PARAMETER_LIST_SIZE(1));
    write_packet(packet_list, comms_channel, vehicle_id);
}


//------------------------------------------------------------------------------
// Name:        send_enable_magnetometer_stream
// Description: Sends an enable magnetometer stream command to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_enable_magnetometer_stream(CommsChannel::Value comms_channel,
                                                        int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_SET_MAG_STREAM(true));
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_disable_magnetometer_stream
// Description: Sends a disable magnetometer stream command to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_disable_magnetometer_stream(CommsChannel::Value comms_channel,
                                                         int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_SET_MAG_STREAM(false));
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_zero_pressure_sensor
// Description: Sends a zero pressure sensor command to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_zero_pressure_sensor(CommsChannel::Value comms_channel,
                                                  int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_TARE_PRESSURE());
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_reset_safety_node
// Description: Sends a reset safety node command to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_reset_safety_node(CommsChannel::Value comms_channel,
                                               int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_RESET_SAFETY());
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_add_mission
// Description: Sends an append command to the vehicle.
// Arguments:   - duration: mission duration in seconds
//              - roll: roll angle in degrees
//              - pitch: pitch angle in degrees
//              - yaw: yaw angle in degrees
//              - vx: x-axis velocity in m/s
//              - vy: y-axis velocity in m/s
//              - vz: z-axis velocity in m/s
//              - lat: latitude in degrees
//              - lon: longitude in degrees
//              - alt: altitude in meters
//              - depth: depth in meters
//              - height: height in meters
//              - rpm: RPM
//              - dive: true to indicate a dive mission
//------------------------------------------------------------------------------
void VehicleConnection::send_add_mission(Mission* mission,
                                         CommsChannel::Value comms_channel,
                                         int vehicle_id)
{
    avl::Packet mission_packet = MISSION_PACKET();
    mission_packet.add_field(MISSION_APPEND(mission->get_task_packets()));
    write_packet(mission_packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_helm_throttle
// Description: Sends a helm throttle command to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_helm_throttle(double value,
                                           CommsChannel::Value comms_channel,
                                           int vehicle_id)
{
    avl::Packet packet = HELM_PACKET();
    packet.add_field(HELM_THROTTLE(value));
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_helm_rudder
// Description: Sends a helm rudder command to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_helm_rudder(double value,
                                         CommsChannel::Value comms_channel,
                                         int vehicle_id)
{
    avl::Packet packet = HELM_PACKET();
    packet.add_field(HELM_RUDDER(value));
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_start_lbl_pings
// Description: Sends an action command to start LBL pings to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_start_lbl_pings(CommsChannel::Value comms_channel,
                                             int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_START_LBL_PINGS());
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_start_owtt_pings
// Description: Sends an action command to start OWTT pings to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_start_owtt_pings(CommsChannel::Value comms_channel,
                                              int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_START_OWTT_PINGS());
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_stop_acoustic_pings
// Description: Sends an action command to stop acoustic pings to the
//              vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_stop_acoustic_pings(CommsChannel::Value comms_channel,
                                                 int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_STOP_ACOUSTIC_PINGS());
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        send_set_geofence
// Description: Sends an action command to set the geofence to the vehicle.
//------------------------------------------------------------------------------
void VehicleConnection::send_set_geofence(CommsChannel::Value comms_channel,
                                          int vehicle_id,
                                          QVector<QPointF> geofence_points)
{

    std::vector<double> lats;
    std::vector<double> lons;
    for (QPointF point : geofence_points)
    {
        lats.push_back(point.y());
        lons.push_back(point.x());
    }

    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_SET_GEOFENCE(lats, lons));
    write_packet(packet, comms_channel, vehicle_id);

}

//--------------------------------------------------------------------------
// Name:        send_enable_lights
// Description: Sends an action command to enable lights on the vehicle.
//--------------------------------------------------------------------------
void VehicleConnection::send_enable_lights(CommsChannel::Value comms_channel,
                                           int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_ENABLE_STROBE());
    write_packet(packet, comms_channel, vehicle_id);
}

//--------------------------------------------------------------------------
// Name:        send_disable_lights
// Description: Sends an action command to disable lights on the vehicle.
//--------------------------------------------------------------------------
void VehicleConnection::send_disable_lights(CommsChannel::Value comms_channel,
                                           int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_DISABLE_STROBE());
    write_packet(packet, comms_channel, vehicle_id);
}

//--------------------------------------------------------------------------
// Name:        send_enable_sonar
// Description: Sends an action command to enable sonar attached to the
//              vehicle.
//--------------------------------------------------------------------------
void VehicleConnection::send_enable_sonar(CommsChannel::Value comms_channel, int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_ENABLE_SONAR());
    write_packet(packet, comms_channel, vehicle_id);
}

//--------------------------------------------------------------------------
// Name:        send_disable_sonar
// Description: Sends an action command to disable sonar attached to the
//              vehicle.
//--------------------------------------------------------------------------
void VehicleConnection::send_disable_sonar(CommsChannel::Value comms_channel, int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_DISABLE_SONAR());
    write_packet(packet, comms_channel, vehicle_id);
}

//--------------------------------------------------------------------------
// Name:        send_start_sonar_recording
// Description: Sends an action command to start recording sonar data.
//--------------------------------------------------------------------------
void VehicleConnection::send_start_sonar_recording(CommsChannel::Value comms_channel,
                                                   int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_START_SONAR_RECORDING());
    write_packet(packet, comms_channel, vehicle_id);
}

//--------------------------------------------------------------------------
// Name:        send_stop_sonar_recording
// Description: Sends an action command to stop recording sonar data.
//--------------------------------------------------------------------------
void VehicleConnection::send_stop_sonar_recording(CommsChannel::Value comms_channel,
                                                  int vehicle_id)
{
    avl::Packet packet = ACTION_PACKET();
    packet.add_field(ACTION_STOP_SONAR_RECORDING());
    write_packet(packet, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        tcp_connection_error
// Description: Slot that is called when the TCP socket encounters a
//              connection error.
// Arguments:   - error: SocketError describing the error that occured
//------------------------------------------------------------------------------
void VehicleConnection::tcp_connection_error(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    if (retry_connection)
        open(m_ip_address, m_port);
}

//------------------------------------------------------------------------------
// Name:        tcp_state_changed
// Description: Slot that is called when the connection state of the TCP
//              socket changes.
// Arguments:   - state: new TCP socket state
//------------------------------------------------------------------------------
void VehicleConnection::tcp_state_changed(QAbstractSocket::SocketState state)
{
    switch (state)
    {
        case QAbstractSocket::UnconnectedState:
        {
            connection_status = "DISCONNECTED";
            emit connectionStatusChanged(m_ip_address, connection_status, false);
            if (retry_connection)
                open(m_ip_address, m_port);
            break;
        }
        case QAbstractSocket::HostLookupState:
        {
            connection_status = "CONNECTING";
            emit connectionStatusChanged(m_ip_address, connection_status, false);
            break;
        }
        case QAbstractSocket::ConnectingState:
        {
            connection_status = "CONNECTING";
            emit connectionStatusChanged(m_ip_address, connection_status, false);
            break;
        }
        case QAbstractSocket::ConnectedState:
        {
            connection_status = "CONNECTED";
            emit connectionStatusChanged(m_ip_address, connection_status, true);
            break;
        }
        case QAbstractSocket::BoundState:
        {
            emit connectionStatusChanged(m_ip_address, connection_status, false);
            connection_status = "BOUND";
            break;
        }
        case QAbstractSocket::ClosingState:
        {
            connection_status = "DISCONNECTING";
            emit connectionStatusChanged(m_ip_address, connection_status, false);
            break;
        }
        case QAbstractSocket::ListeningState:
        {
            connection_status = "LISTENING";
            emit connectionStatusChanged(m_ip_address, connection_status, false);
            break;
        }
    }
}






//------------------------------------------------------------------------------
// Name:        tcp_read_data_ready
// Description: Slot that is called when the TCP socket has data available
//              to read.
//------------------------------------------------------------------------------
void VehicleConnection::tcp_read_data_ready()
{

    // Read all available bytes and turn the byte array into a vector
    QByteArray data = tcp_socket->readAll();
    std::vector<uint8_t> data_bytes(data.begin(), data.end());

    try
    {

        // Parse all packets from the byte vector
        std::vector<avl::Packet> packets = avl::Packet::parse_multiple(data_bytes);

        // Handle all packets in the packet vector
        for (avl::Packet packet: packets)
        {
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Handle response packets

            if (packet.get_descriptor() == RESPONSE_PACKET_DESC)
            {
                if(packet.has_field(RESPONSE_FIELD_DESCRIPTOR_DESC))
                {
                    uint8_t response_packet_descriptor = packet.get_field(RESPONSE_FIELD_DESCRIPTOR_DESC).get_data().at(0);
                    if (response_packet_descriptor == MISSION_READ_ALL_DESC)
                    {
                        if (packet.has_field(VEHICLE_ID_DESC))
                        {
                            int origin_vehicle_id = static_cast<int>(packet.get_field(VEHICLE_ID_DESC).get_data().at(0));
                            std::vector<uint8_t> message_data = packet.get_field(RESPONSE_DATA_DESC).get_data();
                            std::vector<avl::Packet> missions = avl::Packet::parse_multiple(message_data);
                            Mission* current_mission = new Mission();
                            for(auto mission : missions)
                               current_mission->append(Task::packet_to_task(mission));
                            emit vehicleMissionReceived(origin_vehicle_id, current_mission);
                        }
                    }
                    else if(response_packet_descriptor == PARAMETER_LIST_REQUEST_DESC)
                    {
                        if(packet.has_field(VEHICLE_ID_DESC))
                        {
                            int origin_vehicle_id = static_cast<int>(packet.get_field(VEHICLE_ID_DESC).get_data().at(0));
                            std::vector<uint8_t> message_data = packet.get_field(RESPONSE_DATA_DESC).get_data();
                            avl::Packet list_packet(message_data);
                            if(list_packet.has_field(PARAMETER_LIST_DESC))
                            {
                                std::vector<uint8_t> list_data = list_packet.get_field(PARAMETER_LIST_DESC).get_data();
                                QString response = QString::fromStdString(std::string(list_data.begin(), list_data.end()));
                                //emit vehicleResponseReceived(origin_vehicle_id, response);

                                // Create a vector of parameter packets
                                std::vector<avl::Packet> parameter_packets = avl::Packet::parse_multiple(list_data);
                                emit vehicleParameterRefresh(origin_vehicle_id);

                                // Handle each parameter
                                for(int i = 0; i < parameter_packets.size(); i++)
                                    packet_to_parameter(parameter_packets.at(i), origin_vehicle_id);

                                emit vehicleParametersFullyReceived(origin_vehicle_id);
                            }
                        }
                    }
                    else if (packet.has_field(RESPONSE_DATA_DESC))
                    {
                        if (packet.has_field(VEHICLE_ID_DESC))
                        {
                            int origin_vehicle_id = static_cast<int>(packet.get_field(VEHICLE_ID_DESC).get_data().at(0));
                            std::vector<uint8_t> message_data = packet.get_field(RESPONSE_DATA_DESC).get_data();
                            QString response = QString::fromStdString(std::string(message_data.begin(), message_data.end()));
                            emit vehicleResponseReceived(origin_vehicle_id, response);
                        }
                        else
                        {
                            qDebug() << "ignoring response packet with no vehicle ID field received by vehicle " << m_ip_address;
                        }
                    }
                }
            }

            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Handle status packets
            else if (packet.get_descriptor() == STATUS_PACKET_DESC)
            {
                if (packet.has_field(VEHICLE_ID_DESC))
                {
                    int origin_vehicle_id = static_cast<int>(packet.get_field(VEHICLE_ID_DESC).get_data().at(0));
                    emit vehicleStatusReceived(origin_vehicle_id, VehicleStatus(packet));
                }
                else
                {
                    qDebug() << "ignoring status packet with no vehicle ID field received by vehicle " << m_ip_address;
                }
            }

        }

    }
    catch (const std::exception& ex)
    {
        qDebug() << "ignoring invalid packet (" << ex.what() << ")";
    }

}

//--------------------------------------------------------------------------
// Name:        packet_to_parameter
// Description: Parses a PARAMETER packet and attempts to set the
//              parameter of interest
// Arguments:   - parameter_packet: Packet containing parameter info
//--------------------------------------------------------------------------
void VehicleConnection::packet_to_parameter(avl::Packet parameter_packet, int vehicle_id)
{
   // Get the parameter name
   std::string name;
   if(parameter_packet.has_field(PARAMETER_NAME_DESC))
   {
       std::vector<uint8_t> name_data = parameter_packet.get_field(PARAMETER_NAME_DESC).get_data();
       name = std::string(name_data.begin(), name_data.end());
   }
   else
       return;

   // Get the parameter type
   std::string type;
   if(parameter_packet.has_field(PARAMETER_TYPE_DESC))
   {
       std::vector<uint8_t> type_data = parameter_packet.get_field(PARAMETER_TYPE_DESC).get_data();
       type = std::string(type_data.begin(), type_data.end());
   }
   else
       return;

   // Get the parameter value
   if(parameter_packet.has_field(PARAMETER_TYPE_DESC))
   {

       // Handle setting the parameter to the appropriate type
       if(!type.compare("bool"))
       {
           bool value = avl::from_bytes<bool>(parameter_packet.get_field(PARAMETER_VALUE_DESC).get_data());
           qDebug () <<QString::fromStdString(name) << " "<< QString::fromStdString(type) << " " << value;
           emit vehicleParameterReceived(vehicle_id, name, type, value);
           return;
       }
       else if(!type.compare("int"))
       {
           int value = avl::from_bytes<int>(parameter_packet.get_field(PARAMETER_VALUE_DESC).get_data());
           qDebug () <<QString::fromStdString(name) << " "<< QString::fromStdString(type) << " " << value;
           emit vehicleParameterReceived(vehicle_id, name, type, value);
           return;
       }
       else if(!type.compare("float"))
       {
           float value = avl::from_bytes<float>(parameter_packet.get_field(PARAMETER_VALUE_DESC).get_data());
           qDebug () <<QString::fromStdString(name) << " "<< QString::fromStdString(type) << " " << value;
           emit vehicleParameterReceived(vehicle_id, name, type, value);
           return;
       }
       else if(!type.compare("double"))
       {
           double value = avl::from_bytes<double>(parameter_packet.get_field(PARAMETER_VALUE_DESC).get_data());
           qDebug () <<QString::fromStdString(name) << " "<< QString::fromStdString(type) << " " << value;
           emit vehicleParameterReceived(vehicle_id, name, type, value);
           return;
       }
       else if(!type.compare("string") || !type.compare("std::string"))
       {
           std::vector<uint8_t> string_data = parameter_packet.get_field(PARAMETER_VALUE_DESC).get_data();
           std::string value(string_data.begin(), string_data.end());
           qDebug () <<QString::fromStdString(name) << " "<< QString::fromStdString(type) << " " << QString::fromStdString(value);
           emit vehicleParameterReceived(vehicle_id, name, type, QString::fromStdString(value));
           return;
       }
       else
       {
           qDebug() <<"Parameter not deciphered";
           return;
       }

   }
   else
   {
       return;
   }

}


//------------------------------------------------------------------------------
// Name:        write
// Description: Writes data to the host if the connection to the host is
//              open. Does nothing if the connection is not open.
// Arguments:   - data: data to write to host
//------------------------------------------------------------------------------
void VehicleConnection::write(std::vector<uint8_t> data)
{
    if (tcp_socket->state() == QTcpSocket::ConnectedState)
    {
        std::string data_string(data.begin(),data.end());
        tcp_socket->write(data_string.c_str(), static_cast<quint16>(data_string.length()));
        tcp_socket->waitForBytesWritten();
    }
}

//------------------------------------------------------------------------------
// Name:        write_packet
// Description: Writes a packet to the host with the given comms channel
//              field and vehicle ID field.
// Arguments:   - packet: packet to write to host
//              - comms_channel: comms channel field value
//              - vehicle_id: vehicle ID field value
//------------------------------------------------------------------------------
void VehicleConnection::write_packet(avl::Packet packet, CommsChannel::Value comms_channel,
                                     int vehicle_id)
{
    packet.add_field(VEHICLE_ID(static_cast<uint8_t>(vehicle_id)));
    qDebug() <<"inside write packet";
    switch (comms_channel)
    {
        case CommsChannel::Value::COMMS_RADIO:    packet.add_field(COMMS_CHANNEL(COMMS_CHANNEL_RADIO)); break;
        case CommsChannel::Value::COMMS_ACOUSTIC: packet.add_field(COMMS_CHANNEL(COMMS_CHANNEL_ACOMMS)); break;
        case CommsChannel::Value::COMMS_IRIDIUM:  packet.add_field(COMMS_CHANNEL(COMMS_CHANNEL_IRIDIUM)); break;
    }
    write(packet.get_bytes());
}
