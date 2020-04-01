//==============================================================================
// Autonomous Vehicle Library
//
// Description: QQuickComponent that displays an ArcGIS map with a satellite
//              imagery base map and ENC layers on top.
//==============================================================================

#include "avl_map_display.h"

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//------------------------------------------------------------------------------
// Name:        DisplayMap constructor
// Description: Default constructor.
//------------------------------------------------------------------------------
AvlMapDisplay::AvlMapDisplay(QQuickItem* parent) : QQuickItem(parent)
{

}

//------------------------------------------------------------------------------
// Name:        DisplayMap destructor
// Description: Default destructor.
//------------------------------------------------------------------------------
AvlMapDisplay::~AvlMapDisplay() = default;

//------------------------------------------------------------------------------
// Name:        declare_qml
// Description: Registers types with the QML engine.
//------------------------------------------------------------------------------
void AvlMapDisplay::declare_qml()
{
    qmlRegisterType<MapQuickView>("avl.gui", 1, 0, "AvlMapView");
    qmlRegisterType<AvlMapDisplay>("avl.gui", 1, 0, "AvlMapDisplay");
}

//------------------------------------------------------------------------------
// Name:        componentComplete
// Description: Called after the component is instantiated. Sets up the base map
//              and starts loading the ENC exchange set.
//------------------------------------------------------------------------------
void AvlMapDisplay::componentComplete()
{

    // Call the base class component complete function before adding this
    // class's actions as per the QQuickItem documentation
    QQuickItem::componentComplete();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Configure the base map

    // Get the map view child of this component so that the map can be added
    map_view = findChild<MapQuickView*>("avl_map_view");

    // Create a base map and add it to the map.  The ENC layer will be added
    // later when it finishes loading
    Basemap* basemap = Basemap::imageryWithLabels(this);
    map = new Map(basemap, this);
    map_view->setMap(map);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Add a weather layer

    // Create an image layer for weather display and add it to the map
    QUrl weather_server_url("https://nowcoast.noaa.gov/arcgis/rest/services/nowcoast/radar_meteo_imagery_nexrad_time/MapServer");
    ArcGISMapImageLayer* mapImageLayer = new ArcGISMapImageLayer(weather_server_url, this);
    map->operationalLayers()->append(mapImageLayer);

    geofence_overlay = std::shared_ptr<GraphicsOverlay>(new GraphicsOverlay(this));

    map_view->graphicsOverlays()->append(geofence_overlay.get());

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Configure mouse click signals and slots

    connect(map_view, &MapQuickView::mousePressed,
            this, &AvlMapDisplay::mouse_pressed);
    connect(map_view, &MapQuickView::mousePressedAndHeld,
            this, &AvlMapDisplay::mouse_pressed_and_held);
    connect(map_view, &MapQuickView::mouseMoved,
            this, &AvlMapDisplay::mouse_moved);
    connect(map_view, &MapQuickView::mouseReleased,
            this, &AvlMapDisplay::mouse_released);
    connect(map_view, &MapQuickView::identifyGraphicsOverlaysCompleted,
            this, &AvlMapDisplay::identify_graphics_overlays_completed);

}

//------------------------------------------------------------------------------
// Name:        toggle_weather_visibility
// Description: Toggles the visibility of the map's weather layers
//------------------------------------------------------------------------------
void AvlMapDisplay::toggle_weather_visibility()
{
    if (map->operationalLayers()->size() >= 1)
    {
        bool layer_visible = map->operationalLayers()->at(0)->isVisible();
        map->operationalLayers()->at(0)->setVisible(!layer_visible);
    }
}

//------------------------------------------------------------------------------
// Name:        toggle_enc_visibility
// Description: Toggles the visibility of the map's ENC layers
//------------------------------------------------------------------------------
void AvlMapDisplay::toggle_enc_visibility()
{
    if (map->operationalLayers()->size() >= 2)
    {
        bool layer_visible = map->operationalLayers()->at(1)->isVisible();
        map->operationalLayers()->at(1)->setVisible(!layer_visible);
    }
}

