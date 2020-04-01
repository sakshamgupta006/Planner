//==============================================================================
// Autonomous Vehicle Library
//
// Description: Class that manages a list of vehicles that the user interface
//              can interact with. Provides functions for selection and
//              interaction with vehicles. Emits signals that can be caught by
//              QML when vehicle properties change.
//==============================================================================

#include "vehicle_manager.h"

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//------------------------------------------------------------------------------
// Name:        VehicleManager constructor
// Description: Constructs the vehicle manager with a pointer to a vehicle
//              list model for display in a TableView.
// Arguments:   - vehicle_data_model: pointer to vehcle data model
//              - mission_data_model: pointer to mission data model
//------------------------------------------------------------------------------
VehicleManager::VehicleManager(VehicleDataModel* vehicle_data_model,
                               MissionDataModel* mission_data_model,
                               ParamDataModel* param_data_model,
                               GeofenceDataModel* geofence_data_model)
    : QObject(nullptr), vehicle_data_model(vehicle_data_model),
      mission_data_model(mission_data_model), param_data_model(param_data_model),
      geofence_data_model(geofence_data_model)
{

    // Set the vehicle list pointer in the vehicle data model. This is where the
    // data model will get its display data from
    vehicle_data_model->vehicle_list = &vehicle_list;

    // Bind to the multicast port and join the multicast address
    udp_socket.bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress);
    udp_socket.joinMulticastGroup(QHostAddress(multicast_address));

    // Fixes the startup problem by adding a default vehicle
    add_default_vehicle();

    // Connect the UDP's read ready signal to our data ready callback
    connect(&udp_socket, SIGNAL(readyRead()),
            this,        SLOT(udp_read_data_ready()));

}

//------------------------------------------------------------------------------
// Name:        VehicleManager destructor
// Description: Default destructor.
//------------------------------------------------------------------------------
VehicleManager::~VehicleManager()
{

}

//--------------------------------------------------------------------------
// Name:        add_default_vehicle
// Description: Adds a deafult vehicle to mitigate the startup problem
//--------------------------------------------------------------------------
void VehicleManager::add_default_vehicle()
{
    Vehicle* new_vehicle = new Vehicle(0, 1338);
    vehicle_data_model->start_insert_row();
    vehicle_list.append(new_vehicle);
    vehicle_data_model->stop_insert_row();

    if (selected_vehicles.empty())
        select_vehicles({0});

    emit vehicleAdded(0, new_vehicle);
    vehicle_data_model->vehicle_list = &vehicle_list;
}

//------------------------------------------------------------------------------
// Name:        num_vehicles
// Description: Returns the number of vehicles being managed.
// Returns:     Number of vehicles being managed.
//------------------------------------------------------------------------------
int VehicleManager::num_vehicles()
{
    return vehicle_list.size();
}

//------------------------------------------------------------------------------
// Name:        has_vehicle
// Description: Checks whether the the vehicle corresponding to the given
//              vehicle ID is in the list of vehicles.
// Arguments:   - vehicle_id: ID number of the vehicle
// Returns:     True if the vehicle is in the list, false otherwise.
//------------------------------------------------------------------------------
int VehicleManager::has_vehicle(int vehicle_id)
{

    bool vehicle_in_list = false;
    for (int i = 0; i < vehicle_list.size(); i++)
    {
        if (vehicle_list[i]->get_vehicle_id() == vehicle_id)
        {
            vehicle_in_list = true;
            break;
        }
    }

    return vehicle_in_list;

}

//------------------------------------------------------------------------------
// Name:        has_deckbox
// Description: Checks whether the vehicle manager has a deckbox in the
//              vehicle list and that it's connected.
// Returns:     True if there is a connected deckbox in the vehicle list,
//              false otherwise.
//------------------------------------------------------------------------------
bool VehicleManager::has_deckbox()
{
    int deckbox_index;
    return get_deckbox_index(deckbox_index) &&
           vehicle_list[deckbox_index]->is_connected();
}

