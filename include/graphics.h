//==============================================================================
// Autonomous Vehicle Library
//
// Description: Utility functions for creating graphics for display on a map.
//==============================================================================

#ifndef GRAPHICS_H
#define GRAPHICS_H

// Mission class for creation of mission graphics
#include "mission.h"

// C++ includes
#include <memory>

// Qt includes
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QQueue>
#include <QPoint>

// ArcGIS includes
#include <GraphicsOverlay.h>
#include <Graphic.h>
#include <Point.h>
#include <PolylineBuilder.h>
#include <SpatialReference.h>
#include <SimpleMarkerSymbol.h>
#include <TextSymbol.h>
#include <CompositeSymbol.h>
#include <MultilayerPointSymbol.h>
#include <PictureMarkerSymbol.h>
#include <PolygonBuilder.h>
#include <LineSegment.h>
#include "GeometryEngine.h"

using namespace Esri::ArcGISRuntime;

const double PI = 3.141592;

//==============================================================================
//                                   DEFINES
//==============================================================================

// z-index for each graphic type to ensure the correct draw order
#define SWATH_LINE_Z_INDEX       0
#define GEOFENCE_POINT_Z_INDEX   1
#define GEOFENCE_OUTLINE_Z_INDEX 2
#define WAYPOINT_Z_INDEX         3
#define VEHICLE_PATH_Z_INDEX     4
#define VEHICLE_ICON_Z_INDEX     5

//==============================================================================
//                             FUNCTION DECLARATIONS
//==============================================================================

//------------------------------------------------------------------------------
// Name:        get_icon_in_color
// Description: Gets the image in the specified color.
// Arguments:   - resource_name: resource name string
//              - color: icon color
// Returns:     Colored icon.
//------------------------------------------------------------------------------
inline QImage get_icon_in_color(QString resource_name, QColor color)
{

    // Load image to use as alpha map
    QPixmap pixmap = QPixmap(resource_name);

    // Initialize painter to draw on a pixmap and set composition mode
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.setBrush(color);
    painter.setPen(color);
    painter.drawRect(pixmap.rect());

    return pixmap.toImage();

}

//------------------------------------------------------------------------------
// Name:        get_vehicle_icon_graphic
// Description: Gets a graphic representing a vehicle at a location with a yaw.
// Arguments:   - lat: vehicle location latitude in degrees
//              - lat: vehicle location longitude in degrees
//              - yaw: vehicle yaw in degrees
//              - color: vehicle icon color
// Returns:     Graphic representing the vehicle.
//------------------------------------------------------------------------------
inline std::shared_ptr<Graphic> get_vehicle_icon_graphic(double lat, double lon, double yaw, QColor color)
{

    // Create the vehicle outline symbol in white
    QImage vehicle_icon_outline = get_icon_in_color(":/resources/gps_icon.png", QColor(255, 255, 255));
    PictureMarkerSymbol vehicle_outline_symbol(vehicle_icon_outline);
    vehicle_outline_symbol.setHeight(55);
    vehicle_outline_symbol.setWidth(55);
    vehicle_outline_symbol.setAngle(static_cast<float>(yaw));

    // Create the vehicle colored symbol in the given color
    QImage vehicle_color_icon = get_icon_in_color(":/resources/gps_icon.png", color);
    PictureMarkerSymbol vehicle_color_symbol(vehicle_color_icon);
    vehicle_color_symbol.setHeight(40);
    vehicle_color_symbol.setWidth(40);
    vehicle_color_symbol.setAngle(static_cast<float>(yaw));

    // Create a composite symbol containing the vehicle symbol components
    QList<Symbol*> symbol_list;
    symbol_list.append(&vehicle_outline_symbol);
    symbol_list.append(&vehicle_color_symbol);
    CompositeSymbol vehicle_symbol(symbol_list);

    // Construct a point in WGS84 using the given lat and lon
    Point vehicle_position = Point(lon, lat, SpatialReference::wgs84());

    // Create a graphic from the position and symbol
    std::shared_ptr<Graphic> vehicle_icon_graphic =
            std::shared_ptr<Graphic>(new Graphic(vehicle_position, &vehicle_symbol));
    vehicle_icon_graphic->setZIndex(VEHICLE_ICON_Z_INDEX);
    return vehicle_icon_graphic;

}

