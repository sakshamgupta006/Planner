//==============================================================================
// Autonomous Vehicle Library
//
// Description:
//==============================================================================

#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "Point.h"
#include "GraphicsOverlay.h"
#include "SimpleMarkerSymbol.h"
#include "TextSymbol.h"
#include <iomanip>
#include <iostream>

// NAN value
#include <cmath>


using namespace Esri::ArcGISRuntime;

//==============================================================================
//                              CLASS DECLARATION
//==============================================================================

class Waypoint
{

public:

    //--------------------------------------------------------------------------
    // Name:        Waypoint constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    Waypoint()
    {

    }

    //--------------------------------------------------------------------------
    // Name:        Waypoint destructor
    // Description: Default virtual descructor.
    //--------------------------------------------------------------------------
    virtual ~Waypoint()
    {

    }

public:

    Point location;
    int type;
    double radius;
    bool is_selected;
    int waypoint_number;

    double roll =     nan("");
    double pitch =    nan("");
    double yaw =      nan("");
    double vx =       nan("");
    double vy =       nan("");
    double vz =       nan("");
    double lat =      nan("");
    double lon =      nan("");
    double altitude = nan("");
    double depth =    nan("");
    double height =   nan("");
    double rpm =      nan("");
    double duration = nan("");
    bool dive = false;

    QString lat_string;
    QString lon_string;
    QString roll_string;
    QString pitch_string;
    QString yaw_string;
    QString vx_string;
    QString vy_string;
    QString vz_string;
    QString altitude_string;
    QString depth_string;
    QString height_string;
    QString rpm_string;
    QString duration_string;

    SimpleMarkerSymbol* waypoint_graphic_symbol = nullptr;
    TextSymbol* waypoint_text_symbol = nullptr;

    Graphic* waypoint_graphic = nullptr;
    Graphic* waypoint_text = nullptr;

};

#endif // WAYPOINT_H

