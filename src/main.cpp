//==============================================================================
// Autonomous Vehicle Library
//
// Description: AVL User Interface main file.
//==============================================================================

// Qt inclides
#include <QDir>
#include <QGuiApplication>
#include <QSettings>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

// ArcGIS Includes
#include "ArcGISRuntimeEnvironment.h"
#include "MapQuickView.h"

// AVL includes
#include "avl_map_display.h"
#include "vehicle_manager.h"
#include "vehicle_data_model.h"
#include "mission_data_model.h"
#include "points_data_model.h"
#include "vehicle_status.h"
#include "vehicle_type.h"
#include "comms_channel.h"
#include "task_type.h"
#include "task.h"
#include "geofence_data_model.h"
#include "param_data_model.h"
#include "action_type.h"

using namespace Esri::ArcGISRuntime;

//==============================================================================
//                                  DEFINES
//==============================================================================

#define STRINGIZE(x) #x
#define QUOTE(x) STRINGIZE(x)

//==============================================================================
//                                  MAIN
//==============================================================================

int main(int argc, char *argv[])
{

    // Set generic application settings
    QCoreApplication::setOrganizationName("ASCL");
    QCoreApplication::setOrganizationDomain("https://marinerobotics.centers.vt.edu/index.html");
    QCoreApplication::setApplicationName("AVL Vehicle Control");
    QSettings settings;

    // Configure the application
    QGuiApplication application(argc, argv);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQuickStyle::setStyle("Material");

    // Declare custom QML interfaces and components
    Vehicle::declare_qml();
    VehicleType::declare_qml();
    CommsChannel::declare_qml();
    AvlMapDisplay::declare_qml();
    TaskType::declare_qml();
    Task::declare_qml();
    ActionType::declare_qml();

    // Configure the QML engine
    QQmlApplicationEngine engine;
    engine.addImportPath(QDir(QCoreApplication::applicationDirPath()).filePath("qml"));
    engine.addImportPath(QUOTE(ARCGIS_RUNTIME_IMPORT_PATH));
    engine.addImportPath(QUOTE(ARCGIS_TOOLKIT_IMPORT_PATH));

    // Configure the context properties for the vehicle and waypoint data models
    VehicleDataModel* vehicle_list_model = new VehicleDataModel();
    PointsDataModel* points_data_model = new PointsDataModel();
    MissionDataModel* mission_data_model = new MissionDataModel(points_data_model);
    GeofenceDataModel* geofence_data_model = new GeofenceDataModel();
    ParamDataModel* param_data_model = new ParamDataModel();

    VehicleManager vehicle_manager(vehicle_list_model, mission_data_model,
                                   param_data_model, geofence_data_model);
    QVector<QPointF> geofence_points;

    engine.rootContext()->setContextProperty("vehicle_data_model", vehicle_list_model);
    engine.rootContext()->setContextProperty("mission_data_model", mission_data_model);
    engine.rootContext()->setContextProperty("points_data_model", points_data_model);
    engine.rootContext()->setContextProperty("vehicle_manager", &vehicle_manager);
    engine.rootContext()->setContextProperty("geofence_data_model", geofence_data_model);
    engine.rootContext()->setContextProperty("param_data_model", param_data_model);
    // Load the main QML file
    engine.load(QUrl("qrc:///qml/main.qml"));

    // Execute the application
    return application.exec();

}
