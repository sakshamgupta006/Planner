//==============================================================================
// Autonomous Vehicle Library
//
// Description: A class representing a mission on a vehicle. Contains a vector
//              of tasks and provides functions to edit it. Also provides
//              functions to get the graphics required to visualize the mission
//              for display on an ArcGIS map.
//==============================================================================
#ifndef GEOFENCE_H
#define GEOFENCE_H

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

class Geofence : public QObject
{
    Q_OBJECT
public:
    //--------------------------------------------------------------------------
    // Name:        Params constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    Geofence(QObject* parent=nullptr);

    //--------------------------------------------------------------------------
    // Name:        Params destructor
    // Description: Default destructor.
    //--------------------------------------------------------------------------
    virtual ~Geofence();

    //--------------------------------------------------------------------------
    // Name:        size
    // Description: Gets the number of parameters in the parameter list.
    // Returns:     Number of tasks in the mission.
    //--------------------------------------------------------------------------
    int size();

    //--------------------------------------------------------------------------
    // Name:        append
    // Description: Appends a parameter to the parameter list.
    //--------------------------------------------------------------------------
    void append(double lat, double lon);

    //--------------------------------------------------------------------------
    // Name:        append
    // Description: Appends a parameter to the parameter list.
    //--------------------------------------------------------------------------
    void append(QPointF point);

    //--------------------------------------------------------------------------
    // Name:        get_all
    // Description: Gets a copy of the vector of params in the parameter list.
    // Returns:     Copy of the vector of parametes in the parameter list.
    //--------------------------------------------------------------------------
    QVector<QPointF> get_all();

    //--------------------------------------------------------------------------
    // Name:        remove
    // Description: Removes the parameter at the specified index from the
    //              parameter list.
    // Arguments:   - index: index of the parameter to remove
    //--------------------------------------------------------------------------
    void remove(int index);

    //--------------------------------------------------------------------------
    // Name:        clear
    // Description: Removes all parameters from the parameter list.
    //--------------------------------------------------------------------------
    void clear();

    //--------------------------------------------------------------------------
    // Name:        get
    // Description: Gets a copy of the param at the given index
    // Returns:     Copy of the param at the given index
    //--------------------------------------------------------------------------
    QPointF get(int index);



private:
    QVector<QPointF>* geofence_points = new QVector<QPointF>();



};
#endif // GEOFENCE_H