//------------------------------------------------------------------------------
// Name:        get_vehicle_index
// Description: Gets the index of the vehicle corresponding to the given
//              vehicle ID.
// Arguments:   - vehicle_id: ID number of the vehicle
// Returns:     Index of vehicle in vehicle list.
//------------------------------------------------------------------------------
int VehicleManager::get_vehicle_index(int vehicle_id)
{

    int vehicle_index = 0;
    for (int i = 0; i < vehicle_list.size(); i++)
    {
        if (vehicle_list[i]->get_vehicle_id() == vehicle_id)
        {
            vehicle_index = i;
            break;
        }
    }

    return vehicle_index;

}

//------------------------------------------------------------------------------
// Name:        get_deckbox_index
// Description: Gets the index of the first vehicle with type DECKBOX in the
//              vehicle list.
// Arguments:   - deckbox_index: reference to int to store deckbox index
// Returns:     True if a deckbox was found in the vehicle list, false
//              otherwise.
//------------------------------------------------------------------------------
bool VehicleManager::get_deckbox_index(int& deckbox_index)
{

    bool found_deckbox = false;

    for (int i = 0; i < vehicle_list.size(); i++)
    {
        if (vehicle_list[i]->get_vehicle_type() == VehicleType::VEHICLE_DECKBOX)
        {
            deckbox_index = i;
            found_deckbox = true;
            break;
        }
    }

    return found_deckbox;

}

//------------------------------------------------------------------------------
// Name:        select_vehicles
// Description: Sets the given vehicle IDs as the selected vehicles.
// Arguments:   - vehicle_index: index of vehicle to be selected
//------------------------------------------------------------------------------
void VehicleManager::select_vehicles(QVector<int> vehicle_ids)
{
    selected_vehicles = vehicle_ids;

    mission_data_model->mission = get_selected_vehicle()->get_mission();
    mission_data_model->redraw();

    param_data_model->current_params = get_selected_vehicle()->get_params();
    param_data_model->redraw();

    geofence_data_model->current_geofence = get_selected_vehicle()->get_geofence();
    geofence_data_model->redraw();

    emit vehicleSelectionChanged(selected_vehicles);
}

//------------------------------------------------------------------------------
// Name:        get_selected_vehicles
// Description: Returns a vector of the IDs of the selected vehicles.
// Returns:     Vector of the IDs of the selected vehicles.
//------------------------------------------------------------------------------
QVector<int> VehicleManager::get_selected_vehicles()
{
    return selected_vehicles;
}

//------------------------------------------------------------------------------
// Name:        get_selected_vehicle
// Description: Returns a pointer to the selected vehicle.
// Returns:     Pointer to the selected vehicle.
//------------------------------------------------------------------------------
Vehicle* VehicleManager::get_selected_vehicle()
{
    if (!selected_vehicles.empty())
        return vehicle_list[get_vehicle_index(selected_vehicles[0])];
    return nullptr;
}

//------------------------------------------------------------------------------
// Name:        is_selected
// Description: Returns the selection status of a vehicle ID.
// Returns:     True if the vehicle is selected, false if it is not.
//------------------------------------------------------------------------------
bool VehicleManager::is_selected(int vehicle_id)
{
    for(int selected_vehicle_id: selected_vehicles)
        if (selected_vehicle_id == vehicle_id)
            return true;
    return false;
}

//------------------------------------------------------------------------------
// Name:        get_vehicle
// Description: Gets a pointer to the vehicle with the given vehicle ID.
// Arguments:   - vehicle_id: vehicle ID to get pointer to
// Returns:     Pointer to vehicle with given vehicle ID
//------------------------------------------------------------------------------
Vehicle *VehicleManager::get_vehicle(int vehicle_id)
{
    if (has_vehicle(vehicle_id))
        return vehicle_list[get_vehicle_index(vehicle_id)];
    return nullptr;
}

//------------------------------------------------------------------------------
// Name:        get_deckbox
// Description: Gets a pointer to the deckbox.
// Returns:     Pointer to deckbox.
//------------------------------------------------------------------------------
Vehicle *VehicleManager::get_deckbox()
{
    int index;
    if (get_deckbox_index(index))
        return vehicle_list[index];
    return nullptr;
}

