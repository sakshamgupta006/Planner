//==============================================================================
// Autonomous Vehicle Library
//
// Description:
//==============================================================================

#ifndef TASK_H
#define TASK_H

// QObject base class
#include <QObject>
#include <QDebug>

// Task types
#include "task_type.h"
#include "action_type.h"

// QPointF class
#include <QPointF>

// NAN value
#include <cmath>

// AVL command packets
#include "comms/avl_commands.h"
#include "util/byte.h"
#include "util/vector.h"

//==============================================================================
//                              CLASS DECLARATION
//==============================================================================

class Task : public QObject
{

    Q_OBJECT

signals:

    //--------------------------------------------------------------------------
    // Name:        taskChanged
    // Description: Signal emitted when the mission changes via editing.
    //--------------------------------------------------------------------------
    void taskChanged();

public:

    // Property configuration
    Q_PROPERTY(double duration MEMBER m_duration READ get_duration WRITE set_duration)
    Q_PROPERTY(double roll     MEMBER m_roll     READ get_roll     WRITE set_roll)
    Q_PROPERTY(double pitch    MEMBER m_pitch    READ get_pitch    WRITE set_pitch)
    Q_PROPERTY(double yaw      MEMBER m_yaw      READ get_yaw      WRITE set_yaw)
    Q_PROPERTY(double vx       MEMBER m_vx       READ get_vx       WRITE set_vx)
    Q_PROPERTY(double vy       MEMBER m_vy       READ get_vy       WRITE set_vy)
    Q_PROPERTY(double vz       MEMBER m_vz       READ get_vz       WRITE set_vz)
    Q_PROPERTY(double depth    MEMBER m_depth    READ get_depth    WRITE set_depth)
    Q_PROPERTY(double height   MEMBER m_height   READ get_height   WRITE set_height)
    Q_PROPERTY(double rpm      MEMBER m_rpm      READ get_rpm      WRITE set_rpm)
    Q_PROPERTY(double swath    MEMBER m_swath    READ get_swath    WRITE set_swath)
    Q_PROPERTY(double angle    MEMBER m_angle    READ get_angle    WRITE set_angle)
    Q_PROPERTY(bool   dive     MEMBER m_dive     READ get_dive     WRITE set_dive)

    // Primitive guidance values
    double m_duration = std::nan("");
    double m_roll     = std::nan("");
    double m_pitch    = std::nan("");
    double m_yaw      = std::nan("");
    double m_vx       = std::nan("");
    double m_vy       = std::nan("");
    double m_vz       = std::nan("");
    double m_depth    = std::nan("");
    double m_height   = std::nan("");
    double m_rpm      = std::nan("");
    double m_swath    = 10;
    double m_angle    = 0;
    bool   m_dive     = false;

public:

    //--------------------------------------------------------------------------
    // Name:        declare_qml
    // Description: Registers types with the QML engine.
    //--------------------------------------------------------------------------
    static void declare_qml();

public:

    //--------------------------------------------------------------------------
    // Name:        Task constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    Task(QObject* parent=nullptr);

    //--------------------------------------------------------------------------
    // Name:        Task destructor
    // Description: Default destructor.
    //--------------------------------------------------------------------------
    virtual ~Task();

    TaskType::Value get_type();
    void set_type(TaskType::Value new_type);
    void set_type(int new_type);

    ActionType::Value get_command();
    void set_command(ActionType::Value new_command);
    void set_command(int new_command);

    QVector<std::pair<QPointF, ActionType::Value>> get_points();
    void add_point(QPointF new_point, ActionType::Value command = ActionType::ACTION_NO_ACTION);
    void add_point_silent(QPointF new_point);
    void edit_point(int index, QPointF new_point);
    void set_point_lat(int index, double lat);
    void set_point_lon(int index, double lon);
    void set_point_command(int index, ActionType::Value new_command);
    void move_point_up(int index);
    void move_point_down(int index);
    void remove_point(int index);
    void clear_points();
    void clear_points_silent();
    avl::Packet get_packet();
    static Task* packet_to_task(avl::Packet task_packet);

    //--------------------------------------------------------------------------
    // Name:        get_*
    // Description: Property getters.
    // Returns:     Property value.
    //--------------------------------------------------------------------------
    Q_INVOKABLE double get_duration();
    Q_INVOKABLE double get_roll();
    Q_INVOKABLE double get_pitch();
    Q_INVOKABLE double get_yaw();
    Q_INVOKABLE double get_vx();
    Q_INVOKABLE double get_vy();
    Q_INVOKABLE double get_vz();
    Q_INVOKABLE double get_depth();
    Q_INVOKABLE double get_height();
    Q_INVOKABLE double get_rpm();
    Q_INVOKABLE double get_dive();
    Q_INVOKABLE double get_swath();
    Q_INVOKABLE double get_angle();

    //--------------------------------------------------------------------------
    // Name:        set_*
    // Description: Property setters.
    // Arguments:   - property value
    //--------------------------------------------------------------------------
    void set_duration(double duration);
    void set_roll(double roll);
    void set_pitch(double pitch);
    void set_yaw(double yaw);
    void set_vx(double vx);
    void set_vy(double vy);
    void set_vz(double vz);
    void set_depth(double depth);
    void set_height(double height);
    void set_rpm(double rpm);
    void set_dive(bool dive);
    void set_swath(double swath);
    void set_angle(double angle);


private:

    // Task type
    TaskType::Value type = TaskType::TASK_PRIMITIVE;

    // Command Task
    ActionType::Value action = ActionType::ACTION_NO_ACTION;

    // List of points
    QVector<std::pair<QPointF, ActionType::Value>> points;

};

#endif // TASK_H
