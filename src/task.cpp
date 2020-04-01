//==============================================================================
// Autonomous Vehicle Library
//
// Description:
//==============================================================================

#include "task.h"

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//------------------------------------------------------------------------------
// Name:        declare_qml
// Description: Registers types with the QML engine.
//------------------------------------------------------------------------------
void Task::declare_qml()
{
    qmlRegisterType<TaskType>("Avl", 1, 0, "TaskType");
    qmlRegisterType<Task>("Avl", 1, 0, "Task");
}

//------------------------------------------------------------------------------
// Name:        Task constructor
// Description: Default constructor.
//------------------------------------------------------------------------------
Task::Task(QObject* parent) : QObject(parent)
{

}

//------------------------------------------------------------------------------
// Name:        Task destructor
// Description: Default destructor.
//------------------------------------------------------------------------------
Task::~Task()
{

}

TaskType::Value Task::get_type()
{
    return type;
}

void Task::set_type(TaskType::Value new_type)
{
    type = new_type;
    emit taskChanged();
}

void Task::set_type(int new_type)
{
    type = static_cast<TaskType::Value>(new_type);
    emit taskChanged();
}

ActionType::Value Task::get_command()
{
    return action;
}

void Task::set_command(ActionType::Value new_command)
{
    action = new_command;
    emit taskChanged();
}

void Task::set_command(int new_command)
{
    action = static_cast<ActionType::Value>(new_command);
    emit taskChanged();
}

QVector<std::pair<QPointF, ActionType::Value>> Task::get_points()
{
    return points;
}

void Task::add_point(QPointF new_point, ActionType::Value command)
{
    points.append(std::make_pair(new_point, command));
    emit taskChanged();
}

void Task::add_point_silent(QPointF new_point)
{
    points.append(std::make_pair(new_point, ActionType::ACTION_NO_ACTION));
}

void Task::edit_point(int index, QPointF new_point)
{
    if (points.size() > index)
        points[index].first = new_point;
    emit taskChanged();
}

void Task::set_point_lat(int index, double lat)
{
    points[index].first.setY(lat);
    emit taskChanged();
}

void Task::set_point_lon(int index, double lon)
{
    points[index].first.setX(lon);
    emit taskChanged();
}

void Task::set_point_command(int index, ActionType::Value new_command)
{
    points[index].second = new_command;
    emit taskChanged();
}

void Task::move_point_up(int index)
{
    points.move(index, index-1);
    emit taskChanged();
}

void Task::move_point_down(int index)
{
    points.move(index, index+1);
    emit taskChanged();
}

void Task::remove_point(int index)
{
    points.remove(index);
    emit taskChanged();
}

void Task::clear_points()
{
    points.clear();
    emit taskChanged();
}

void Task::clear_points_silent()
{
    points.clear();
}

avl::Packet Task::get_packet()
{

    std::vector<double> points_vect;
    for (auto point : points)
    {
        points_vect.push_back(point.first.y());
        points_vect.push_back(point.first.x());
        points_vect.push_back(std::nan(""));
        points_vect.push_back(point.second);
    }

    avl::Packet task_packet = TASK_PACKET();
    task_packet.add_field(TASK_DURATION(m_duration));
    task_packet.add_field(TASK_TYPE(static_cast<uint8_t>(type)));
    task_packet.add_field(TASK_ATTITUDE(m_roll, m_pitch, m_yaw));
    task_packet.add_field(TASK_VELOCITY(m_vx, m_vy, m_vz));
    task_packet.add_field(TASK_DEPTH(m_depth));
    task_packet.add_field(TASK_HEIGHT(m_height));
    task_packet.add_field(TASK_RPM(m_rpm));
    task_packet.add_field(TASK_DIVE(m_dive));
    task_packet.add_field(TASK_POINTS(points_vect));
    task_packet.add_field(TASK_COMMAND(static_cast<uint8_t>(action)));

    return task_packet;

}

double Task::get_duration()
{
    return m_duration;
}

double Task::get_roll()
{
    return m_roll;
}

double Task::get_pitch()
{
    return m_pitch;
}

double Task::get_yaw()
{
    return m_yaw;
}

double Task::get_vx()
{
    return m_vx;
}

double Task::get_vy()
{
    return m_vy;
}

double Task::get_vz()
{
    return m_vz;
}

double Task::get_depth()
{
    return m_depth;
}

double Task::get_height()
{
    return m_height;
}

double Task::get_swath()
{
    return m_swath;
}

double Task::get_angle()
{
    return m_angle;
}

