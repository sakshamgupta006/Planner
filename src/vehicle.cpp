//==============================================================================
// Autonomous Vehicle Library
//
// Description: A QObject representing a vehicle. Contains a TCP connection to
// the vehicle, the vehicle's mission, and and vehicle status information.
//==============================================================================

#include "vehicle.h"

// Debug output
#include <QDebug>

#include "pugixml.hpp"

#include "graphics.h"

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//--------------------------------------------------------------------------
// Name:        Vehicle constructor
// Description: Default constructor.
//--------------------------------------------------------------------------
Vehicle::Vehicle(QObject *parent) : VehicleConnection(parent)
{

}

//------------------------------------------------------------------------------
// Name:        Vehicle constructor
// Description: Constructs the vehicle with an IP address and port.
// Arguments:   - ip_address: vehicle IP address
//              - port: vehicle port
//------------------------------------------------------------------------------
Vehicle::Vehicle(QString ip_address, quint16 port, QObject* parent) :
    VehicleConnection(parent), ip_address(ip_address), port(port)
{

    // Get the vehicle ID from the last number in the IP address
    int index_last_period = ip_address.lastIndexOf(".");
    QString last_number = ip_address.mid(index_last_period+1);
    id = static_cast<uint8_t>(last_number.toInt());

    // Set up the vehicle graphics overlays
    path_overlay = std::shared_ptr<GraphicsOverlay>(new GraphicsOverlay(this));
    mission_overlay = std::shared_ptr<GraphicsOverlay>(new GraphicsOverlay(this));
    geofence_overlay = std::shared_ptr<GraphicsOverlay>(new GraphicsOverlay(this));

    //Initialize Geofence


    // Configure the mission time timer
    connect(mission_time_timer, &QTimer::timeout, this, &Vehicle::mission_time_timer_timeout);

    // Conect the mission changed signal of the vehcle's mission to the
    // slot on the vehicle so that the mission graphic can be updated when the
    // mission changes
    connect(&mission, SIGNAL(missionChanged(void)),
            this,     SLOT(mission_changed(void)));

    connect(&geofence, SIGNAL(geofencepointsChanged(QVector<QPointF>)),
            this,      SLOT(geofence_changed(QVector<QPointF>)));

}

//------------------------------------------------------------------------------
// Name:        Vehicle destructor
// Description: Default destructor.
//------------------------------------------------------------------------------
Vehicle::~Vehicle()
{

}

//------------------------------------------------------------------------------
// Name:        declare_qml
// Description: Registers types with the QML engine.
//------------------------------------------------------------------------------
void Vehicle::declare_qml()
{
    qmlRegisterInterface<Vehicle>("Vehicle");
    qRegisterMetaType<VehicleStatus>("VehicleStatus");
}

//------------------------------------------------------------------------------
// Name:        connect_to_vehicle
// Description: Opens a connection to the vehicle, retrying until a
//              connection is established.
//------------------------------------------------------------------------------
void Vehicle::connect_to_vehicle()
{
    open(ip_address, port);
}

//------------------------------------------------------------------------------
// Name:        disconnect_from_vehicle
// Description: Closes the connection to the vehicle.
//------------------------------------------------------------------------------
void Vehicle::disconnect_from_vehicle()
{
    close();
}

//------------------------------------------------------------------------------
// Name:        append_vehicle_response
// Description: Appends a vehicle response to the response list.
// Arguments:   - response: response to append
//------------------------------------------------------------------------------
void Vehicle::append_vehicle_response(QString response)
{
    vehicle_responses.append(response);
}

//------------------------------------------------------------------------------
// Name:        clear_vehicle_responses
// Description: Clears the list of vehicle responses.
//------------------------------------------------------------------------------
void Vehicle::clear_vehicle_responses()
{
    vehicle_responses.clear();
}

