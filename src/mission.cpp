//==============================================================================
// Autonomous Vehicle Library
//
// Description: A class representing a mission on a vehicle. Contains a vector
//              of tasks and provides functions to edit it. Also provides
//              functions to get the graphics required to visualize the mission
//              for display on an ArcGIS map.
//==============================================================================

#include "mission.h"

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//------------------------------------------------------------------------------
// Name:        Mission constructor
// Description: Default constructor.
//------------------------------------------------------------------------------
Mission::Mission(QObject* parent) : QObject(parent)
{

}

//------------------------------------------------------------------------------
// Name:        Mission destructor
// Description: Default destructor.
//------------------------------------------------------------------------------
Mission::~Mission()
{

}

//------------------------------------------------------------------------------
// Name:        size
// Description: Gets the number of tasks in the mission.
// Returns:     Number of tasks in the mission.
//------------------------------------------------------------------------------
int Mission::size()
{
    return task_list.size();
}

//------------------------------------------------------------------------------
// Name:        append
// Description: Appends a default task to the mission.
//------------------------------------------------------------------------------
void Mission::append()
{
    Task* new_task = new Task(this);
    task_list.append(new_task);
    connect(new_task, SIGNAL(taskChanged()), this, SLOT(task_changed()));
    emit missionChanged();
}

void Mission::append(Task* task)
{
    task_list.append(task);
    connect(task, SIGNAL(taskChanged()), this, SLOT(task_changed()));
    emit missionChanged();
}
//------------------------------------------------------------------------------
// Name:        get
// Description: Gets a copy of the task at the specified index in
//              the mission.
// Arguments:   - index: index of the task to get
// Returns:     Copy of the  task at the given index.
//------------------------------------------------------------------------------
Task* Mission::get(int index)
{
    return task_list[index];
}

//------------------------------------------------------------------------------
// Name:        get_all
// Description: Gets a copy of the vector of tasks in the mission.
// Returns:     Copy of the vector of tasks in the mission.
//------------------------------------------------------------------------------
QVector<Task*> Mission::get_all()
{
    return task_list;
}

//------------------------------------------------------------------------------
// Name:        remove
// Description: Removes the task at the specified index from the
//              mission.
// Arguments:   - index: index of the task to remove
//------------------------------------------------------------------------------
void Mission::remove(int index)
{
    task_list.remove(index);
    emit missionChanged();
}

//------------------------------------------------------------------------------
// Name:        move_up
// Description: Moves the specified task up one index, making it
//              earlier in the mission.
// Arguments:   - index: index of the task to move up
//------------------------------------------------------------------------------
void Mission::move_up(int index)
{
    task_list.move(index, index - 1);
    emit missionChanged();
}

//------------------------------------------------------------------------------
// Name:        move_down
// Description: Moves the specified task down one index, making it
//              later in the mission.
// Arguments:   - index: index of the task to move down
//------------------------------------------------------------------------------
void Mission::move_down(int index)
{
    task_list.move(index, index + 1);
    emit missionChanged();
}

//------------------------------------------------------------------------------
// Name:        clear
// Description: Removes all tasks from the mission.
//------------------------------------------------------------------------------
void Mission::clear()
{
    task_list.clear();
    emit missionChanged();
}


//------------------------------------------------------------------------------
// Name:        get_task_packets
// Description: Gets a vector of packets containing the mission tasks.
// Returns:     Vector of packets containing the mission tasks
//------------------------------------------------------------------------------
std::vector<avl::Packet> Mission::get_task_packets()
{
    std::vector<avl::Packet> task_packets;
    for (Task* task : get_all())
    {
        if(!(task->get_type() == TaskType::TASK_ZONE))
            task_packets.push_back(task->get_packet());
    }
    return task_packets;
}

//------------------------------------------------------------------------------
// Name:        task_changed
// Description: Slot called when one of the tasks in a mission changes.
//              Simply emits a missionChanged signal.
//------------------------------------------------------------------------------
void Mission::task_changed()
{
    emit missionChanged();
}
