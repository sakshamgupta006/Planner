//==============================================================================
// Autonomous Vehicle Library
//
// Description: A class representing a mission on a vehicle. Contains a vector
//              of tasks and provides functions to edit it. Also provides
//              functions to get the graphics required to visualize the mission
//              for display on an ArcGIS map.
//==============================================================================

#ifndef MISSION_H
#define MISSION_H

// QObject base class
#include <QObject>

// QVector class
#include <QVector>

// Task class
#include "task.h"

// Action class
#include "action_type.h"

// ArcGIS includes
#include <GraphicsOverlay.h>

// AVL command packets
#include "comms/avl_commands.h"

//==============================================================================
//                              CLASS DECLARATION
//==============================================================================

class Mission : public QObject
{

    Q_OBJECT

signals:

    //--------------------------------------------------------------------------
    // Name:        missionChanged
    // Description: Signal emitted when the mission changes.
    //--------------------------------------------------------------------------
    void missionChanged();

public:

    //--------------------------------------------------------------------------
    // Name:        Mission constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    Mission(QObject* parent=nullptr);

    //--------------------------------------------------------------------------
    // Name:        Mission destructor
    // Description: Default destructor.
    //--------------------------------------------------------------------------
    virtual ~Mission();

public:

    //--------------------------------------------------------------------------
    // Name:        size
    // Description: Gets the number of tasks in the mission.
    // Returns:     Number of tasks in the mission.
    //--------------------------------------------------------------------------
    int size();

    //--------------------------------------------------------------------------
    // Name:        append
    // Description: Appends a default task to the mission.
    //--------------------------------------------------------------------------
    void append();

    void append(Task* task);

    //--------------------------------------------------------------------------
    // Name:        get
    // Description: Gets a pointer to the task at the specified index in
    //              the mission.
    // Arguments:   - index: index of the task to get
    // Returns:     Pointer to the task at the given index.
    //--------------------------------------------------------------------------
    Task* get(int index);

    //--------------------------------------------------------------------------
    // Name:        get_all
    // Description: Gets a copy of the vector of tasks in the mission.
    // Returns:     Copy of the vector of tasks in the mission.
    //--------------------------------------------------------------------------
    QVector<Task*> get_all();

    //--------------------------------------------------------------------------
    // Name:        remove
    // Description: Removes the task at the specified index from the
    //              mission.
    // Arguments:   - index: index of the task to remove
    //--------------------------------------------------------------------------
    void remove(int index);

    //--------------------------------------------------------------------------
    // Name:        move_up
    // Description: Moves the specified task up one index, making it
    //              earlier in the mission.
    // Arguments:   - index: index of the task to move up
    //--------------------------------------------------------------------------
    void move_up(int index);

    //--------------------------------------------------------------------------
    // Name:        move_down
    // Description: Moves the specified task down one index, making it
    //              later in the mission.
    // Arguments:   - index: index of the task to move down
    //--------------------------------------------------------------------------
    void move_down(int index);

    //--------------------------------------------------------------------------
    // Name:        clear
    // Description: Removes all tasks from the mission.
    //--------------------------------------------------------------------------
    void clear();

    //--------------------------------------------------------------------------
    // Name:        get_task_packets
    // Description: Gets a vector of packets containing the mission tasks.
    // Returns:     Vector of packets containing the mission tasks
    //--------------------------------------------------------------------------
    std::vector<avl::Packet> get_task_packets();

private:

    // Vector of tasks forming the mission
    QVector<Task*> task_list;

private slots:

    //--------------------------------------------------------------------------
    // Name:        task_changed
    // Description: Slot called when one of the tasks in a mission changes.
    //              Simply emits a missionChanged signal.
    //--------------------------------------------------------------------------
    void task_changed();

};

#endif // MISSION_H
