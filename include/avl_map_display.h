//==============================================================================
// Autonomous Vehicle Library
//
// Description: QQuickComponent that displays an ArcGIS map with a satellite
//              imagery base map and ENC layers on top.
//==============================================================================

#ifndef AVL_MAP_DISPLAY_H
#define AVL_MAP_DISPLAY_H

// Base class
#include <QQuickItem>

// Vehicle class
#include "vehicle.h"

// ArcGIS Map includes
#include "Map.h"
#include "MapQuickView.h"
#include "Basemap.h"

// ArcGIS ENC includes
#include "EncCell.h"
#include "EncExchangeSet.h"
#include "EncLayer.h"

// ArcGIS image layer for displaying a weather layer
#include "ArcGISMapImageLayer.h"

// ArcGIS geometry engine
#include "GeometryEngine.h"

// To Run Matlab deattached to the current process
#include "QProcess"

// To load the map from offline directory
#include "MobileMapPackage.h"

// To laod tpk offline map files
#include "ArcGISTiledLayer.h"

using namespace Esri::ArcGISRuntime;

//==============================================================================
//                              CLASS DECLARATION
//==============================================================================

class AvlMapDisplay : public QQuickItem
{

    Q_OBJECT

signals:

    //--------------------------------------------------------------------------
    // Name:        waypointMoved
    // Description: Signal emitted when a mission waypoint is moved.
    // Arguments:   - vehicle_index: index of the vehicle being edited in the
    //                vehicle list. This is not the vehicle ID
    //              - waypoint_index: index of the waypoint being edited
    //              - new_location: new waypoint location in lat/lon in degrees
    //--------------------------------------------------------------------------
    void waypointMoved(int vehicle_index, int waypoint_index, QPointF new_location);

public:

    //--------------------------------------------------------------------------
    // Name:        DisplayMap constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    explicit AvlMapDisplay(QQuickItem* parent = nullptr);

    //--------------------------------------------------------------------------
    // Name:        DisplayMap destructor
    // Description: Default destructor.
    //--------------------------------------------------------------------------
    ~AvlMapDisplay() Q_DECL_OVERRIDE;

    //--------------------------------------------------------------------------
    // Name:        declare_qml
    // Description: Registers types with the QML engine.
    //--------------------------------------------------------------------------
    static void declare_qml();

    //--------------------------------------------------------------------------
    // Name:        componentComplete
    // Description: Called after the component is instantiated. Sets up the base
    //              map and starts loading the ENC exchange set.
    //--------------------------------------------------------------------------
    void componentComplete() Q_DECL_OVERRIDE;

    //--------------------------------------------------------------------------
    // Name:        toggle_weather_visibility
    // Description: Toggles the visibility of the map's weather layers
    //--------------------------------------------------------------------------
    Q_INVOKABLE void toggle_weather_visibility();

    //--------------------------------------------------------------------------
    // Name:        toggle_enc_visibility
    // Description: Toggles the visibility of the map's ENC layers
    //--------------------------------------------------------------------------
    Q_INVOKABLE void toggle_enc_visibility();

    //--------------------------------------------------------------------------
    // Name:        add_vehicle_graphics
    // Description: Adds a vehicle's graphics overlays to the map view.
    // Arguments:   - vehicle: pointer to vehicle whose graphics to add to the
    //                map view
    //--------------------------------------------------------------------------
    Q_INVOKABLE void add_vehicle_graphics(Vehicle* vehicle);

    //--------------------------------------------------------------------------
    // Name:        load_enc_chart
    // Description: Loads a ENC file in the map
    // Arguments:   - filepath: the path of the ENC path to be loaded
    //--------------------------------------------------------------------------
    Q_INVOKABLE void load_enc_chart(QVariant filepath);

    //--------------------------------------------------------------------------
    // Name:        clear_enc_charts
    // Description: Clear any previous ENC charts loaded on the map
    //--------------------------------------------------------------------------
    Q_INVOKABLE void clear_enc_charts();

    //--------------------------------------------------------------------------
    // Name:        set_viewpoint_to_location
    // Description: Zooms the map view to the given location with the given
    //              side length in meters of the area to zoom to.
    // Arguments:   - lat: latitutde to move view to
    //              - lon: longitude to move view to
    //              - side_length: side length of zoom area in meters
    //--------------------------------------------------------------------------
    Q_INVOKABLE void set_viewpoint_to_location(const double lat, const double lon,
                                               const double side_length);

