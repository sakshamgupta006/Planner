//==============================================================================
// Autonomous Vehicle Library
//
// Description: Class that manages a list of vehicles that the user interface
//              can interact with. Provides functions for selection and
//              interaction with vehicles. Emits signals that can be caught by
//              QML when vehicle properties change.
//==============================================================================

#ifndef VEHICLE_MANAGER_H
#define VEHICLE_MANAGER_H

// QObject base class
#include <QObject>
//#include <QQmlContext>

// Vehicle class
#include "vehicle.h"

// UDP socket
#include <QtNetwork>

// Table model for status display
#include "vehicle_data_model.h"

// Table model for mission display
#include "mission_data_model.h"

// Table model for parameter display
#include "param_data_model.h"

// Comms channel enum
#include "comms_channel.h"

#include "geofence_data_model.h"

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

class VehicleManager : public QObject
{

    Q_OBJECT

signals:

    //--------------------------------------------------------------------------
    // Name:        vehicleAdded
    // Description: Signal that is emitted when a vehicle is added to the
    //              vehicle list.
    // Arguments:   - vehicle_id: ID of the added vehicle
    //              - vehicle: pointer to the new vehicle instance
    //--------------------------------------------------------------------------
    void vehicleAdded(int vehicle_id, Vehicle* vehicle);

    //--------------------------------------------------------------------------
    // Name:        vehicleConnectionStatusChanged
    // Description: Signal that is emitted when the TCP connection status of a
    //              vehicle changes.
    // Arguments:   - vehicle_id: ID of the vehicle whose connection status
    //                changed
    //              - new_connection_status: string representing the new
    //                connection status
    //              - can_send: indicates whether commands can be sent to the
    //                vehicle in the new connection status
    //--------------------------------------------------------------------------
    void vehicleConnectionStatusChanged(int vehicle_id, QString new_connection_status,
                                        bool can_send);

    //--------------------------------------------------------------------------
    // Name:        vehicleTypeChanged
    // Description: Signal that is emitted when a vehicle type changes.
    // Arguments:   - vehicle_id: ID of the changed vehicle
    //              - new_type: new vehicle type
    //--------------------------------------------------------------------------
    void vehicleTypeChanged(int vehicle_id, VehicleType::Value new_type);

    //--------------------------------------------------------------------------
    // Name:        vehicleStatusUpdated
    // Description: Signal that is emitted when the status of a vehicle changes.
    // Arguments:   - vehicle_id: ID of the vehicle whose status changed
    //              - new_status: new vehicle status struct
    //--------------------------------------------------------------------------
    void vehicleStatusUpdated(int vehicle_id, VehicleStatus new_status);

    //--------------------------------------------------------------------------
    // Name:        vehicleSelectionChanged
    // Description: Signal that is emitted when the list of selected vehicles
    //              changes.
    // Arguments:   - selected_vehicles: IDs of the selected vehicles
    //--------------------------------------------------------------------------
    void vehicleSelectionChanged(QVector<int> selected_vehicles);

    //--------------------------------------------------------------------------
    // Name:        vehicleResponseReceived
    // Description: Signal that is emitted when a vehicle response is received
    //              over one of the communication channels.
    // Arguments:   - origin_vehicle_id: ID of the vehicle responding
    //              - response: response string received
    //--------------------------------------------------------------------------
    void vehicleResponseReceived(int origin_vehicle_id, QString response);

    //--------------------------------------------------------------------------
    // Name:        vehicleZoomTriggered
    // Description: Signal that is emitted when the a zoom to a vehicle is
    //              triggered.
    // Arguments:   - lat: zoom center latitude in degrees
    //              - lon: zoom center longitude in degrees
    //              - side_length: zoom area side length in meters
    //--------------------------------------------------------------------------
    void vehicleZoomTriggered(double lat, double lon, double side_length);

    //--------------------------------------------------------------------------
    // Name:        vehicleMissionTimeChanged
    // Description: Signal that is emitted when a vehicle's mission time
    //              changes.
    // Arguments:   - vehicle_id: ID of the vehicle whose mission time changed
    //              - new_mission_time: new mission time in seconds
    //--------------------------------------------------------------------------
    void vehicleMissionTimeChanged(int vehicle_id, int new_mission_time);

    //--------------------------------------------------------------------------
    // Name:        vehicleMissionDistanceChanged
    // Description: Signal that is emitted when a vehicle's mission time
    //              changes.
    // Arguments:   - vehicle_id: ID of the vehicle whose mission time changed
    //              - new_mission_time: new mission time in seconds
    //--------------------------------------------------------------------------
    void vehicleMissionDistanceChanged(int vehicle_id, double new_mission_distance);