//------------------------------------------------------------------------------
// Name:        add_vehicle_graphics
// Description: Adds a vehicle's graphics overlays to the map view.
// Arguments:   - vehicle: pointer to vehicle whose graphics to add to the
//                map view
//------------------------------------------------------------------------------
void AvlMapDisplay::add_vehicle_graphics(Vehicle *vehicle)
{
    map_view->graphicsOverlays()->append(vehicle->get_mission_overlay().get());
    map_view->graphicsOverlays()->append(vehicle->get_path_overlay().get());
}

//------------------------------------------------------------------------------
// Name:        load_enc_chart
// Description: Loads an ENC from a filepath onto the map display.
// Arguments:   - filepath: filepath to the CATALOG.031 file of the ENC.
//------------------------------------------------------------------------------
void AvlMapDisplay::load_enc_chart(QVariant filepath)
{

    // Remove the first "file://" characters
    QString filepath_string = filepath.toString();
    if(filepath_string.length() > 0)
        nautical_filepaths.append(filepath_string.mid(7));

    // Create a new ENC exchange set from the list of ENC filepaths
    encExchangeSet = new EncExchangeSet(nautical_filepaths, this);

    // Set up the ENC exchange set to call the ENC loaded callback when the ENC
    // data is finished loading
    connect(encExchangeSet, &EncExchangeSet::doneLoading,
            this, &AvlMapDisplay::enc_loaded);

    // Start loading the ENC exchange set
    encExchangeSet->load();
    encExchangeSet->paths();

}

//------------------------------------------------------------------------------
// Name:        clear_enc_charts
// Description: Clears all ENCs from the map display.
//------------------------------------------------------------------------------
void AvlMapDisplay::clear_enc_charts()
{
    if (map->operationalLayers()->size() >= 2)
    {
        map->operationalLayers()->removeAt(1);
        nautical_filepaths.clear();
    }
}

//------------------------------------------------------------------------------
// Name:        set_viewpoint_to_location
// Description: Zooms the map view to the given location with the given
//              side length in meters of the area to zoom to.
// Arguments:   - lat: latitutde to move view to
//              - lon: longitude to move view to
//              - side_length: side length of zoom area in meters
//------------------------------------------------------------------------------
void AvlMapDisplay::set_viewpoint_to_location(const double lat, const double lon,
                                              const double side_length)
{

    // Calculate the side length in degrees
    double degrees = side_length / 6371000.0 * 180.0 / 3.141592;

    // Calculate the lower left and the upper right corner positions of the area
    double x_min = lon - degrees / 2.0;
    double y_min = lat - degrees / 2.0;
    double x_max = lon + degrees / 2.0;
    double y_max = lat + degrees / 2.0;

    Viewpoint zoom_viewpoint(Envelope(x_min, y_min, x_max, y_max, SpatialReference::wgs84()));
    map_view->setViewpointAnimated(zoom_viewpoint, 2.0f, AnimationCurve::EaseInOutCubic);

}

//------------------------------------------------------------------------------
// Name:        get_map_position
// Description: Gets the latitude and longitude of a mouse position in
//              degrees.
// Arguments:   - mouse_x: mouse x position
//              - mouse_y: mouse y position
// Returns:     Point containing latitude and longitude in degrees.
//------------------------------------------------------------------------------
QPointF AvlMapDisplay::get_map_position(double mouse_x, double mouse_y)
{

    // Convert the mouse location to a WGS84 point
    Point map_point = map_view->screenToLocation(mouse_x, mouse_y);
    Point wgs84_point = GeometryEngine::project(map_point, SpatialReference::wgs84());
    return QPointF(wgs84_point.x(), wgs84_point.y());

}