//------------------------------------------------------------------------------
// Name:        get_vehicle_path_graphic
// Description: Gets a graphic representing a vehicle at a location with a yaw.
// Arguments:   - path: queue of points forming the vehicle path
//              - color: vehicle path color
// Returns:     Graphic representing the vehicle.
//------------------------------------------------------------------------------
inline std::shared_ptr<Graphic> get_vehicle_path_graphic(QQueue<Point> path, QColor color)
{

    // Create a polyline from the points in the path
    PolylineBuilder path_polyline_builder(SpatialReference::wgs84());
    for (Point point: path)
        path_polyline_builder.addPoint(point);
    Polyline path_polyline = path_polyline_builder.toPolyline();

    // Create the path symbol in the given color
    SimpleLineSymbol path_symbol(SimpleLineSymbolStyle::Solid, color, 3);

    // Create and return a graphic from the polyline and symbol
    std::shared_ptr<Graphic> vehicle_path_graphic =
            std::shared_ptr<Graphic>(new Graphic(path_polyline, &path_symbol));
    vehicle_path_graphic->setZIndex(VEHICLE_PATH_Z_INDEX);
    return vehicle_path_graphic;

}

//------------------------------------------------------------------------------
// Name:        get_vehicle_path_graphic
// Description: Gets a graphic representing a vehicle and its path..
// Arguments:   - graphics_overlay: pointer to graphics overlay to add vehicle
//                graphics to.
//              - path: queue of points forming the vehicle path
//              - yaw: vehice yaw in degrees
//              - color: vehicle path color
//------------------------------------------------------------------------------
inline void get_vehicle_graphic(std::shared_ptr<GraphicsOverlay> graphics_overlay,
                                QQueue<Point> path, double yaw, QColor color)
{

    // Get new graphics for the new path and vehicle icon
    std::shared_ptr<Graphic> path_graphic =
            get_vehicle_path_graphic(path, color);
    std::shared_ptr<Graphic> icon_graphic =
            get_vehicle_icon_graphic(path.last().y(), path.last().x(), yaw, color);

    // If the graphics overlay is empty, add the new graphics, otherwise update
    // the existing graphics
    if (graphics_overlay->graphics()->size() == 0)
    {
        graphics_overlay->graphics()->append(path_graphic.get());
        graphics_overlay->graphics()->append(icon_graphic.get());
    }
    else
    {
        graphics_overlay->graphics()->at(0)->setGeometry(path_graphic->geometry());
        graphics_overlay->graphics()->at(0)->setSymbol(path_graphic->symbol());
        graphics_overlay->graphics()->at(1)->setGeometry(icon_graphic->geometry());
        graphics_overlay->graphics()->at(1)->setSymbol(icon_graphic->symbol());
    }

}

//------------------------------------------------------------------------------
// Name:        get_waypoint_graphic
// Description: Gets a graphic representing a waypoint at a given location.
// Arguments:   - lat: waypoint location latitude in degrees
//              - lon: waypoint location longitude in degrees
//              - number: waypoint number
// Returns:     Graphic representing the waypoint.
//------------------------------------------------------------------------------
inline std::shared_ptr<Graphic> get_waypoint_graphic(double lat, double lon, int number, QColor color)
{

    // Create a point representing the waypoint location
    Point waypoint_location(lon, lat, SpatialReference::wgs84());

    // Set up the symbols representing the waypoint. A waypoint is a white
    // circle with text inside holding the waypoint number
    SimpleMarkerSymbol waypoint_border_symbol(SimpleMarkerSymbolStyle::Circle, color, 31);
    SimpleMarkerSymbol waypoint_marker_symbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::white), 25);
    TextSymbol waypoint_text_symbol(QString::number(number), QColor(Qt::black), 13,
                                    HorizontalAlignment::Center, VerticalAlignment::Middle);
    waypoint_text_symbol.setFontWeight(FontWeight::Bold);

    // Create a composite symbol containing the waypoint marker and the text
    QList<Symbol*> symbol_list;
    symbol_list.append(&waypoint_border_symbol);
    symbol_list.append(&waypoint_marker_symbol);
    symbol_list.append(&waypoint_text_symbol);
    CompositeSymbol waypoint_symbol(symbol_list);

    // Create and return a graphic from the waypoint location and symbol
    std::shared_ptr<Graphic> waypoint_graphic = std::shared_ptr<Graphic>(new Graphic(waypoint_location, &waypoint_symbol));
    waypoint_symbol.setProperty("test", 1338);
    waypoint_graphic->setProperty("test", 1337);
    waypoint_graphic->setZIndex(WAYPOINT_Z_INDEX);
    return waypoint_graphic;

}

