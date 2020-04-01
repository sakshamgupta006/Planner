//==============================================================================
// Autonomous Vehicle Library
//
// Description: A QObject representing a vehicle. Contains a TCP connection to
// the vehicle, the vehicle's mission, and and vehicle status information.
//==============================================================================

#ifndef Vehicle_H
#define Vehicle_H

// VehicleConnection base class
#include "vehicle_connection.h"

// Vehicle commands
#include "comms/avl_commands.h"

// Vehicle status struct
#include "vehicle_status.h"

// Mission class
#include "mission.h"

// Vehicle types enum
#include "vehicle_type.h"

// Vehicle graphics generation
#include "graphics.h"

// QTimer class
#include <QTimer>

#include "param.h"

#include "geofence.h"

//==============================================================================
//                              CLASS DECLARATION
//==============================================================================

class Vehicle : public VehicleConnection
{

    Q_OBJECT

signals:

    //--------------------------------------------------------------------------
    // Name:        vehicleTypeChanged
    // Description: Signal that is emitted when the vehicle's type changes.
    // Arguments:   - vehicle_id: ID of the vehicle
    //              - new_type: new vehicle type
    //--------------------------------------------------------------------------
    void vehicleTypeChanged(int vehicle_id, VehicleType::Value new_type);

    //--------------------------------------------------------------------------
    // Name:        missionTimeChanged
    // Description: Signal that is emitted when the mission time changes.
    // Arguments:   - vehicle_id: ID of the vehicle
    //              - new_mission_time: new mission time in seconds
    //--------------------------------------------------------------------------
    void missionTimeChanged(int vehicle_id, int new_mission_time);

    //--------------------------------------------------------------------------
    // Name:        missionDistanceChanged
    // Description: Signal that is emitted when the mission distance changes.
    // Arguments:   - vehicle_id: ID of the vehicle
    //              - new_mission_distance: new mission distance in Kms
    //--------------------------------------------------------------------------
    void missionDistanceChanged(int vehicle_id, double new_mission_distance);

    //--------------------------------------------------------------------------
    // Name:        missionDurationChanged
    // Description: Signal that is emitted when the mission distance changes.
    // Arguments:   - vehicle_id: ID of the vehicle
    //              - new_mission_duration: new mission distance in Kms
    //--------------------------------------------------------------------------
    void missionDurationChanged(int vehicle_id, double new_mission_duration);




public:

    //--------------------------------------------------------------------------
    // Name:        Vehicle constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    Vehicle(QObject *parent=nullptr);

    //--------------------------------------------------------------------------
    // Name:        Vehicle constructor
    // Description: Constructs the vehicle with an IP address and port.
    // Arguments:   - ip_address: vehicle IP address
    //              - port: vehicle port
    //--------------------------------------------------------------------------
    Vehicle(QString ip_address, quint16 port, QObject *parent=nullptr);

    //--------------------------------------------------------------------------
    // Name:        Vehicle destructor
    // Description: Default destructor.
    //--------------------------------------------------------------------------
    ~Vehicle();

    //--------------------------------------------------------------------------
    // Name:        declare_qml
    // Description: Registers types with the QML engine.
    //--------------------------------------------------------------------------
    static void declare_qml();

    //--------------------------------------------------------------------------
    // Name:        connect_to_vehicle
    // Description: Opens a connection to the vehicle, retrying until a
    //              connection is established.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void connect_to_vehicle();

    //--------------------------------------------------------------------------
    // Name:        disconnect_from_vehicle
    // Description: Closes the connection to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void disconnect_from_vehicle();

    //--------------------------------------------------------------------------
    // Name:        append_vehicle_response
    // Description: Appends a vehicle response to the response list.
    // Arguments:   - response: response to append
    //--------------------------------------------------------------------------
    Q_INVOKABLE void append_vehicle_response(QString response);

    //--------------------------------------------------------------------------
    // Name:        clear_vehicle_responses
    // Description: Clears the list of vehicle responses.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void clear_vehicle_responses();

    //--------------------------------------------------------------------------
    // Name:        clear_path
    // Description: Clears the path representing the vehicle's location history.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void clear_path();

    //--------------------------------------------------------------------------
    // Name:        start_mission_timer
    // Description: Starts the vehicle's mission time timer.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void start_mission_timer();

    //--------------------------------------------------------------------------
    // Name:        reset_mission_timer
    // Description: Resets the vehicle's mission time timer.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void reset_mission_timer();



    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                                GETTERS
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //--------------------------------------------------------------------------
    // Name:        get_vehicle_id
    // Description: Gets the vehicle's ID number.
    // Returns:     Vehicle ID number.
    //--------------------------------------------------------------------------
    Q_INVOKABLE int get_vehicle_id();