//------------------------------------------------------------------------------
// Name:        clear_path
// Description: Clears the path representing the vehicle's location history.
//------------------------------------------------------------------------------
void Vehicle::clear_path()
{
    path.clear();
    path_overlay->graphics()->clear();
}

//------------------------------------------------------------------------------
// Name:        start_mission_timer
// Description: Starts the vehicle's mission time timer.
//------------------------------------------------------------------------------
void Vehicle::start_mission_timer()
{
    mission_time = 0;
    mission_time_timer->start(1000);
    emit missionTimeChanged(id, mission_time);

}

//------------------------------------------------------------------------------
// Name:        reset_mission_timer
// Description: Resets the vehicle's mission time timer.
//------------------------------------------------------------------------------
void Vehicle::reset_mission_timer()
{
    mission_time_timer->stop();
    mission_time = 0;
    emit missionTimeChanged(id, mission_time);
}

//------------------------------------------------------------------------------
// Name:        get_vehicle_id
// Description: Gets the vehicle's ID number.
// Returns:     Vehicle ID number.
//------------------------------------------------------------------------------
int Vehicle::get_vehicle_id()
{
    return id;
}

//------------------------------------------------------------------------------
// Name:        get_vehicle_color
// Description: Gets the vehicle's color.
// Returns:     Vehicle color.
//------------------------------------------------------------------------------
QColor Vehicle::get_vehicle_color()
{
    return color;
}

//------------------------------------------------------------------------------
// Name:        get_vehicle_type
// Description: Gets the vehicle's type.
// Returns:     Vehicle type.
//------------------------------------------------------------------------------
VehicleType::Value Vehicle::get_vehicle_type()
{
    return type;
}

//------------------------------------------------------------------------------
// Name:        get_vehicle_status
// Description: Gets the vehicle's status.
// Returns:     Vehicle status struct.
//------------------------------------------------------------------------------
VehicleStatus Vehicle::get_vehicle_status()
{
    return status;
}

//------------------------------------------------------------------------------
// Name:        get_mission
// Description: Gets the vehicle's mission.
// Returns:     Vehicle mission.
//------------------------------------------------------------------------------
Mission* Vehicle::get_mission()
{
    return &mission;
}

//--------------------------------------------------------------------------
// Name:        get_params
// Description: Gets the vehicle's params.
// Returns:     Vehicle parameter.
//--------------------------------------------------------------------------
Params* Vehicle::get_params()
{
    return &parameters;
}

//--------------------------------------------------------------------------
// Name:        get_geofence
// Description: Gets the vehicle's geofence.
// Returns:     Vehicle parameter.
//--------------------------------------------------------------------------
Geofence* Vehicle::get_geofence()
{
    return &geofence;
}


//------------------------------------------------------------------------------
// Name:        get_mission_time
// Description: Gets the time since a mission was started.
// Returns:     Time in seconds since a mission was started.
//------------------------------------------------------------------------------
int Vehicle::get_mission_time()
{
    return mission_time;
}

//--------------------------------------------------------------------------
// Name:        get_mission_distance
// Description: Gets the total mission distance.
// Returns:     Distance in Km.
//--------------------------------------------------------------------------
double Vehicle::get_mission_distance()
{
    return mission_distance;
}

//--------------------------------------------------------------------------
// Name:        get_mission_duration
// Description: Gets the total mission distance.
// Returns:     Distance in Km.
//--------------------------------------------------------------------------
double Vehicle::get_mission_duration()
{
    return mission_duration;
}


//------------------------------------------------------------------------------
// Name:        get_vehicle_responses
// Description: Gets a list of vehicle responses.
// Returns:     Vector of vehicle response strings.
//------------------------------------------------------------------------------
QVector<QString> Vehicle::get_vehicle_responses()
{
    return vehicle_responses;
}


//------------------------------------------------------------------------------
// Name:        get_path_overlay
// Description: Gets a pointer to the vehicle's path overlay.
// Returns:     Pointer to the vehicle's path overlay.
//------------------------------------------------------------------------------
std::shared_ptr<GraphicsOverlay> Vehicle::get_path_overlay()
{
    return path_overlay;
}