//------------------------------------------------------------------------------
// Name:        get_waypoint_graphic
// Description: Gets a graphic representing a waypoint at a given location.
// Arguments:   - lat: waypoint location latitude in degrees
//              - lon: waypoint location longitude in degrees
//              - number: waypoint number
// Returns:     Graphic representing the waypoint.
//------------------------------------------------------------------------------
inline std::shared_ptr<Graphic> get_zone_waypoint_graphic(double lat, double lon, int number, QColor color)
{

    // Create a point representing the waypoint location
    Point waypoint_location(lon, lat, SpatialReference::wgs84());

    // Set up the symbols representing the waypoint. A waypoint is a white
    // circle with text inside holding the waypoint number
    SimpleMarkerSymbol waypoint_border_symbol(SimpleMarkerSymbolStyle::Circle, color, 31);
    SimpleMarkerSymbol waypoint_marker_symbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::white), 25);
    TextSymbol waypoint_text_symbol("z" + QString::number(number), QColor(Qt::black), 13,
                                    HorizontalAlignment::Center, VerticalAlignment::Middle);
    waypoint_text_symbol.setFontWeight(FontWeight::Bold);

    // Create a composite symbol containing the waypoint marker and the text
    QList<Symbol*> symbol_list;
    symbol_list.append(&waypoint_border_symbol);
    symbol_list.append(&waypoint_marker_symbol);
    symbol_list.append(&waypoint_text_symbol);
    CompositeSymbol waypoint_symbol(symbol_list);

    // Create and return a graphic from the waypoint location and symbol
    std::shared_ptr<Graphic> waypoint_graphic = std::shared_ptr<Graphic>(new Graphic(waypoint_location, &waypoint_symbol));
    waypoint_symbol.setProperty("test", 1338);
    waypoint_graphic->setProperty("test", 1337);
    waypoint_graphic->setZIndex(WAYPOINT_Z_INDEX);
    return waypoint_graphic;

}

//------------------------------------------------------------------------------
// Name:        get_geofence_graphic
// Description: Gets a graphic representing a point at a given lat and lon.
// Arguments:   - lat: latitude of point in degrees
//              - lon: longitude of point in degrees
// Returns:     Graphic representing the geofence point.
//------------------------------------------------------------------------------
inline std::shared_ptr<Graphic> get_geofence_graphic(double lat, double lon)
{
    Point geofence_point_location(lon, lat, SpatialReference::wgs84());

    SimpleMarkerSymbol geofence_outline_point_symbol(SimpleMarkerSymbolStyle::Square,
                                   QColor(Qt::red), 10);
    std::shared_ptr<Graphic> geofence_point_graphic = std::shared_ptr<Graphic>(new Graphic(geofence_point_location, &geofence_outline_point_symbol));

    geofence_point_graphic->setZIndex(GEOFENCE_POINT_Z_INDEX);
    //graphics_overlay->graphics()->append(geofence_point_graphic.get());
    return geofence_point_graphic;
}

//------------------------------------------------------------------------------
// Name:        get_geofence_outline_graphic
// Description: Gets a graphic representing a swath line between two locations.
// Arguments:   - lat1: start point latitude in degrees
//              - lon1: start point longitude in degrees
//              - lat1: end point latitude in degrees
//              - lon1: end point longitude in degrees
//              - color: swath line coloe
// Returns:     Graphic representing the swath line.
//------------------------------------------------------------------------------
inline std::shared_ptr<Graphic> get_geofence_outline_graphic(QVector<QPointF> geofence_points)
{
    SimpleLineSymbol geofence_outline_symbol(SimpleLineSymbolStyle::Solid, QColor(Qt::red), 2);
    PolygonBuilder polygonBuilder(SpatialReference::wgs84());
    for(auto point:geofence_points)
        polygonBuilder.addPoint(point.x(), point.y());
    Polygon polygon(polygonBuilder.toGeometry());
    Polyline polygon_outline = polygon.toPolyline();

    std::shared_ptr<Graphic> geofence_outline_graphic = std::shared_ptr<Graphic>(
                new Graphic(polygon, &geofence_outline_symbol));

    geofence_outline_graphic->setZIndex(GEOFENCE_OUTLINE_Z_INDEX);
    return geofence_outline_graphic;
}