//------------------------------------------------------------------------------
// Name:        enc_loaded
// Description: Slot called when the ENC exchange set finishes loading.
//              Turns the ENC datasets into layers and adds them to the map
//              view.
// Arguments:   - error: load error status
//------------------------------------------------------------------------------
void AvlMapDisplay::enc_loaded(Esri::ArcGISRuntime::Error error)
{

    if (error.isEmpty())
    {

        // Get the list of datasets from the exchange set and loop though them,
        // turning them into an ENC layer and adding them to the map
        QList<EncDataset*> enc_datasets = encExchangeSet->datasets();
        for (EncDataset* enc_dataset: enc_datasets)
            map->operationalLayers()->append(new EncLayer(new EncCell(enc_dataset)));

    }
    else
    {
        qDebug() << "Failed to load ENC (" << error.message() << ")";
    }

}

//------------------------------------------------------------------------------
// Name:        mouse_pressed
// Description: Slot that is called when the mouse is pressed on the map.
// Arguments:   - center: mouse_event with mouse press information
//------------------------------------------------------------------------------
void AvlMapDisplay::mouse_pressed_and_held(QMouseEvent& mouse_event)
{

    if (mouse_event.button () == Qt::LeftButton)
    {
        qDebug()<<"The hold slot called";
        //        constexpr double tolerance = 5.0;
//        constexpr bool returnPopupsOnly = false;
//        constexpr int maximumResults = 1;
//        const double screenX = mouse_event.x();
//        const double screenY = mouse_event.y();
//        map_view->identifyGraphicsOverlay(survey_zone_point_overlay, screenX, screenY, tolerance, returnPopupsOnly, maximumResults);
        constexpr double tolerance = 5.0;
        constexpr bool returnPopupsOnly = false;
        constexpr int maximumResults = 1;

        //m_mapView->identifyGraphicsOverlay(graphicsOverlay, mouseEvent.x(), mouseEvent.y(), tolerance, returnPopupsOnly, maximumResults);
        map_view->identifyGraphicsOverlays(mouse_event.x(), mouse_event.y(), 0, false, 10);
    }

}

//------------------------------------------------------------------------------
// Name:        mouse_moved
// Description: Slot that is called when the mouse is pressed on the map.
// Arguments:   - center: mouse_event with mouse press information
//------------------------------------------------------------------------------
void AvlMapDisplay::mouse_moved(QMouseEvent& mouse_event)
{
    if (dragging)
    {


        QPointF new_location = get_map_position(mouse_event.x(), mouse_event.y());
        emit waypointMoved(dragged_vehicle_index, dragged_waypoint_index, new_location);

//        qDebug() << "dragged vehicle" << dragged_vehicle_index
//                 << "waypoint" << dragged_waypoint_index
//                 << "to" << new_lat << new_lon;

    }
}

//------------------------------------------------------------------------------
// Name:        mouse_released
// Description: Slot that is called when the mouse is pressed on the map.
// Arguments:   - center: mouse_event with mouse press information
//------------------------------------------------------------------------------
void AvlMapDisplay::mouse_released(QMouseEvent& mouse_event)
{
    Q_UNUSED(mouse_event);
    if (dragging)
    {
        dragged_graphic->setSelected(false);
        dragging = false;
    }
}

//------------------------------------------------------------------------------
// Name:        mouse_pressed
// Description: Slot that is called when the mouse is pressed on the map.
// Arguments:   - center: mouse_event with mouse press information
//------------------------------------------------------------------------------
void AvlMapDisplay::mouse_pressed(QMouseEvent& mouse_event)
{
//    map_view->identifyGraphicsOverlays(mouse_event.x(), mouse_event.y(), 0, false, 10);
}