//------------------------------------------------------------------------------
// Name:        get_vehicle_names
// Description: Gets a vector of managed vehicle names.
// Returns:     Vector of vehicle names
//------------------------------------------------------------------------------
QStringList VehicleManager::get_vehicle_names()
{
    QStringList vehicle_names;
    for (Vehicle* vehicle: vehicle_list)
        vehicle_names.append(QString::number(vehicle->get_vehicle_id()));
    return vehicle_names;
}

//------------------------------------------------------------------------------
// Name:        clear_vehicle_paths
// Description: Clears all vehicle paths.
//------------------------------------------------------------------------------
void VehicleManager::clear_vehicle_paths()
{
    for (Vehicle* vehicle: vehicle_list)
        vehicle->clear_path();
}

//------------------------------------------------------------------------------
// Name:        set_communication_channel
// Description: Sets the communication channel to be used to transmit
//              commands to the selected vehicle.
// Arguments:   - channel: channel to use to transmit commands
//------------------------------------------------------------------------------
Q_INVOKABLE void VehicleManager::set_communication_channel(CommsChannel::Value channel)
{
    comms_channel = channel;
}

//------------------------------------------------------------------------------
// Name:        zoom_to_vehicle
// Description: Emits a signal triggering a zoom to the vehicle with the
//              given vehicle ID.
// Arguments:   - vehicle_id: vehicle ID to zoom to
//------------------------------------------------------------------------------
void VehicleManager::zoom_to_vehicle(int vehicle_id)
{
    VehicleStatus status = get_vehicle(vehicle_id)->get_vehicle_status();
    emit vehicleZoomTriggered(status.lat, status.lon, 1000.0);
}

//------------------------------------------------------------------------------
// Name:        generate_empty_status
// Description: Generates an empty vehicle status struct for use in QML when
//              no vehicles have been connected.
// Returns:     Empty vehicle status struct.
//------------------------------------------------------------------------------
VehicleStatus VehicleManager::generate_empty_status()
{
    return VehicleStatus();
}

//------------------------------------------------------------------------------
// Name:        move_vehicle_waypoint
// Description: Edits the position of a waypoint in a vehicle's mission.
// Arguments:   - vehicle_index: index of the vehicle being edited in the
//                vehicle list. This is not the vehicle ID
//              - waypoint_index: index of the waypoint being edited
//              - new_location: new waypoint location in lat/lon in degrees
//------------------------------------------------------------------------------
void VehicleManager::move_vehicle_waypoint(int vehicle_index, int waypoint_index,
                                           QPointF new_location)
{
//    vehicle_list.at(vehicle_index)->get_mission()->move_waypoint(waypoint_index, new_location);
    mission_data_model->redraw();
}

//------------------------------------------------------------------------------
// Name:        get_deckbox_distance
// Description: Calculates the distance from the deckbox to the vehicle with
//              the given ID.
// Arguments:   - vehicle_id: ID of vehicle to calculate distance to
// Returns:     Distance from the deckbox to the vehicle with the given ID.
//------------------------------------------------------------------------------
double VehicleManager::get_deckbox_distance(int vehicle_id)
{

    if (has_deckbox())
    {

        // Get the deckbox and vehicle positions
        Vehicle* deckbox = get_deckbox();
        double lat1 = deckbox->get_vehicle_status().lat;
        double lon1 = deckbox->get_vehicle_status().lon;
        Vehicle* vehicle = get_vehicle(vehicle_id);
        double lat2 = vehicle->get_vehicle_status().lat;
        double lon2 = vehicle->get_vehicle_status().lon;

        // Calculate the range
        double d2r = 3.141592 / 180.0;
        double dlong = (lon2 - lon1) * d2r;
        double dlat = (lat2 - lat1) * d2r;
        double a = pow(sin(dlat/2.0), 2) + cos(lat1*d2r) * cos(lat2*d2r) * pow(sin(dlong/2.0), 2);
        double c = 2.0 * atan2(sqrt(a), sqrt(1.0-a));
        double d = 6367000.0 * c;

        return d;

    }

    return std::nan("");

}