//------------------------------------------------------------------------------
// Name:        get_zone_outline_graphicl
// Description: Gets a graphic representing a envolope with the given points.
// Arguments:   - points: vertices of the polygon
//              - color: swath line coloe
// Returns:     Graphic representing the zone outline.
//------------------------------------------------------------------------------
inline std::shared_ptr<Graphic> get_zone_outline_graphic(QVector<QPointF> points, QColor color)
{
    SimpleLineSymbol zone_outline_symbol(SimpleLineSymbolStyle::Solid, color, 3);

    PolygonBuilder polygonBuilder(SpatialReference::wgs84());

    for(auto point:points)
        polygonBuilder.addPoint(point.x(), point.y());

    Polygon polygon(polygonBuilder.toGeometry());
    Polyline polygon_outline = polygon.toPolyline();

    std::shared_ptr<Graphic> zone_outline_graphic = std::shared_ptr<Graphic>(
                new Graphic(polygon, &zone_outline_symbol));

    zone_outline_graphic->setZIndex(SWATH_LINE_Z_INDEX);
    return zone_outline_graphic;
}


//------------------------------------------------------------------------------
// Name:        get_swath_line_graphic
// Description: Gets a graphic representing a swath line between two locations.
// Arguments:   - lat1: start point latitude in degrees
//              - lon1: start point longitude in degrees
//              - lat1: end point latitude in degrees
//              - lon1: end point longitude in degrees
//              - color: swath line coloe
// Returns:     Graphic representing the swath line.
//------------------------------------------------------------------------------
inline std::shared_ptr<Graphic> get_swath_line_graphic(double lat1, double lon1, double lat2, double lon2, QColor color)
{

    // Make the swath line partially transparent
    color.setAlpha(100);

    // Create the line symbol for the swath line
    SimpleLineSymbol swath_line_symbol(SimpleLineSymbolStyle::Solid, color, 3);

    // Create the points representing the lat/lon
    Point point1(lon1, lat1, SpatialReference::wgs84());
    Point point2(lon2, lat2, SpatialReference::wgs84());

    // Create a polyline from the points
    PolylineBuilder polyline_builder(SpatialReference::wgs84());
    polyline_builder.addPoint(point1);
    polyline_builder.addPoint(point2);
    Polyline swath_line = polyline_builder.toGeometry();

    // Create and return the swath line graphic
    std::shared_ptr<Graphic> swath_line_graphic = std::shared_ptr<Graphic>(new Graphic(swath_line, &swath_line_symbol));
    swath_line_graphic->setZIndex(SWATH_LINE_Z_INDEX);
    return swath_line_graphic;

}