//------------------------------------------------------------------------------
// Name:        identify_graphics_overlays_completed
// Description: Called when the identify graphics overlay function finishes.
// Arguments:   - task_id: task ID of overlay identification
//              - identify_results: result of the overlay identification
//------------------------------------------------------------------------------
void AvlMapDisplay::identify_graphics_overlays_completed(QUuid task_id,
    const QList<IdentifyGraphicsOverlayResult*> &identify_results)
{

    // Only handle the results if there are any identified overlays
    if (identify_results.size() > 0)
    {

        // Only handle the results if the identified overlay has graphics
        if (identify_results[0]->graphics().size() > 0)
        {

            // Find the index of the overlay and the graphic in their lists
            int overlay_index = map_view->graphicsOverlays()->indexOf(identify_results[0]->graphicsOverlay());
            int graphic_index = map_view->graphicsOverlays()->at(overlay_index)->graphics()->indexOf(identify_results[0]->graphics().at(0));

            // Overlays are organized as <mission><vehicle><mission><vehicle>.
            // Therefore if the overlay index is even, the overlay is a mission overlay
            if (overlay_index % 2 == 0)
            {

                // Get the vehicle index from the overlay index. Since each vehicle
                // has two overlays and the mission overlay is the first one for
                // each vehicle, the vehicle index is calculated as follows
                dragged_vehicle_index = (overlay_index + 1) / 2;

                // Graphics in a mission overlay are arranged as
                //    <wp><wp><path><wp><path><wp><path>....
                // Note that there is no path between the first two. Therefore,
                // the graphic is a waypoint if it is an odd index or is zero
                if (graphic_index == 0 || graphic_index % 2 != 0)
                {

                    // Get the waypoint index from the graphic index following
                    // logic similar to the vehicle index. This also handles
                    // the 0 index case since 1 / 2 = 0 for integers
                    dragged_waypoint_index = (graphic_index + 1) / 2;
                    qDebug() << "selected vehicle" << dragged_vehicle_index
                             << "waypoint" << dragged_waypoint_index;

                    // Select the graphic to draw a selection indicator around it
                    dragged_graphic = identify_results[0]->graphics().at(0);
                    dragged_graphic->setSelected(true);
                    qDebug() << dragged_graphic->property("test");
                    qDebug() << dragged_graphic;
                    dragging = true;

                }

            }

        }

    }

}

//--------------------------------------------------------------------------
// Name:        draw_geofence_graphics
// Description: draws geofence graphics on the map
// Arguments:   - geofence_points: lat, lon of geofence points
//--------------------------------------------------------------------------
void AvlMapDisplay::draw_geofence_graphics(QVector<QPointF> geofence_points)
{

    geofence_overlay->graphics()->clear();
    for(auto point:geofence_points)
    {
        std::shared_ptr<Graphic> geofence_point_graphic = get_geofence_graphic(point.y(), point.x());
        geofence_overlay->graphics()->append(geofence_point_graphic.get());
    }

    std::shared_ptr<Graphic> geofence_outline_graphic = get_geofence_outline_graphic(geofence_points);
    geofence_overlay->graphics()->append(geofence_outline_graphic.get());

}

//--------------------------------------------------------------------------
// Name:        launch_log_plotter
// Description: launches the log file plotting in matlab on different process
// Arguments:   - matlab_path: location of the matlab installation executable
//              - log_path: location of the script avl_log_plotter.m in the
//                 directory
//--------------------------------------------------------------------------
void AvlMapDisplay::launch_log_plotter(QString matlab_path, QString log_path)
{
    // Remove file:// from the start
    matlab_path = matlab_path.mid(7);
    log_path = log_path.mid(7);
    QString log_script = "run('"+ log_path + "');";
    QStringList params;
    params << "-nosplash" <<"-nodesktop" <<"-r" << log_script;
    QString plot_command = matlab_path + " -nosplash -nodesktop -r \"run('"
                           + log_path + "');\"";
    QProcess::startDetached(matlab_path, params);
}