//------------------------------------------------------------------------------
// Name:        get_deckbox_distance
// Description: Calculates the heading from the deckbox to the vehicle with
//              the given ID.
// Arguments:   - vehicle_id: ID of vehicle to calculate heading to
// Returns:     heading from the deckbox to the vehicle with the given ID.
//------------------------------------------------------------------------------
double VehicleManager::get_deckbox_heading(int vehicle_id)
{

    if (has_deckbox())
    {

        // Get the deckbox and vehicle positions
        Vehicle* deckbox = get_deckbox();
        double lat1 = deckbox->get_vehicle_status().lat;
        double lon1 = deckbox->get_vehicle_status().lon;
        Vehicle* vehicle = get_vehicle(vehicle_id);
        double lat2 = vehicle->get_vehicle_status().lat;
        double lon2 = vehicle->get_vehicle_status().lon;

        // Calculate the heading
        double d2r = 3.141592 / 180.0;
        double dLon = (lon2 - lon1) * d2r;
        double y = sin(dLon) * cos(lat2 * d2r);
        double x = cos(lat1 * d2r) * sin (lat2 * d2r) - sin(lat1 * d2r) * cos(lat2 * d2r) * cos(dLon);

        double r2d = 180.0 / 3.141592;
        double heading = atan2(y, x);
        heading = heading * r2d;
        heading = heading + 360;
        heading = fmod(heading, 360);

        return heading;

    }

    return std::nan("");

}

//------------------------------------------------------------------------------
// Name:        udp_read_data_ready
// Description: Slot that is called when the UDP socket has data available
//              to read.
//------------------------------------------------------------------------------
void VehicleManager::udp_read_data_ready()
{

    // Loop through all pending datagram data
    while (udp_socket.hasPendingDatagrams())
    {

        // Read the datagram
        QNetworkDatagram datagram = udp_socket.receiveDatagram();

        // Get the IP address that the datagram was received from
        QString ip_address = datagram.senderAddress().toString();

        // Turn the datagram data into an array of bytes, and
        QByteArray datagram_data = datagram.data();
        std::vector<uint8_t> data_bytes(datagram_data.begin(),
                                        datagram_data.end());

        // Attempt to parse the bytes in to a packet. If the bytes are not a
        // valid packet, ignore them
        avl::Packet packet;
        try
        {
            packet.set_bytes(data_bytes);
        }
        catch (const std::exception& ex)
        {
            qDebug() << "udp_read_data_ready: ignoring invalid packet bytes (" << ex.what() << ")";
            return;
        }

        // Get the origin vehicle ID from the packet. If it does not have a
        // vehicle ID field, ignore the packet
        int origin_vehicle_id;
        if (packet.has_field(VEHICLE_ID_DESC))
        {
            origin_vehicle_id = static_cast<int>(packet.get_field(VEHICLE_ID_DESC).get_data().at(0));
        }
        else
        {
            qDebug() << "udp_read_data_ready: ignoring packet without vehicle ID from" << ip_address << packet.get_num_fields();
            return;
        }

        // If the vehicle is not already in the vehicle list, append it
        if (!has_vehicle(origin_vehicle_id))
        {

            Vehicle* new_vehicle = new Vehicle(id_to_ip(origin_vehicle_id), 1338, this);
            vehicle_data_model->start_insert_row();
            vehicle_list.append(new_vehicle);
            vehicle_data_model->stop_insert_row();

            connect(new_vehicle, SIGNAL(connectionStatusChanged(QString, QString, bool)),
                    this,        SLOT(vehicle_connection_status_changed(QString, QString, bool)));

            connect(new_vehicle, SIGNAL(vehicleResponseReceived(int, QString)),
                    this,        SLOT(vehicle_response_received(int, QString)));

            connect(new_vehicle, SIGNAL(vehicleStatusReceived(int, VehicleStatus)),
                    this,        SLOT(vehicle_status_received(int, VehicleStatus)));

            connect(new_vehicle, SIGNAL(vehicleTypeChanged(int, VehicleType::Value)),
                    this,        SLOT(vehicle_type_changed(int, VehicleType::Value)));

            connect(new_vehicle, SIGNAL(missionTimeChanged(int, int)),
                    this,        SLOT(vehicle_mission_time_changed(int, int)));

            connect(new_vehicle, SIGNAL(missionDistanceChanged(int, double)),
                    this,        SLOT(vehicle_mission_distance_changed(int, double)));

            connect(new_vehicle, SIGNAL(missionDurationChanged(int, double)),
                    this,        SLOT(vehicle_mission_duration_changed(int, double)));

            connect(new_vehicle, SIGNAL(vehicleMissionReceived(int, Mission*)),
                    this,        SLOT(vehicle_mission_received(int, Mission*)));

            connect(new_vehicle, SIGNAL(vehicleParameterReceived(int, std::string, std::string, QVariant)),
                    this,        SLOT(vehicle_param_received(int, std::string, std::string, QVariant)));

            connect(new_vehicle, SIGNAL(vehicleParameterRefresh(int)),
                    this,        SLOT(vehicle_param_refresh(int)));

            connect(new_vehicle, SIGNAL(vehicleParametersFullyReceived(int)),
                    this,        SLOT(vehicle_parameters_fully_received(int)));

            if (selected_vehicles.empty())
                select_vehicles({origin_vehicle_id});

            emit vehicleAdded(origin_vehicle_id, new_vehicle);

        }

        // If the packet is a status packet and does not have magnetic flux
        // data, handle the status packet. We do not want to handle magnetic
        // flux status fields because they are only for calibration
        if (packet.get_descriptor() == STATUS_PACKET_DESC &&
            !packet.has_field(STATUS_MAG_FLUX_DESC))
        {

            // Update the vehicle's status
            VehicleStatus status(packet);
            int vehicle_index = get_vehicle_index(origin_vehicle_id);
            vehicle_list[vehicle_index]->set_vehicle_status(status);
            vehicle_data_model->update_row(vehicle_index);

            emit vehicleStatusUpdated(origin_vehicle_id, status);

        }

    }

}

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
void VehicleManager::vehicle_connection_status_changed(QString ip_address, QString connection_status,
                                                       bool can_send)
{
    emit vehicleConnectionStatusChanged(ip_to_id(ip_address), connection_status, can_send);
}