//------------------------------------------------------------------------------
// Name:        get_task_graphic
// Description: Gets a graphic representing a task.
// Arguments:   - graphics_overlay: pointer to graphics overlay to add task
//                graphic to.
//              - task: task to generate graphic for
//              - color: task color
// Returns:     Graphic representing the task.
//------------------------------------------------------------------------------
inline void get_task_graphic(GraphicsOverlay* graphics_overlay, Task* task, QColor color)
{

    switch (task->get_type())
    {

        case TaskType::TASK_PRIMITIVE:
        {
            return;
        }

        case TaskType::TASK_WAYPOINT:
        {

            if (task->get_points().size() > 0)
            {
                // Create a waypoint graphic and add it to the overlay
                double lat = task->get_points()[0].first.y();
                double lon = task->get_points()[0].first.x();
                std::shared_ptr<Graphic> waypoint_graphic =
                        get_waypoint_graphic(lat, lon, 0, color);
                graphics_overlay->graphics()->append(waypoint_graphic.get());

            }

            break;

        }

        case TaskType::TASK_PATH:
        {

            // Loop through all points in the taskwaypoint_graphic
            QVector<std::pair<QPointF, ActionType::Value>> points = task->get_points();
            for (int i = 0; i < points.size(); i++)
            {

                // Create a waypoint graphic and add it to the overlay
                double lat = points.at(i).first.y();
                double lon = points.at(i).first.x();

                if (!std::isnan(lat) && !std::isnan(lon))
                {
                    std::shared_ptr<Graphic> waypoint_graphic =
                            get_waypoint_graphic(lat, lon, i, color);
                    graphics_overlay->graphics()->append(waypoint_graphic.get());

                    // If this is not the first point, it can be connected
                    // to the previous point by a line
                    if (i > 0)
                    {

                        // Create a swath line graphic and add it to the overlay
                        double prev_lat = points.at(i-1).first.y();
                        double prev_lon = points.at(i-1).first.x();
                        std::shared_ptr<Graphic> swath_line_graphic =
                                get_swath_line_graphic(prev_lat,
                                                       prev_lon,
                                                       lat,
                                                       lon,
                                                       color);
                        graphics_overlay->graphics()->append(swath_line_graphic.get());

                    }

                }

            }

            break;

        }
        case TaskType::TASK_ZONE:
        {

        // Loop through all points in the taskwaypoint_graphic
        QVector<std::pair<QPointF, ActionType::Value>> points = task->get_points();
        for (int i = 0; i < points.size(); i++)
        {

            // Create a waypoint graphic and add it to the overlay
            double lat = points.at(i).first.y();
            double lon = points.at(i).first.x();

            if (!std::isnan(lat) && !std::isnan(lon))
            {
                std::shared_ptr<Graphic> waypoint_graphic =
                        get_zone_waypoint_graphic(lat, lon, i, color);
                graphics_overlay->graphics()->append(waypoint_graphic.get());
            }

        }

        QVector<QPointF> only_points;
        for(auto point: points)
        {
            only_points.push_back(point.first);
        }

        std::shared_ptr<Graphic> zone_outline_graphic = get_zone_outline_graphic(only_points, color);
        graphics_overlay->graphics()->append(zone_outline_graphic.get());
         break;
        }

    }
    return;
}