    //--------------------------------------------------------------------------
    // Name:        vehicleMissionDurationChanged
    // Description: Signal that is emitted when a vehicle's mission time
    //              changes.
    // Arguments:   - vehicle_id: ID of the vehicle whose mission time changed
    //              - new_mission_time: new mission time in seconds
    //--------------------------------------------------------------------------
    void vehicleMissionDurationChanged(int vehicle_id, double new_mission_duration);


    //--------------------------------------------------------------------------
    // Name:        vehicleMissionReceived
    // Description: Signal that is emitted when a vehicle message is received
    //              over one of the communication channels.
    // Arguments:   - origin_vehicle_id: ID of the vehicle responding
    //              - mission: response mission received
    //--------------------------------------------------------------------------
    void vehicleMissionReceived(int origin_vehicle_id, Mission* mission);

    //--------------------------------------------------------------------------
    // Name:        vehicleMissionReceived
    // Description: Signal that is emitted when a vehicle message is received
    //              over one of the communication channels.
    // Arguments:   - origin_vehicle_id: ID of the vehicle responding
    //              - mission: response mission received
    //--------------------------------------------------------------------------
    void vehicleParameterReceived(int origin_vehicle_id, std::string name,
                                  std::string type, QVariant value);

    void vehicleParameterRefresh(int origin_vehicle_id);

    void vehicleParametersFullyReceived(int origin_vehicle_id);

public:

    //--------------------------------------------------------------------------
    // Name:        VehicleManager constructor
    // Description: Constructs the vehicle manager with a pointer to a vehicle
    //              list model for display in a TableView.
    // Arguments:   - vehicle_data_model: pointer to vehcle data model
    //              - mission_data_model: pointer to mission data model
    //--------------------------------------------------------------------------
    VehicleManager(VehicleDataModel* vehicle_data_model,
                   MissionDataModel* mission_data_model,
                   ParamDataModel* param_data_model,
                   GeofenceDataModel* geofence_data_model);

    //--------------------------------------------------------------------------
    // Name:        VehicleManager destructor
    // Description: Default destructor.
    //--------------------------------------------------------------------------
    ~VehicleManager();

    //--------------------------------------------------------------------------
    // Name:        num_vehicles
    // Description: Returns the number of vehicles being managed.
    // Returns:     Number of vehicles being managed.
    //--------------------------------------------------------------------------
    Q_INVOKABLE int num_vehicles();