//------------------------------------------------------------------------------
// Name:        get_mission_overlay
// Description: Gets a pointer to the vehicle's mission overlay.
// Returns:     Pointer to the vehicle's mission overlay.
//------------------------------------------------------------------------------
std::shared_ptr<GraphicsOverlay> Vehicle::get_mission_overlay()
{
    return mission_overlay;
}

//--------------------------------------------------------------------------
// Name:        get_geofence_overlay
// Description: Gets a pointer to the vehicle's mission overlay.
// Returns:     Pointer to the vehicle's mission overlay.
//--------------------------------------------------------------------------
std::shared_ptr<GraphicsOverlay> Vehicle::get_geofence_overlay()
{
    return geofence_overlay;
}

//------------------------------------------------------------------------------
// Name:        set_vehicle_color
// Description: Sets the vehicle's icon and path color.
// Arguments:   - color: vehicle color
//------------------------------------------------------------------------------
void Vehicle::set_vehicle_color(QColor new_color)
{

    color = new_color;

    // Call the mission changed slot to redraw the mission with the new color
    mission_changed();

}

//------------------------------------------------------------------------------
// Name:        set_vehicle_type
// Description: Sets the vehicle's type.
// Arguments:   - type: vehicle type
//------------------------------------------------------------------------------
void Vehicle::set_vehicle_type(VehicleType::Value new_type)
{
    type = new_type;
    emit vehicleTypeChanged(id, new_type);
}

//------------------------------------------------------------------------------
// Name:        set_vehicle_status
// Description: Sets the vehicle's status.
// Arguments:   - new_status: vehicle status
//------------------------------------------------------------------------------
void Vehicle::set_vehicle_status(VehicleStatus new_status)
{

    // Set the new status
    status = new_status;

    // If the new status value has a location, update the vehicle icon and path
    if (!std::isnan(status.lat) && !std::isnan(status.lon))
    {

        // Create a point at the new vehicle location and add it to the path
        // queue. If the queue is over the max number of points to display,
        // remove the oldest point (the front of the queue)
        Point new_location(status.lon, status.lat, SpatialReference::wgs84());
        path.enqueue(new_location);
        if (path.size() > MAX_PATH_POINTS)
            path.dequeue();

        // Update the vehicle and path graphic with the new location
        get_vehicle_graphic(path_overlay, path, status.yaw, color);

    }

}

//------------------------------------------------------------------------------
// Name:        set_mission_time
// Description: Sets the time since a mission was started.
// Arguments:   - time: time in seconds since a mission was started.
//------------------------------------------------------------------------------
void Vehicle::set_mission_time(int time)
{
    mission_time = time;
}

//--------------------------------------------------------------------------
// Name:        set_mission_distance
// Description: Sets the mission distance.
// Arguments:   - distance: distance in Km.
//--------------------------------------------------------------------------
void Vehicle::set_mission_distance(double distance)
{
    mission_distance = distance;
}

//--------------------------------------------------------------------------
// Name:        set_mission_duration
// Description: Sets the mission distance.
// Arguments:   - distance: distance in Km.
//--------------------------------------------------------------------------
void Vehicle::set_mission_duration(double duration)
{
    mission_duration = duration;
}