//------------------------------------------------------------------------------
// Name:        vehicle_response_received
// Description: Slot that is called when a response packet is received from
//              a vehicle.
// Arguments:   - origin_vehicle_id: ID of the vehicle that the response
//                originated from
//              - response: vehicle response string
//------------------------------------------------------------------------------
void VehicleManager::vehicle_response_received(int origin_vehicle_id, QString response)
{
    if (has_vehicle(origin_vehicle_id))
    {
        vehicle_list[get_vehicle_index(origin_vehicle_id)]->append_vehicle_response(response);
        emit vehicleResponseReceived(origin_vehicle_id, response);
    }
}


//------------------------------------------------------------------------------
// Name:        vehicle_mission_received
// Description: Slot that is called when a response packet is received from
//              a vehicle.
// Arguments:   - origin_vehicle_id: ID of the vehicle that the response
//                originated from
//              - response: vehicle response string
//------------------------------------------------------------------------------
void VehicleManager::vehicle_mission_received(int origin_vehicle_id, Mission* mission)
{
    if (has_vehicle(origin_vehicle_id))
    {
        vehicle_list[get_vehicle_index(origin_vehicle_id)]->parse_populate_mission(mission);
        emit vehicleMissionReceived(origin_vehicle_id, mission);
    }
}

void VehicleManager::vehicle_param_received(int origin_vehicle_id, std::string name, std::string type, QVariant value)
{
    if(has_vehicle(origin_vehicle_id))
    {
        vehicle_list[get_vehicle_index(origin_vehicle_id)]->append_param(name, type, value);
    }
}

void VehicleManager::vehicle_param_refresh(int origin_vehicle_id)
{
    if(has_vehicle(origin_vehicle_id))
    {//useless
        vehicle_list[get_vehicle_index(origin_vehicle_id)]->current_params.clear();

        get_selected_vehicle()->clear_param_list();
    }
}