    //--------------------------------------------------------------------------
    // Name:        has_vehicle
    // Description: Checks whether the the vehicle corresponding to the given
    //              vehicle ID is in the list of vehicles.
    // Arguments:   - vehicle_id: ID number of the vehicle
    // Returns:     True if the vehicle is in the list, false otherwise.
    //--------------------------------------------------------------------------
    Q_INVOKABLE int has_vehicle(int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        has_deckbox
    // Description: Checks whether the vehicle manager has a deckbox in the
    //              vehicle list and that it's connected.
    // Returns:     True if there is a connected deckbox in the vehicle list,
    //              false otherwise.
    //--------------------------------------------------------------------------
    Q_INVOKABLE bool has_deckbox();

    //--------------------------------------------------------------------------
    // Name:        get_vehicle_index
    // Description: Gets the index of the vehicle corresponding to the given
    //              vehicle ID.
    // Arguments:   - vehicle_id: ID number of the vehicle
    // Returns:     Index of vehicle in vehicle list.
    //--------------------------------------------------------------------------
    Q_INVOKABLE int get_vehicle_index(int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        get_deckbox_index
    // Description: Gets the index of the first vehicle with type DECKBOX in the
    //              vehicle list.
    // Arguments:   - deckbox_index: reference to int to store deckbox index
    // Returns:     True if a deckbox was found in the vehicle list, false
    //              otherwise.
    //--------------------------------------------------------------------------
    Q_INVOKABLE bool get_deckbox_index(int& deckbox_index);

    //--------------------------------------------------------------------------
    // Name:        select_vehicles
    // Description: Sets the given vehicle IDs as the selected vehicles.
    // Arguments:   - vehicle_index: index of vehicle to be selected
    //--------------------------------------------------------------------------
    Q_INVOKABLE void select_vehicles(QVector<int> vehicle_ids);

    //--------------------------------------------------------------------------
    // Name:        get_selected_vehicles
    // Description: Returns a vector of the IDs of the selected vehicles.
    // Returns:     Vector of the IDs of the selected vehicles.
    //--------------------------------------------------------------------------
    Q_INVOKABLE QVector<int> get_selected_vehicles();

    //--------------------------------------------------------------------------
    // Name:        get_selected_vehicle
    // Description: Returns a pointer to the selected vehicle.
    // Returns:     Pointer to the selected vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE Vehicle* get_selected_vehicle();

    //--------------------------------------------------------------------------
    // Name:        is_selected
    // Description: Returns the selection status of a vehicle ID.
    // Returns:     True if the vehicle is selected, false if it is not.
    //--------------------------------------------------------------------------
    Q_INVOKABLE bool is_selected(int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        get_vehicle
    // Description: Gets a pointer to the vehicle with the given vehicle ID.
    // Arguments:   - vehicle_id: vehicle ID to get pointer to
    // Returns:     Pointer to vehicle with given vehicle ID
    //--------------------------------------------------------------------------
    Q_INVOKABLE Vehicle* get_vehicle(int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        get_deckbox
    // Description: Gets a pointer to the deckbox.
    // Returns:     Pointer to deckbox.
    //--------------------------------------------------------------------------
    Q_INVOKABLE Vehicle* get_deckbox();

    //--------------------------------------------------------------------------
    // Name:        get_vehicle_names
    // Description: Gets a list of managed vehicle names.
    // Returns:     List of vehicle names
    //--------------------------------------------------------------------------
    Q_INVOKABLE QStringList get_vehicle_names();

    //--------------------------------------------------------------------------
    // Name:        clear_vehicle_paths
    // Description: Clears all vehicle paths.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void clear_vehicle_paths();

    //--------------------------------------------------------------------------
    // Name:        set_communication_channel
    // Description: Sets the communication channel to be used to transmit
    //              commands to the selected vehicle.
    // Arguments:   - channel: channel to use to transmit commands
    //--------------------------------------------------------------------------
    Q_INVOKABLE void set_communication_channel(CommsChannel::Value channel);

    //--------------------------------------------------------------------------
    // Name:        zoom_to_vehicle
    // Description: Emits a signal triggering a zoom to the vehicle with the
    //              given vehicle ID.
    // Arguments:   - vehicle_id: vehicle ID to zoom to
    //--------------------------------------------------------------------------
    Q_INVOKABLE void zoom_to_vehicle(int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        generate_empty_status
    // Description: Generates an empty vehicle status struct for use in QML when
    //              no vehicles have been connected.
    // Returns:     Empty vehicle status struct.
    //--------------------------------------------------------------------------
    Q_INVOKABLE VehicleStatus generate_empty_status();

    //--------------------------------------------------------------------------
    // Name:        move_vehicle_waypoint
    // Description: Edits the position of a waypoint in a vehicle's mission.
    // Arguments:   - vehicle_index: index of the vehicle being edited in the
    //                vehicle list. This is not the vehicle ID
    //              - waypoint_index: index of the waypoint being edited
    //              - new_location: new waypoint location in lat/lon in degrees
    //--------------------------------------------------------------------------
    Q_INVOKABLE void move_vehicle_waypoint(int vehicle_index, int waypoint_index,
                                           QPointF new_location);

    //--------------------------------------------------------------------------
    // Name:        get_deckbox_distance
    // Description: Calculates the distance from the deckbox to the vehicle with
    //              the given ID.
    // Arguments:   - vehicle_id: ID of vehicle to calculate distance to
    // Returns:     Distance from the deckbox to the vehicle with the given ID.
    //--------------------------------------------------------------------------
    Q_INVOKABLE double get_deckbox_distance(int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        get_deckbox_distance
    // Description: Calculates the heading from the deckbox to the vehicle with
    //              the given ID.
    // Arguments:   - vehicle_id: ID of vehicle to calculate heading to
    // Returns:     heading from the deckbox to the vehicle with the given ID.
    //--------------------------------------------------------------------------
    Q_INVOKABLE double get_deckbox_heading(int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        add_default_vehicle
    // Description: Adds a deafult vehicle to mitigate the startup problem
    //--------------------------------------------------------------------------
    Q_INVOKABLE void add_default_vehicle();

private:

    // List of vehicles being managed
    QVector<Vehicle*> vehicle_list;

    // Index of the selected vehicle
    QVector<int> selected_vehicles;

    // Selected communication channel
    CommsChannel::Value comms_channel = CommsChannel::Value::COMMS_RADIO;

    // UDP socket for receiving status messages
    QUdpSocket udp_socket;

    // Multicast address and port to join for status messages
    QString multicast_address = "224.0.0.138";
    quint16 port = 1338;

    // Pointer to vehicle data model to display vehicle status as a table
    VehicleDataModel* vehicle_data_model;

    // Pointer to mission data model to display vehicle mission information
    // as a table
    MissionDataModel* mission_data_model;


    // Pointer to param data model to display vehicle parameter information
    // as a table
    ParamDataModel* param_data_model;

    GeofenceDataModel* geofence_data_model;

private slots:

    //--------------------------------------------------------------------------
    // Name:        udp_read_data_ready
    // Description: Slot that is called when the UDP socket has data available
    //              to read.
    //--------------------------------------------------------------------------
    void udp_read_data_ready();

    //--------------------------------------------------------------------------
    // Name:        vehicle_connection_status_changed
    // Description: Slot that is called when the connection status of a vehicle
    //              changes.
    // Arguments:   - ip_address: IP address of the vehicle
    //              - connection_status: string representing the connection
    //                status
    //              - can_send: true if data can be sent to the vehicle, false
    //                otherwise
    //--------------------------------------------------------------------------
    void vehicle_connection_status_changed(QString ip_address, QString connection_status,
                                           bool can_send);

    //--------------------------------------------------------------------------
    // Name:        vehicle_response_received
    // Description: Slot that is called when a response packet is received from
    //              a vehicle.
    // Arguments:   - origin_vehicle_id: ID of the vehicle that the response
    //                originated from
    //              - response: vehicle response string
    //--------------------------------------------------------------------------
    void vehicle_response_received(int origin_vehicle_id, QString response);

    //--------------------------------------------------------------------------
    // Name:        vehicleStatusReceived
    // Description: Slot that is called when a status packet is received from
    //              a vehicle.
    // Arguments:   - origin_vehicle_id: ID of the vehicle that the status
    //                originated from
    //              - status: vehicle status struct
    //--------------------------------------------------------------------------
    void vehicle_status_received(int origin_vehicle_id, VehicleStatus status);

    //--------------------------------------------------------------------------
    // Name:        vehicle_type_changed
    // Description: Slot that is called when a vehicle's type changes.
    // Arguments:   - vehicle_id: ID of the vehicle whose type changed
    //              - new_type: new vehicle type
    //--------------------------------------------------------------------------
    void vehicle_type_changed(int vehicle_id, VehicleType::Value new_type);

    //--------------------------------------------------------------------------
    // Name:        vehicle_mission_time_changed
    // Description: Slot that is called when a vehicle's mission time changes.
    // Arguments:   - vehicle_id: ID of the vehicle whose mission time changed
    //              - new_mission_time: new mission time
    //--------------------------------------------------------------------------
    void vehicle_mission_time_changed(int vehicle_id, int new_mission_time);

    //--------------------------------------------------------------------------
    // Name:        vehicle_mission_distance_changed
    // Description: Slot that is called when a vehicle's mission time changes.
    // Arguments:   - vehicle_id: ID of the vehicle whose mission time changed
    //              - new_mission_time: new mission time
    //--------------------------------------------------------------------------
    void vehicle_mission_distance_changed(int vehicle_id, double new_mission_distance);

    //--------------------------------------------------------------------------
    // Name:        vehicle_mission_duration_changed
    // Description: Slot that is called when a vehicle's mission time changes.
    // Arguments:   - vehicle_id: ID of the vehicle whose mission time changed
    //              - new_mission_time: new mission time
    //--------------------------------------------------------------------------
    void vehicle_mission_duration_changed(int vehicle_id, double new_mission_duration);

    //--------------------------------------------------------------------------
    // Name:        vehicle_mission_received
    // Description: Slot that is called when a response packet is received from
    //              a vehicle.
    // Arguments:   - origin_vehicle_id: ID of the vehicle that the response
    //                originated from
    //              - response: vehicle response string
    //--------------------------------------------------------------------------
    void vehicle_mission_received(int origin_vehicle_id, Mission* mission);

    //--------------------------------------------------------------------------
    // Name:        vehicle_mission_received
    // Description: Slot that is called when a response packet is received from
    //              a vehicle.
    // Arguments:   - origin_vehicle_id: ID of the vehicle that the response
    //                originated from
    //              - response: vehicle response string
    //--------------------------------------------------------------------------
    void vehicle_param_received(int origin_vehicle_id, std::string name,
                                std::string type, QVariant value);

    void vehicle_param_refresh(int origin_vehicle_id);

    void vehicle_parameters_fully_received(int origin_vehicle_id);


private:

    //--------------------------------------------------------------------------
    // Name:        ip_to_id
    // Description: Converts an IP address string to a vehicle ID number. The
    //              ID number is simply the last number in the address.
    // Arguments:   - ip_address: IP address to convert
    // Returns:     Vehicle ID number.
    //--------------------------------------------------------------------------
    int ip_to_id(QString ip_address);

    //--------------------------------------------------------------------------
    // Name:        id_to_ip
    // Description: Converts a vehicle ID number to an IP address string. The
    //              ID number is simply the last number in the address.
    //              TODO: For now, assume 10.0.10.XXX
    // Arguments:   - vehicle_id: vehicle ID number to convert
    // Returns:     IP address string.
    //--------------------------------------------------------------------------
    QString id_to_ip(int vehicle_id);

};

#endif // VEHICLE_MANAGER_H