//--------------------------------------------------------------------------
// Name:        load_offline_maps
// Description: loads the offline maps on the map.
// Arguments:   - paths: paths of the mmpk files to be loaded
//--------------------------------------------------------------------------
void AvlMapDisplay::load_offline_maps(QString paths)
{
     // instatiate a mobile map package
     const QString offline_map_path = paths.mid(7);

     MobileMapPackage* m_mobileMapPackage = new MobileMapPackage(offline_map_path, this);

     // wait for the mobile map package to load
     connect(m_mobileMapPackage, &MobileMapPackage::doneLoading, this, [m_mobileMapPackage, this](Error error)
     {
       if (!error.isEmpty())
       {
         qDebug() << QString("Package load error: %1 %2").arg(error.message(), error.additionalMessage());
         return;
       }

       if (!&m_mobileMapPackage || !map_view || m_mobileMapPackage->maps().isEmpty())
       {
            return;
       }

       // The package contains a list of maps that could be shown in the UI for selection.
       // For simplicity, obtain the first map in the list of maps.
       // set the map on the map view to display
       map_view->setMap(m_mobileMapPackage->maps().at(0));


     });

     // ---------------------EXP--------------- //

//     MobileMapPackage *m_mobileMapPackage2 = new MobileMapPackage("/home/cooper/ClaytorLake_1.mmpk", this);
//       connect(m_mobileMapPackage2, &MobileMapPackage::doneLoading, this, [m_mobileMapPackage2, this](Error error)
//       {
//           if (!error.isEmpty())
//           {
//             qDebug() << QString("Package load error: %1 %2").arg(error.message(), error.additionalMessage());
//             return;
//           }

//           if (!&m_mobileMapPackage2 || !map_view || m_mobileMapPackage2->maps().isEmpty())
//           {
//                return;
//           }
//           auto* map2 = m_mobileMapPackage2->maps().at(0);
//           QList<Layer*> layersFromMap2;
//           for (auto* layerFromMap2 : *(map2->operationalLayers()))
//           {
//             layersFromMap2.append(layerFromMap2);
//             layerFromMap2->setParent(this); // re-parent the layers
//           }

//           // delete the map
//           delete map2;

//           // add all the layers to the mapView's map
//           auto* operationalLayers = map_view->map()->operationalLayers();
//           for (auto* layer : layersFromMap2)
//             operationalLayers->append(layer);
//          });
     // ---------------------EXP--------------- //

     m_mobileMapPackage->load();

}

//--------------------------------------------------------------------------
// Name:        refresh_map
// Description: refreshs the base map to try to load again
//--------------------------------------------------------------------------
void AvlMapDisplay::refresh_map(QString map_name)
{
    if(map_name == "OpenStreetMap")
    {
        Basemap* basemap = Basemap::openStreetMap(this);
        map = new Map(basemap, this);
        map_view->setMap(map);
    }
    else if (map_name == "TerrainMap")
    {
        Basemap* basemap = Basemap::terrainWithLabels(this);
        map = new Map(basemap, this);
        map_view->setMap(map);
    }
    else if (map_name == "OceanMap")
    {
        Basemap* basemap = Basemap::oceans(this);
        map = new Map(basemap, this);
        map_view->setMap(map);
    }
    else if (map_name == "ImageryMap")
    {
        Basemap* basemap = Basemap::imageryWithLabels(this);
        map = new Map(basemap, this);
        map_view->setMap(map);
    }

}


//--------------------------------------------------------------------------
// Name:        load_offline_maps_tpk
// Description: loads the offline maps on the map.
// Arguments:   - paths: paths of the tpk files to be loaded
//              (You can create tpk files at:
//                   https://nixta.github.io/tpk-creator/# )
//--------------------------------------------------------------------------
void AvlMapDisplay::load_offline_maps_tpk(QUrl paths)
{
    // create a new tiled layer
    ArcGISTiledLayer* tiledLayer = new ArcGISTiledLayer(paths, this);
    Basemap* basemap = new Basemap(tiledLayer, this);
    // create a new map instance
    Map* m_map = new Map(basemap, this);
    // set map on the map view
    map_view->setMap(m_map);
}