//------------------------------------------------------------------------------
// Name:        save_mission
// Description: Saves the vehicle's mission to file.
//------------------------------------------------------------------------------
void Vehicle::save_mission(QString filepath)
{

    using namespace pugi;
    xml_document doc;
    xml_node mission_node = doc.append_child("mission");

    for (Task* task: mission.get_all())
    {

        xml_node task_node = mission_node.append_child("task");
        task_node.append_attribute("duration") = task->get_duration();
        task_node.append_attribute("type") =     TaskType::to_string(task->get_type()).c_str();
        task_node.append_attribute("roll") =     task->get_roll();
        task_node.append_attribute("pitch") =    task->get_pitch();
        task_node.append_attribute("yaw") =      task->get_yaw();
        task_node.append_attribute("vx") =       task->get_vx();
        task_node.append_attribute("vy") =       task->get_vy();
        task_node.append_attribute("vz") =       task->get_vz();
        task_node.append_attribute("depth") =    task->get_depth();
        task_node.append_attribute("height") =   task->get_height();
        task_node.append_attribute("rpm") =      task->get_rpm();
        task_node.append_attribute("dive") =     task->get_dive();
        task_node.append_attribute("command") =  ActionType::to_string(task->get_command()).c_str();

        QVector<std::pair<QPointF, ActionType::Value>> points = task->get_points();
        for (auto point : points)
        {
            xml_node point_node = task_node.append_child("point");
            point_node.append_attribute("latitude") = point.first.y();
            point_node.append_attribute("longitude") = point.first.x();
            point_node.append_attribute("yaw") = NAN;
            point_node.append_attribute("pcommand") = ActionType::to_string(point.second).c_str();
        }

    }

    doc.save_file(filepath.toStdString().c_str());

}

//------------------------------------------------------------------------------
// Name:        load_mission
// Description: Loads the vehicle's mission from file.
//------------------------------------------------------------------------------
void Vehicle::load_mission(QString filepath)
{

    using namespace pugi;
    xml_document doc;
    xml_parse_result result = doc.load_file(filepath.toStdString().c_str());

    mission.clear();

    if (result)
    {
        for (xml_node task_node: doc.child("mission").children("task"))
        {

            mission.append();
            Task* task = mission.get(mission.size() - 1);

            task->set_duration(task_node.attribute("duration").as_double());
            task->set_type(TaskType::from_string(std::string(task_node.attribute("type").as_string())));
            task->set_roll(task_node.attribute("roll").as_double());
            task->set_pitch(task_node.attribute("pitch").as_double());
            task->set_yaw(task_node.attribute("yaw").as_double());
            task->set_vx(task_node.attribute("vx").as_double());
            task->set_vy(task_node.attribute("vy").as_double());
            task->set_vz(task_node.attribute("vz").as_double());
            task->set_depth(task_node.attribute("depth").as_double());
            task->set_height(task_node.attribute("height").as_double());
            task->set_rpm(task_node.attribute("rpm").as_double());
            task->set_dive(task_node.attribute("dive").as_bool());
            task->set_command(ActionType::from_string(task_node.attribute("command").as_string()));

            for (xml_node point_node: task_node.children("point"))
            {
                task->add_point(QPointF(point_node.attribute("longitude").as_double(),
                                        point_node.attribute("latitude").as_double()),
                                ActionType::from_string(point_node.attribute("pcommand").as_string()));
            }

        }
    }
    else
        qDebug() << "failed to load mission file (" << result.description() << ")";

}