    //--------------------------------------------------------------------------
    // Name:        get_map_position
    // Description: Gets the latitude and longitude of a mouse position in
    //              degrees.
    // Arguments:   - mouse_x: mouse x position
    //              - mouse_y: mouse y position
    // Returns:     Point containing latitude and longitude in degrees.
    //--------------------------------------------------------------------------
    Q_INVOKABLE QPointF get_map_position(double mouse_x, double mouse_y);

    //--------------------------------------------------------------------------
    // Name:        draw_geofence_graphics
    // Description: draws geofence graphics on the map
    // Arguments:   - geofence_points: lat, lon of geofence points
    //--------------------------------------------------------------------------
    Q_INVOKABLE void draw_geofence_graphics(QVector<QPointF> geofence_points);

    //--------------------------------------------------------------------------
    // Name:        launch_log_plotter
    // Description: launches the log file plotting in matlab on different process
    // Arguments:   - matlab_path: location of the matlab installation executable
    //              - log_path: location of the script avl_log_plotter.m in the
    //                 directory
    //--------------------------------------------------------------------------
    Q_INVOKABLE void launch_log_plotter(QString matlab_path, QString log_path);

    //--------------------------------------------------------------------------
    // Name:        load_offline_maps
    // Description: loads the offline maps on the map.
    // Arguments:   - paths: paths of the mmpk files to be loaded
    //--------------------------------------------------------------------------
    Q_INVOKABLE void load_offline_maps(QString paths);

    //--------------------------------------------------------------------------
    // Name:        refresh_map
    // Description: refreshs the base map to try to load again
    //--------------------------------------------------------------------------
    Q_INVOKABLE void refresh_map(QString map_name);

    //--------------------------------------------------------------------------
    // Name:        load_offline_maps_tpk
    // Description: loads the offline maps on the map.
    // Arguments:   - paths: paths of the tpk files to be loaded
    //              (You can create tpk files at:
    //                   https://nixta.github.io/tpk-creator/# )
    //--------------------------------------------------------------------------
    Q_INVOKABLE void load_offline_maps_tpk(QUrl paths);


private:

    // Map and MapView used to display the map
    Map* map = nullptr;
    MapQuickView* map_view = nullptr;

    // List of nautical chart file paths
    QStringList nautical_filepaths;

    // ENC exchange set used to load and display ENC data
    EncExchangeSet* encExchangeSet = nullptr;

    // Dragging variables
    bool dragging = false;
    int dragged_vehicle_index;
    int dragged_waypoint_index;
    Graphic* dragged_graphic;

    std::shared_ptr<GraphicsOverlay> geofence_overlay;


private slots:

    //--------------------------------------------------------------------------
    // Name:        enc_loaded
    // Description: Slot called when the ENC exchange set finishes loading.
    //              Turns the ENC datasets into layers and adds them to the map
    //              view.
    // Arguments:   - error: load error status
    //--------------------------------------------------------------------------
    void enc_loaded(Esri::ArcGISRuntime::Error error);

    //--------------------------------------------------------------------------
    // Name:        mouse_pressed
    // Description: Slot that is called when the mouse is pressed on the map.
    // Arguments:   - center: mouse_event with mouse press information
    //--------------------------------------------------------------------------
    void mouse_pressed(QMouseEvent& mouse_event);
    void mouse_pressed_and_held(QMouseEvent& mouse_event);
    void mouse_moved(QMouseEvent& mouse_event);
    void mouse_released(QMouseEvent& mouse_event);

    //--------------------------------------------------------------------------
    // Name:        identify_graphics_overlays_completed
    // Description: Called when the identify graphics overlay function finishes.
    // Arguments:   - task_id: task ID of overlay identification
    //              - identify_results: result of the overlay identification
    //--------------------------------------------------------------------------
    void identify_graphics_overlays_completed(QUuid task_id,
        const QList<IdentifyGraphicsOverlayResult*> &identify_results);

};

#endif // AVL_MAP_DISPLAY_H