inline void generate_survey_zone_points(Task* zone_task, Task* path_task, GraphicsOverlay* graphics_overlay)
{
    path_task->clear_points_silent();
    double swath_heading = zone_task->get_angle();
    double swath_width = zone_task->get_swath();
    if(zone_task->get_points().size() == 0 || swath_width == 0.0)
        return ;
    // Convert the QVector<QPointF> to QList<Point>
    QList<Point> zone_points;
    for(auto point:zone_task->get_points())
    {
        // Check for latitude and long inversion later
        zone_points.push_back(Point(point.first.x(), point.first.y()));
    }

    // Build a survey zone polygon and polyline from the list of survey zone outline
    // points. THe polygon will be filled as a background, and the polyline will
    // be rendered as an outline
    PolygonBuilder* polygonBuilder = new PolygonBuilder(SpatialReference::wgs84());
    polygonBuilder->addPoints(zone_points);
    Polygon polygon(polygonBuilder->toGeometry());
    Polyline polygon_outline = polygon.toPolyline();

    // Generate the initial scanline

    const double swath_heading_rad = (-zone_task->get_angle() + 90.0) * 3.141592/180.0;
    // Determine the envelope of the survey area polygon and draw it on the map
    Envelope envelope = polygon.extent();

    // We want to start from the upper left corner of the envelope and move through
    // the entire survey area envelope. The scan line must be the maximum length of
    // a line that fits in the envelope, which is the diagonal length
    Point envelope_corner_point;

    if (swath_heading >= 0.0 && swath_heading < 90.0)
        envelope_corner_point = Point(envelope.xMin(), envelope.yMax(), SpatialReference::wgs84());
    else if (swath_heading >= 90.0 && swath_heading < 180.0)
        envelope_corner_point = Point(envelope.xMax(), envelope.yMax(), SpatialReference::wgs84());
    else if (swath_heading >= 180.0 && swath_heading < 270.0)
        envelope_corner_point = Point(envelope.xMax(), envelope.yMin(), SpatialReference::wgs84());
    else
        envelope_corner_point = Point(envelope.xMin(), envelope.yMin(), SpatialReference::wgs84());

    const double envelope_max_length = sqrt(envelope.height()*envelope.height() +
                                            envelope.width()*envelope.width());


    // corner point at the swath heading angle
    LineSegment scan_line_segment1 = LineSegment::createLineAtAngleFromStartPoint(envelope_corner_point, swath_heading_rad, envelope_max_length);
    LineSegment scan_line_segment2 = LineSegment::createLineAtAngleFromStartPoint(envelope_corner_point, swath_heading_rad + PI, envelope_max_length);

    // Build a polyline connecting the two endpoints of the line segments to
    // complete form the initial scanline
    PolylineBuilder scan_line_builder(SpatialReference::wgs84());
    scan_line_builder.addPoint(scan_line_segment2.endPoint());
    scan_line_builder.addPoint(scan_line_segment1.endPoint());
    Polyline scan_line = scan_line_builder.toGeometry();

    // *****************************************************************************
    // Generate scanlines in the survey zone

    SimpleLineSymbol* swath_line_symbol = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, QColor(0, 255, 0, 255), 3);
    SimpleMarkerSymbol* waypoint_marker_symbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::white), 25);

    int waypoint_counter = 1;
    bool alternate = false;
    // Create scan lines offset by the swath width until they are no longer in
    // the survey zone envelope
    bool scan_line_in_zone;
    do
    {

        // Offset the scanline by the swath width and determine if it is still
        // within the survey zone
        scan_line = GeometryEngine::offset(scan_line, swath_width/6371000*180.0/3.141592, GeometryOffsetType::Mitered, 0, 0);
        scan_line_in_zone = GeometryEngine::intersects(scan_line, envelope);

        // Find all intersections of the scanline with the survey zone
        QList<Geometry> intersection_geometry = GeometryEngine::intersections(polygon_outline, scan_line);
        if (intersection_geometry.size() > 0)
        {

            // The first entry of the intersection results are intersection points.
            // If there are two intersections, the scan line is going through the
            // survey zone
            Multipoint intersection_points = intersection_geometry[0];
            ImmutablePointCollection point_collection = intersection_points.points();
            if (point_collection.size() == 2)
            {

                // Draw a swath line between the intersection points
                PolylineBuilder swath_line_builder(SpatialReference::wgs84());
                swath_line_builder.addPoint(point_collection.point(0));
                swath_line_builder.addPoint(point_collection.point(1));
                Polyline swath_line = swath_line_builder.toGeometry();
                Graphic* swath_line_graphic = new Graphic(swath_line, swath_line_symbol);
                swath_line_graphic->setZIndex(SWATH_LINE_Z_INDEX);

                // Draw waypoints at the intersection points
                Graphic* swath_start_waypoint = new Graphic(point_collection.point(0), waypoint_marker_symbol);
                Graphic* swath_end_waypoint = new Graphic(point_collection.point(1), waypoint_marker_symbol);
                swath_start_waypoint->setZIndex(SWATH_LINE_Z_INDEX);
                swath_end_waypoint->setZIndex(SWATH_LINE_Z_INDEX);

                if(!alternate)
                {   path_task->add_point_silent(QPointF(point_collection.point(0).x(), point_collection.point(0).y()));
                    path_task->add_point_silent(QPointF(point_collection.point(1).x(), point_collection.point(1).y()));

                    alternate = true;
                }
                else
                {
                    path_task->add_point_silent(QPointF(point_collection.point(1).x(), point_collection.point(1).y()));
                    path_task->add_point_silent(QPointF(point_collection.point(0).x(), point_collection.point(0).y()));

                    alternate = false;
                }

            }

        }

    } while (scan_line_in_zone);



}


//------------------------------------------------------------------------------
// Name:        get_mission_graphic
// Description: Gets a graphic representing a mission consisting of a series of
//              tasks.
// Arguments:   - graphics_overlay: pointer to graphics overlay to add mission
//                graphic to.
//              - mission: mission to generate graphic for
//              - color: task color
// Returns:     Graphic representing the mission.
//------------------------------------------------------------------------------
inline void get_mission_graphic(GraphicsOverlay* graphics_overlay, Mission* mission, QColor color)
{

    // Clear any existing graphics from the overlay
    graphics_overlay->graphics()->clear();

    for (int i = 0; i < mission->size(); i++)
    {
        get_task_graphic(graphics_overlay, mission->get(i), color);

        if(mission->get(i)->get_type() == TaskType::TASK_ZONE && mission->get(i)->get_points().size() > 2)
        {
            // Manipulate the next task to the zone task
            generate_survey_zone_points(mission->get(i), mission->get(i+1), graphics_overlay);
            get_task_graphic(graphics_overlay, mission->get(i+1), color);
            i++;
        }
    }
}




#endif // GRAPHICS_H