//------------------------------------------------------------------------------
// Name:        send_mission
// Description: Sends the mission to the vehicle.
//------------------------------------------------------------------------------
void Vehicle::send_mission(CommsChannel::Value comms_channel,
                           int vehicle_id)
{
    if (mission.size() > 0)
        send_add_mission(&mission, comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        read_mission
// Description: Reads the mission from the vehicle.
//------------------------------------------------------------------------------
void Vehicle::read_mission(CommsChannel::Value comms_channel,
                           int vehicle_id)
{
    // TODO: Implement this
    send_read_mission(comms_channel, vehicle_id);
}

//------------------------------------------------------------------------------
// Name:        read_params
// Description: Reads the params from the vehicle.
//------------------------------------------------------------------------------
void Vehicle::read_params(CommsChannel::Value comms_channel,
                           int vehicle_id)
{
    send_read_params(comms_channel, vehicle_id);
}

//--------------------------------------------------------------------------
// Name:        send_params
// Description: Sends the params from the vehicle.
//--------------------------------------------------------------------------
void Vehicle::send_params(CommsChannel::Value comms_channel, int vehicle_id)
{
    if(parameters.size() > 0)
        send_write_params(&parameters, comms_channel, vehicle_id);
}

//--------------------------------------------------------------------------
// Name:        append_param
// Description: Appends a new parameter in the param list.
// Arguments:   - name:  name of the parameter.
//              - type:  the datatype of the parameter.
//              - value: value of the parameter.
//--------------------------------------------------------------------------
void Vehicle::append_param(std::string name, std::string type, QVariant value)
{
    param current{name, type, value};
    // Change later
    param_list.push_back(current);
    // Change later

    parameters.append(name, type, value);
}

//--------------------------------------------------------------------------
// Name:        clear_param_list
// Description: Clears previous parameter list if any.
//--------------------------------------------------------------------------
void Vehicle::clear_param_list()
{
    // remove later
    param_list.clear();
    // remove later

    parameters.clear();
}

//--------------------------------------------------------------------------
// Name:        print_param_list
// Description: Prints the parameter list as received from the vehicle.
//--------------------------------------------------------------------------
void Vehicle::print_param_list()
{
    for(auto current_param:param_list)
    {

        qDebug () << QString::fromStdString(current_param.name) << " "<<
                     QString::fromStdString(current_param.type) << " " <<
                     current_param.value;
    }
}


//------------------------------------------------------------------------------
// Name:        mission_changed
// Description: Slot that is called when the vehicle's mission changes.
//------------------------------------------------------------------------------
void Vehicle::mission_changed()
{
    get_mission_graphic(mission_overlay.get(), &mission, color);

    // Update Mission Distance and Time
    mission_distance = 0.0;
    mission_duration = 0.0;

    QVector<Task*> current_tasks = get_mission()->get_all();
    for(auto curr_task:current_tasks)
    {
        QVector<std::pair<QPointF, ActionType::Value>> current_points = curr_task->get_points();
        for(int i = 1; i < current_points.size(); i++)
        {
            // Calculate the distance between the current and the point before it
            mission_distance += calculate_distance(current_points[i].first,
                                                   current_points[i-1].first);
        }
        mission_duration += curr_task->get_duration();
    }
    emit missionDistanceChanged(id, mission_distance);
    emit missionDurationChanged(id, mission_duration);
}

double Vehicle::calculate_distance(QPointF start, QPointF end)
{
    double R = 6371; // Radius of the earth in km
    double dLat = degree2rad(end.x() - start.x());  // deg2rad below
    double dLon = degree2rad(end.y() - start.y());
    double a =
            sin(dLat/2) * sin(dLat/2) +
            cos(degree2rad(start.x())) * cos(degree2rad(end.x())) *
            sin(dLon/2) * sin(dLon/2);

    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double d = R * c; // Distance in km
    return d;
}

double Vehicle::degree2rad(double deg)
{
    double pi = 3.14159265;
    return deg * (pi/180);
}

//------------------------------------------------------------------------------
// Name:        mission_time_timer_timeout
// Description: Slot that is called when the mission time timer times out.
//              Increments the mission time.
//------------------------------------------------------------------------------
void Vehicle::mission_time_timer_timeout()
{
    mission_time = mission_time + 1;
    emit missionTimeChanged(id, mission_time);
}

//--------------------------------------------------------------------------
// Name:        parse_populate_mission
// Description: parses and populates the mission as received from the vehicle.
// Arguments:   - received_mission: the mission received through comms.
//--------------------------------------------------------------------------
void Vehicle::parse_populate_mission(Mission* received_mission)
{
    mission.clear();
    QVector<Task*> received_tasks = received_mission->get_all();

    for(auto task : received_tasks)
         mission.append(task);

    mission_changed();

}

void Vehicle::geofence_changed(QVector<QPointF> geofencePoints)
{
    for(auto point:geofencePoints)
        qDebug() <<point.x()<<point.y()<<endl;
}
