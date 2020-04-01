//==============================================================================
// Autonomous Vehicle Library
//
// Description: Handles communication over TCP with a vehicle and provides
//              functions for sending commands.
//==============================================================================

#ifndef VEHICLE_CONNECTION_H
#define VEHICLE_CONNECTION_H

// QObject base class
#include <QObject>

// QTcpSocket class for TCP communication with vehicle
#include <QTcpSocket>

// Vehicle command packets
#include "comms/avl_commands.h"

// Vehicle status struct
#include "vehicle_status.h"

#include "comms_channel.h"

#include "mission.h"

#include "param.h"

//==============================================================================
//                              CLASS DECLARATION
//==============================================================================

class VehicleConnection : public QObject
{

    Q_OBJECT

signals:

    //--------------------------------------------------------------------------
    // Name:        connectionStatusChanged
    // Description: Signal that is emitted when the connection status of the
    //              vehicle TCP socket changes.
    // Arguments:   - ip_address: IP address of the vehicle
    //              - connection_status: string representing the connection
    //                status
    //              - can_send: true if data can be sent to the vehicle, false
    //                otherwise
    //--------------------------------------------------------------------------
    void connectionStatusChanged(QString ip_address, QString connection_status, bool can_send);

    //--------------------------------------------------------------------------
    // Name:        vehicleResponseReceived
    // Description: Signal that is emitted when a response packet is received.
    //              May come from a vehicle other than this one due to message
    //              forwarding.
    // Arguments:   - origin_vehicle_id: ID of the vehicle that the response
    //                originated from
    //              - response: vehicle response string
    //--------------------------------------------------------------------------
    void vehicleResponseReceived(int origin_vehicle_id, QString response);

    //--------------------------------------------------------------------------
    // Name:        vehicleResponseReceived
    // Description: Signal that is emitted when a response packet is received.
    //              May come from a vehicle other than this one due to message
    //              forwarding.
    // Arguments:   - origin_vehicle_id: ID of the vehicle that the response
    //                originated from
    //              - response: vehicle response string
    //--------------------------------------------------------------------------
    void vehicleMissionReceived(int origin_vehicle_id, Mission* mission);

    void vehicleParameterReceived(int origin_vehicle_id, std::string name,
                                  std::string type, QVariant value);

   void vehicleParametersFullyReceived(int origin_vehicle_id);