    //--------------------------------------------------------------------------
    // Name:        get_vehicle_color
    // Description: Gets the vehicle's color.
    // Returns:     Vehicle color.
    //--------------------------------------------------------------------------
    Q_INVOKABLE QColor get_vehicle_color();

    //--------------------------------------------------------------------------
    // Name:        get_vehicle_type
    // Description: Gets the vehicle's type.
    // Returns:     Vehicle type.
    //--------------------------------------------------------------------------
    Q_INVOKABLE VehicleType::Value get_vehicle_type();

    //--------------------------------------------------------------------------
    // Name:        get_vehicle_status
    // Description: Gets the vehicle's status.
    // Returns:     Vehicle status struct.
    //--------------------------------------------------------------------------
    Q_INVOKABLE VehicleStatus get_vehicle_status();

    //--------------------------------------------------------------------------
    // Name:        get_mission
    // Description: Gets the vehicle's mission.
    // Returns:     Vehicle mission.
    //--------------------------------------------------------------------------
    Q_INVOKABLE Mission* get_mission();


    //--------------------------------------------------------------------------
    // Name:        get_params
    // Description: Gets the vehicle's params.
    // Returns:     Vehicle parameter.
    //--------------------------------------------------------------------------
    Q_INVOKABLE Params* get_params();

    //--------------------------------------------------------------------------
    // Name:        get_geofence
    // Description: Gets the vehicle's geofence.
    // Returns:     Vehicle parameter.
    //--------------------------------------------------------------------------
    Q_INVOKABLE Geofence* get_geofence();

    //--------------------------------------------------------------------------
    // Name:        get_mission_time
    // Description: Gets the time since a mission was started.
    // Returns:     Time in seconds since a mission was started.
    //--------------------------------------------------------------------------
    Q_INVOKABLE int get_mission_time();

    //--------------------------------------------------------------------------
    // Name:        get_mission_distance
    // Description: Gets the total mission distance.
    // Returns:     Distance in Km.
    //--------------------------------------------------------------------------
    Q_INVOKABLE double get_mission_distance();

    //--------------------------------------------------------------------------
    // Name:        get_mission_duration
    // Description: Gets the total mission duration.
    // Returns:     Duration in sec.
    //--------------------------------------------------------------------------
    Q_INVOKABLE double get_mission_duration();

    //--------------------------------------------------------------------------
    // Name:        get_vehicle_responses
    // Description: Gets a list of vehicle responses.
    // Returns:     Vector of vehicle response strings.
    //--------------------------------------------------------------------------
    Q_INVOKABLE QVector<QString> get_vehicle_responses();

    //--------------------------------------------------------------------------
    // Name:        get_path_overlay
    // Description: Gets a pointer to the vehicle's path overlay.
    // Returns:     Pointer to the vehicle's path overlay.
    //--------------------------------------------------------------------------
    Q_INVOKABLE std::shared_ptr<GraphicsOverlay> get_path_overlay();

    //--------------------------------------------------------------------------
    // Name:        get_mission_overlay
    // Description: Gets a pointer to the vehicle's mission overlay.
    // Returns:     Pointer to the vehicle's mission overlay.
    //--------------------------------------------------------------------------
    Q_INVOKABLE std::shared_ptr<GraphicsOverlay> get_mission_overlay();

    //--------------------------------------------------------------------------
    // Name:        get_geofence_overlay
    // Description: Gets a pointer to the vehicle's mission overlay.
    // Returns:     Pointer to the vehicle's mission overlay.
    //--------------------------------------------------------------------------
    Q_INVOKABLE std::shared_ptr<GraphicsOverlay> get_geofence_overlay();




    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                                SETTERS
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //--------------------------------------------------------------------------
    // Name:        set_vehicle_color
    // Description: Sets the vehicle's icon and path color.
    // Arguments:   - color: vehicle color
    //--------------------------------------------------------------------------
    Q_INVOKABLE void set_vehicle_color(QColor new_color);

    //--------------------------------------------------------------------------
    // Name:        set_vehicle_type
    // Description: Sets the vehicle's type.
    // Arguments:   - type: vehicle type string
    //--------------------------------------------------------------------------
    Q_INVOKABLE void set_vehicle_type(VehicleType::Value new_type);

    //--------------------------------------------------------------------------
    // Name:        set_vehicle_status
    // Description: Sets the vehicle's status.
    // Arguments:   - new_status: vehicle status
    //--------------------------------------------------------------------------
    Q_INVOKABLE void set_vehicle_status(VehicleStatus new_status);

