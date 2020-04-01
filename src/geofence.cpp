//==============================================================================
// Autonomous Vehicle Library
//
// Description: A class representing a mission on a vehicle. Contains a vector
//              of tasks and provides functions to edit it. Also provides
//              functions to get the graphics required to visualize the mission
//              for display on an ArcGIS map.
//==============================================================================
#include "geofence.h"
//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//------------------------------------------------------------------------------
// Name:        Geofence constructor
// Description: Default constructor.
//------------------------------------------------------------------------------
Geofence::Geofence(QObject* parent) : QObject(parent)
{

}

//------------------------------------------------------------------------------
// Name:        Geofence destructor
// Description: Default destructor.
//------------------------------------------------------------------------------
Geofence::~Geofence()
{

}

//--------------------------------------------------------------------------
// Name:        size
// Description: Gets the number of parameters in the parameter list.
// Returns:     Number of tasks in the mission.
//--------------------------------------------------------------------------
int Geofence::size()
{
    return geofence_points->size();
}


//--------------------------------------------------------------------------
// Name:        append
// Description: Appends a parameter to the parameter list.
//--------------------------------------------------------------------------
void Geofence::append(double lat, double lon)
{
    geofence_points->append(QPointF(lat, lon));
}


//--------------------------------------------------------------------------
// Name:        append
// Description: Appends a parameter to the parameter list.
//--------------------------------------------------------------------------
void Geofence::append(QPointF point)
{
    geofence_points->append(point);
}

//--------------------------------------------------------------------------
// Name:        clear
// Description: Removes all parameters from the parameter list.
//--------------------------------------------------------------------------
void Geofence::clear()
{
    geofence_points->clear();
}

//--------------------------------------------------------------------------
// Name:        get_all
// Description: Gets a copy of the vector of Geofence in the parameter list.
// Returns:     Copy of the vector of parametes in the parameter list.
//--------------------------------------------------------------------------
QVector<QPointF> Geofence::get_all()
{
    return *geofence_points;
}

//--------------------------------------------------------------------------
// Name:        get
// Description: Gets a copy of the param at the given index
// Returns:     Copy of the param at the given index
//--------------------------------------------------------------------------
QPointF Geofence::get(int index)
{
    return geofence_points->at(index);
}