double Task::get_rpm()
{
    return m_rpm;
}

double Task::get_dive()
{
    return m_dive;
}

void Task::set_duration(double duration)
{
    m_duration = duration;
    emit taskChanged();
}

void Task::set_roll(double roll)
{
    m_roll = roll;
    emit taskChanged();
}

void Task::set_pitch(double pitch)
{
    m_pitch = pitch;
    emit taskChanged();
}

void Task::set_yaw(double yaw)
{
    m_yaw = yaw;
    emit taskChanged();
}

void Task::set_vx(double vx)
{
    m_vx = vx;
    emit taskChanged();
}

void Task::set_vy(double vy)
{
    m_vy = vy;
    emit taskChanged();
}

void Task::set_vz(double vz)
{
    m_vz = vz;
    emit taskChanged();
}

void Task::set_depth(double depth)
{
    m_depth = depth;
    emit taskChanged();
}

void Task::set_height(double height)
{
    m_height = height;
    emit taskChanged();
}

void Task::set_rpm(double rpm)
{
    m_rpm = rpm;
    emit taskChanged();
}

void Task::set_dive(bool dive)
{
    m_dive = dive;
    emit taskChanged();
}

void Task::set_swath(double swath)
{
    m_swath = swath;
    emit taskChanged();
}

void Task::set_angle(double angle)
{
    m_angle = angle;
    emit taskChanged();
}

Task* Task::packet_to_task(avl::Packet task_packet)
{
    // Check for every possible task field. A task packet does not
    // need to have every field. Fields that are not present are set to
    // NaN in the task message

    Task* task = new Task();

    // If the task packet contains the field, put its value into the
    // task message
    if(task_packet.has_field(TASK_DURATION_DESC))
        task->set_duration(avl::from_bytes<double>(task_packet.get_field(TASK_DURATION_DESC).get_data()));

    if(task_packet.has_field(TASK_TYPE_DESC))
        task->set_type(avl::from_bytes<uint8_t>(task_packet.get_field(TASK_TYPE_DESC).get_data()));

    if(task_packet.has_field(TASK_ATTITUDE_DESC))
    {
        std::vector<uint8_t> attitude_data = task_packet.get_field(TASK_ATTITUDE_DESC).get_data();
        task->set_roll(avl::from_bytes<double>(avl::subvector(attitude_data, 0, 8)));
        task->set_pitch(avl::from_bytes<double>(avl::subvector(attitude_data, 8, 8)));
        task->set_yaw(avl::from_bytes<double>(avl::subvector(attitude_data, 16, 8)));
    }

    if(task_packet.has_field(TASK_VELOCITY_DESC))
    {
        std::vector<uint8_t> velocity_data = task_packet.get_field(TASK_VELOCITY_DESC).get_data();
        task->set_vx(avl::from_bytes<double>(avl::subvector(velocity_data, 0, 8)));
        task->set_vy(avl::from_bytes<double>(avl::subvector(velocity_data, 8, 8)));
        task->set_vz(avl::from_bytes<double>(avl::subvector(velocity_data, 16, 8)));
    }

    if(task_packet.has_field(TASK_DEPTH_DESC))
        task->set_depth(avl::from_bytes<double>(task_packet.get_field(TASK_DEPTH_DESC).get_data()));

    if(task_packet.has_field(TASK_HEIGHT_DESC))
        task->set_height(avl::from_bytes<double>(task_packet.get_field(TASK_HEIGHT_DESC).get_data()));

    if(task_packet.has_field(TASK_RPM_DESC))
        task->set_rpm(avl::from_bytes<double>(task_packet.get_field(TASK_RPM_DESC).get_data()));

    if(task_packet.has_field(TASK_DIVE_DESC))
        task->set_dive(avl::from_bytes<bool>(task_packet.get_field(TASK_DIVE_DESC).get_data()));

    if(task_packet.has_field(TASK_POINTS_DESC))
    {
        std::vector<double> task_points = avl::vector_from_bytes<double>(task_packet.get_field(TASK_POINTS_DESC).get_data());

        // Add points to the task
        for(size_t i = 0; i < task_points.size(); i = i+3)
        {
            task->add_point(QPointF(task_points[i+1], task_points[i]), ActionType::Value(task_points[i+2]));
        }
    }

    if(task_packet.has_field(TASK_COMMAND_DESC))
        task->set_command(avl::from_bytes<double>(task_packet.get_field(TASK_COMMAND_DESC).get_data()));

    return task;
}