   void vehicleParameterRefresh(int origin_vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        vehicleStatusReceived
    // Description: Signal that is emitted when a status packet is received from
    //              the vehicle. May come from a vehicle other than this one due
    //              to message forwarding.
    // Arguments:   - origin_vehicle_id: ID of the vehicle that the status
    //                originated from
    //              - status: vehicle status struct
    //--------------------------------------------------------------------------
    void vehicleStatusReceived(int origin_vehicle_id, VehicleStatus status);

public:

    //--------------------------------------------------------------------------
    // Name:        VehicleConnection constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    VehicleConnection(QObject* parent=nullptr);

    //--------------------------------------------------------------------------
    // Name:        VehicleConnection destructor
    // Description: Default destructor.
    //--------------------------------------------------------------------------
    ~VehicleConnection();

    //--------------------------------------------------------------------------
    // Name:        open
    // Description: Opens the connection to the vehicle, retrying until a
    //              connection is established.
    //--------------------------------------------------------------------------
    void open(QString ip_address, quint16 port);

    //--------------------------------------------------------------------------
    // Name:        close
    // Description: Closes the connection to the vehicle.
    //--------------------------------------------------------------------------
    void close();

    //--------------------------------------------------------------------------
    // Name:        is_connected
    // Description: Returns the status of the vehicle connection.
    // Returns:     True if connection is open, false otherwise
    //--------------------------------------------------------------------------
    Q_INVOKABLE bool is_connected();

    //--------------------------------------------------------------------------
    // Name:        get_connection_status
    // Description: Returns a string representing the status of the vehicle
    //              connection.
    // Returns:     String representing the status of the vehicle connection.
    //--------------------------------------------------------------------------
    Q_INVOKABLE QString get_connection_status();

    //--------------------------------------------------------------------------
    // Name:        send_emergency_stop
    // Description: Sends an emergency stop command to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_emergency_stop(CommsChannel::Value comms_channel,
                                         int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_enable_helm_mode
    // Description: Sends an enable helm mode command to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_enable_helm_mode(CommsChannel::Value comms_channel,
                                           int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_disable_helm_mode
    // Description: Sends a disable helm mode command to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_disable_helm_mode(CommsChannel::Value comms_channel,
                                            int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_start_mission
    // Description: Sends a start mission command to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_start_mission(CommsChannel::Value comms_channel,
                                        int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_advance_mission
    // Description: Sends an advance mission command to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_advance_mission(CommsChannel::Value comms_channel,
                                          int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_stop_mission
    // Description: Sends a stop mission command to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_stop_mission(CommsChannel::Value comms_channel,
                                       int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_clear_mission
    // Description: Sends a clear mission command to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_clear_mission(CommsChannel::Value comms_channel,
                                        int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_read_mission
    // Description: Sends a read mission command to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_read_mission(CommsChannel::Value comms_channel,
                                       int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_read_params
    // Description: Sends a read param command to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_read_params(CommsChannel::Value comms_channel,
                                       int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_write_params
    // Description: Sends a param list to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_write_params(Params* parameters,
                                       CommsChannel::Value comms_channel,
                                       int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_enable_magnetometer_stream
    // Description: Sends an enable magnetometer stream command to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_enable_magnetometer_stream(CommsChannel::Value comms_channel,
                                                     int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_disable_magnetometer_stream
    // Description: Sends a disable magnetometer stream command to the vehicle.
    //--------------------------size_t,------------------------------------------------
    Q_INVOKABLE void send_disable_magnetometer_stream(CommsChannel::Value comms_channel,
                                                      int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_zero_pressure_sensor
    // Description: Sends a zero pressure sensor command to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_zero_pressure_sensor(CommsChannel::Value comms_channel,
                                               int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_reset_safety_node
    // Description: Sends a reset safety node command to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_reset_safety_node(CommsChannel::Value comms_channel,
                                            int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_add_mission
    // Description: Sends a mission append command to the vehicle.
    // Arguments:   - mission: pointer to mission to send to the vehicle.
    //              - comms_channel: communication channel to send the packet
    //                over
    //              - vehicle_id: destination vehicle ID
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_add_mission(Mission* mission,
                                      CommsChannel::Value comms_channel,
                                      int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_helm_throttle
    // Description: Sends a helm throttle command to the vehicle.
    // Arguments:   - throttle: throttle percentage
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_helm_throttle(double throttle,
                                        CommsChannel::Value comms_channel,
                                        int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_helm_rudder
    // Description: Sends a helm rudder command to the vehicle.
    // Arguments:   - angle: rudder angle in degrees
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_helm_rudder(double angle,
                                      CommsChannel::Value comms_channel,
                                      int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_start_lbl_pings
    // Description: Sends an action command to start LBL pings to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_start_lbl_pings(CommsChannel::Value comms_channel,
                                          int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_start_owtt_pings
    // Description: Sends an action command to start OWTT pings to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_start_owtt_pings(CommsChannel::Value comms_channel,
                                           int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_stop_acoustic_pings
    // Description: Sends an action command to stop acoustic pings to the
    //              vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_stop_acoustic_pings(CommsChannel::Value comms_channel,
                                              int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_set_geofence
    // Description: Sends an action command to set the geofence to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_set_geofence(CommsChannel::Value comms_channel,
                                       int vehicle_id,
                                       QVector<QPointF> geofence_points);

    //--------------------------------------------------------------------------
    // Name:        send_enable_lights
    // Description: Sends an action command to enable lights on the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_enable_lights(CommsChannel::Value comms_channel,
                                        int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_disable_lights
    // Description: Sends an action command to disable lights on the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_disable_lights(CommsChannel::Value comms_channel,
                                         int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_enable_sonar
    // Description: Sends an action command to enable sonar attached to the
    //              vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_enable_sonar(CommsChannel::Value comms_channel,
                                         int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_disable_sonar
    // Description: Sends an action command to disable sonar attached to the
    //              vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_disable_sonar(CommsChannel::Value comms_channel,
                                         int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_start_sonar_recording
    // Description: Sends an action command to start recording sonar data.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_start_sonar_recording(CommsChannel::Value comms_channel,
                                                int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_stop_sonar_recording
    // Description: Sends an action command to stop recording sonar data.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_stop_sonar_recording(CommsChannel::Value comms_channel,
                                               int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        packet_to_parameter
    // Description: Parses a PARAMETER packet and attempts to set the
    //              parameter of interest
    // Arguments:   - parameter_packet: Packet containing parameter info
    //--------------------------------------------------------------------------
    Q_INVOKABLE void packet_to_parameter(avl::Packet parameter_packet,
                                         int vehicle_id);



private slots:

    //--------------------------------------------------------------------------
    // Name:        tcp_connection_error
    // Description: Slot that is called when the TCP socket encounters a
    //              connection error.
    // Arguments:   - error: SocketError describing the error that occured
    //--------------------------------------------------------------------------
    void tcp_connection_error(QAbstractSocket::SocketError error);

    //--------------------------------------------------------------------------
    // Name:        tcp_state_changed
    // Description: Slot that is called when the connection state of the TCP
    //              socket changes.
    // Arguments:   - state: new TCP socket state
    //--------------------------------------------------------------------------
    void tcp_state_changed(QAbstractSocket::SocketState state);

    //--------------------------------------------------------------------------
    // Name:        tcp_read_data_ready
    // Description: Slot that is called when the TCP socket has data available
    //              to read.
    //--------------------------------------------------------------------------
    void tcp_read_data_ready();

private:

    // Vehicle IP address and port
    QString m_ip_address;
    quint16 m_port;

    // TCP socket for connecting to the vehicle
    QTcpSocket* tcp_socket;

    // Flag indicating whether the connection should be retried if it fails
    bool retry_connection;

    // String representing connection status
    QString connection_status = "DISCONNECTED";

private:

    //--------------------------------------------------------------------------
    // Name:        write
    // Description: Writes data to the vehicle if the connection to the vehicle
    //              is open. Does nothing if the connection is not open.
    // Arguments:   - data: data to write to host
    //--------------------------------------------------------------------------
    void write(std::vector<uint8_t> data);

    //--------------------------------------------------------------------------
    // Name:        write_packet
    // Description: Writes a packet to the host with the given comms channel
    //              field and vehicle ID field.
    // Arguments:   - packet: packet to write to host
    //              - comms_channel: comms channel field value
    //              - vehicle_id: vehicle ID field value
    //--------------------------------------------------------------------------
    void write_packet(avl::Packet packet, CommsChannel::Value comms_channel,
                      int vehicle_id);

};

#endif // VEHICLE_CONNECTION_H