void VehicleManager::vehicle_parameters_fully_received(int origin_vehicle_id)
{
    if(has_vehicle(origin_vehicle_id))
    {
        vehicle_list[get_vehicle_index(origin_vehicle_id)]->print_param_list();

        param_data_model->current_params = get_selected_vehicle()->get_params();
        param_data_model->redraw();
    }
}

//------------------------------------------------------------------------------
// Name:        vehicleStatusReceived
// Description: Slot that is called when a status packet is received from
//              a vehicle.
// Arguments:   - origin_vehicle_id: ID of the vehicle that the status
//                originated from
//              - status: vehicle status struct
//------------------------------------------------------------------------------
void VehicleManager::vehicle_status_received(int origin_vehicle_id, VehicleStatus status)
{
    int vehicle_index = get_vehicle_index(origin_vehicle_id);
    vehicle_list[vehicle_index]->set_vehicle_status(status);
    vehicle_data_model->update_row(vehicle_index);
}

//------------------------------------------------------------------------------
// Name:        vehicle_type_changed
// Description: Slot that is called when a vehicle's type changes.
// Arguments:   - vehicle_id: ID of the vehicle whose type changed
//              - new_type: new vehicle type
//------------------------------------------------------------------------------
void VehicleManager::vehicle_type_changed(int vehicle_id, VehicleType::Value new_type)
{
    emit vehicleTypeChanged(vehicle_id, new_type);
}

//------------------------------------------------------------------------------
// Name:        vehicle_mission_time_changed
// Description: Slot that is called when a vehicle's mission time changes.
// Arguments:   - vehicle_id: ID of the vehicle whose mission time changed
//              - new_mission_time: new mission time
//------------------------------------------------------------------------------
void VehicleManager::vehicle_mission_time_changed(int vehicle_id, int new_mission_time)
{
    emit vehicleMissionTimeChanged(vehicle_id, new_mission_time);
}

//------------------------------------------------------------------------------
// Name:        vehicle_mission_distance_changed
// Description: Slot that is called when a vehicle's mission time changes.
// Arguments:   - vehicle_id: ID of the vehicle whose mission time changed
//              - new_mission_time: new mission time
//------------------------------------------------------------------------------
void VehicleManager::vehicle_mission_distance_changed(int vehicle_id, double new_mission_distance)
{
    emit vehicleMissionDistanceChanged(vehicle_id, new_mission_distance);
}

//------------------------------------------------------------------------------
// Name:        vehicle_mission_duration_changed
// Description: Slot that is called when a vehicle's mission time changes.
// Arguments:   - vehicle_id: ID of the vehicle whose mission time changed
//              - new_mission_time: new mission time
//------------------------------------------------------------------------------
void VehicleManager::vehicle_mission_duration_changed(int vehicle_id, double new_mission_duration)
{
    emit vehicleMissionDurationChanged(vehicle_id, new_mission_duration);
}

//------------------------------------------------------------------------------
// Name:        ip_to_id
// Description: Converts an IP address string to a vehicle ID number. The
//              ID number is simply the last number in the address.
// Arguments:   - ip_address: IP address to convert
// Returns:     Vehicle ID number.
//------------------------------------------------------------------------------
int VehicleManager::ip_to_id(QString ip_address)
{

    // Find the last period in the string. We want the number after the last
    // period, which is the last number in the address
    int index_last_period = ip_address.lastIndexOf(".");

    // Get the substring after the last period, which is the last number in
    // string form
    QString last_number = ip_address.mid(index_last_period+1);

    return last_number.toInt();

}

//------------------------------------------------------------------------------
// Name:        id_to_ip
// Description: Converts a vehicle ID number to an IP address string. The
//              ID number is simply the last number in the address.
//              TODO: For now, assume 10.0.10.XXX
// Arguments:   - vehicle_id: vehicle ID number to convert
// Returns:     IP address string.
//------------------------------------------------------------------------------
QString VehicleManager::id_to_ip(int vehicle_id)
{
    return QStringLiteral("10.0.10.%1").arg(vehicle_id);
}