    //--------------------------------------------------------------------------
    // Name:        set_mission_time
    // Description: Sets the time since a mission was started.
    // Arguments:   - time: time in seconds since a mission was started.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void set_mission_time(int time);

    //--------------------------------------------------------------------------
    // Name:        set_mission_distance
    // Description: Sets the mission distance.
    // Arguments:   - distance: distance in Km.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void set_mission_distance(double distance);

    //--------------------------------------------------------------------------
    // Name:        set_mission_duration
    // Description: Sets the mission distance.
    // Arguments:   - distance: distance in Km.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void set_mission_duration(double duration);


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //                            MISSION CONTROL
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //--------------------------------------------------------------------------
    // Name:        save_mission
    // Description: Saves the vehicle's mission to file.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void save_mission(QString filepath);

    //--------------------------------------------------------------------------
    // Name:        load_mission
    // Description: Loads the vehicle's mission from file.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void load_mission(QString filepath);

    //--------------------------------------------------------------------------
    // Name:        send_mission
    // Description: Sends the mission to the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_mission(CommsChannel::Value comms_channel,
                                  int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        read_mission
    // Description: Reads the mission from the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void read_mission(CommsChannel::Value comms_channel,
                                  int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        read_params
    // Description: Reads the params from the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void read_params(CommsChannel::Value comms_channel,
                                  int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        send_params
    // Description: Sends the params from the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void send_params(CommsChannel::Value comms_channel,
                                 int vehicle_id);

    //--------------------------------------------------------------------------
    // Name:        append_param
    // Description: Appends a new parameter in the param list.
    // Arguments:   - name:  name of the parameter.
    //              - type:  the datatype of the parameter.
    //              - value: value of the parameter.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void append_param(std::string name, std::string type,
                                  QVariant value);

    //--------------------------------------------------------------------------
    // Name:        clear_param_list
    // Description: Clears previous parameter list if any.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void clear_param_list();

    //--------------------------------------------------------------------------
    // Name:        print_param_list
    // Description: Prints the parameter list as received from the vehicle.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void print_param_list();

    //--------------------------------------------------------------------------
    // Name:        parse_populate_mission
    // Description: parses and populates the mission as received from the vehicle.
    // Arguments:   - mission: the mission received through comms.
    //--------------------------------------------------------------------------
    Q_INVOKABLE void parse_populate_mission(Mission* received_mission);

    Q_INVOKABLE double calculate_distance(QPointF start, QPointF end);

    Q_INVOKABLE double degree2rad(double deg);

    Params current_params;



private slots:

    //--------------------------------------------------------------------------
    // Name:        mission_changed
    // Description: Slot that is called when the vehicle's mission changes.
    //--------------------------------------------------------------------------
    void mission_changed();

    //--------------------------------------------------------------------------
    // Name:        mission_time_timer_timeout
    // Description: Slot that is called when the mission time timer times out.
    //              Increments the mission time.
    //--------------------------------------------------------------------------
    void mission_time_timer_timeout();

    void geofence_changed(QVector<QPointF> geofencePoints);

private:

    // Vehicle IP address, port, and ID number derived from the last three
    // numbers of its IP address
    QString ip_address;
    quint16 port;
    uint8_t id;
    std::vector<param> param_list;

    // Icon and path color
    QColor color = Qt::white;

    // Vehicle type
    VehicleType::Value type = VehicleType::VEHICLE_AUV;

    // Status struct containing vehicle status information. Gets updated by
    // incoming UDP and TCP messages through the vehicle manager
    VehicleStatus status;

    // Mission stored locally for the vehicle. This is not necessarily in
    // sync with the mission loaded on the vehicle
    Mission mission;

    Params parameters;

    Geofence geofence;

    // Timer and corresponding age variable for tracking time in seconds since
    // the mission was started
    QTimer* mission_time_timer = new QTimer(this);
    int mission_time = 0;
    double mission_distance = 0.0;
    double mission_duration = 0.0;

    // Vector of command responses from the vehicle
    QVector<QString> vehicle_responses;

    // Distance and heading from the deckbox
    double distance_from_deckbox;
    double heading_from_deckbox;

    // Queue of points representing the vehicle's path (location history)
    const int MAX_PATH_POINTS = 60000;
    QQueue<Point> path;

    // Graphics overlays containing the vehicle's path and mission
    // graphics
    std::shared_ptr<GraphicsOverlay> path_overlay;
    std::shared_ptr<GraphicsOverlay> mission_overlay;
    std::shared_ptr<GraphicsOverlay> geofence_overlay;

};

#endif // Vehicle_H
