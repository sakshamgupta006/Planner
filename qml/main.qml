//==============================================================================
// Autonomous Vehicle Library
//
// Description: QML Main window.
//==============================================================================

// Qt imports
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import QtQuick.Dialogs 1.3
import QtGamepad 1.0

// Custom QML imports
import "qrc:///qml/map"
import "qrc:///qml/vehicle_details"
import "qrc:///qml/vehicles_overview"

//==============================================================================
//                              QML DEFINITION
//==============================================================================

ApplicationWindow
{

    id: appWindow
    width: 1920
    height: 1080
    title: "AVL Vehicle Control"
    Material.theme: dark_theme_enabled ? Material.Dark : Material.Light
    Material.accent: Material.Orange
    visible: true

    // Variable indicating whether the dark theme is enabled
    property bool dark_theme_enabled: true
    property string matlab_path: ""

    // Action to toggle between light/dark theme
    Action
    {

        id: toggle_theme_action
        text: qsTr("Toggle the interface theme")
        shortcut: "t"
        onTriggered: dark_theme_enabled = !dark_theme_enabled

    }

    // Gamepad for manual control of vehicles
    GamepadConnection {}

    // Menu bar for menu options
    menuBar:
        MenuBar
        {

            Menu
            {

                title: qsTr("View")

                Menu
                {
                    title: "Window"
                    Action
                    {
                        text: qsTr("Vehicles Overview")
                        onTriggered:
                        {
                            vehicles_overview_window.visible = true
                            vehicles_overview_window.raise()
                        }
                    }
                    Action
                    {
                        text: qsTr("Vehicle Details")
                        onTriggered:
                        {
                           vehicle_details_window.visible = true
                           vehicle_details_window.raise()
                        }
                    }
                }

                Action
                {
                    text: qsTr("Load Nautical Chart")
                    onTriggered: enc_file_dialog.open()
                }

                Action
                {
                    text: qsTr("Clear Nautical Charts")
                    onTriggered: map_display.clear_enc_charts();
                }

                Action
                {
                    text: qsTr("Toggle Nautical Charts")
                    onTriggered: map_display.toggle_enc_visibility()
                }

                Action
                {
                    text: qsTr("Toggle Weather")
                    onTriggered: map_display.toggle_weather_visibility()
                }

                Action
                {
                    text: qsTr("Toggle UI Theme")
                    onTriggered: dark_theme_enabled = !dark_theme_enabled
                }

                Action
                {
                    text: qsTr("Clear Vehicle Paths")
                    onTriggered: vehicle_manager.clear_vehicle_paths()
                }

                Action
                {
                    text: qsTr("Open Logs")
                    onTriggered:
                    {
                        log_file_dialog.open();
                    }
                }

                Action
                {
                    text: qsTr("Load offline Maps (mmpk)")
                    onTriggered:
                    {
                        map_load_file_dialog.open()
                    }
                }

                Action
                {
                    text: qsTr("Load offline Maps (tpk)")
                    onTriggered:
                    {
                        map_load_tpk_dialog.open()
                    }
                }

                Menu
                {
                    title: "Change Basemap"
                    Action
                    {
                        text: qsTr("Open Street Map")
                        onTriggered:
                        {
                            map_display.refresh_map("OpenStreetMap")
                        }
                    }
                    Action
                    {
                        text: qsTr("Terrain Map")
                        onTriggered:
                        {
                            map_display.refresh_map("TerrainMap")
                        }
                    }

                    Action
                    {
                        text: qsTr("Ocean Map")
                        onTriggered:
                        {
                            map_display.refresh_map("OceanMap")
                        }
                    }

                    Action
                    {
                        text: qsTr("Imagery Map (default)")
                        onTriggered:
                        {
                            map_display.refresh_map("ImageryMap")
                        }
                    }
                }

            } // Menu

        } // MenuBar

    // File dialog for adding nautical charts to the overlay
    FileDialog
    {

        id: map_load_file_dialog
        title: "Please choose mmpk offline map files"
        folder: shortcuts.home
        selectExisting: true
        selectFolder: true
        onAccepted:
        {
            map_display.load_offline_maps(fileUrl)
        }
    } // FileDialog

    // File dialog for adding nautical charts to the overlay
    FileDialog
    {

        id: map_load_tpk_dialog
        title: "Please choose tpk offline map files"
        folder: shortcuts.home
        selectExisting: true
        //selectFolder: true
        onAccepted:
        {
            map_display.load_offline_maps_tpk(fileUrl)
        }
    } // FileDialog


    // File dialog for adding nautical charts to the overlay
    FileDialog
    {

        id: enc_file_dialog
        title: "Please choose a nautical chart CATALOG.031 file"
        folder: shortcuts.home
        nameFilters: [ "ENC files (*.031)"]
        selectExisting: true
        selectMultiple: false
        onAccepted: map_display.load_enc_chart(fileUrl)

    } // FileDialog


    // File dialog for adding nautical charts to the overlay
    FileDialog
    {

        id: log_file_dialog
        title: "Please choose matlab installation location"
        folder: shortcuts.home
        selectExisting: true
        selectMultiple: false
        onAccepted:
        {
            matlab_path = fileUrl;
            log_plot_file_dialog.open()
        }
    } // FileDialog

    FileDialog
    {

        id: log_plot_file_dialog
        title: "Please choose the matlab script"
        folder: shortcuts.home
        selectExisting: true
        selectMultiple: false
        onAccepted:
        {
            map_display.launch_log_plotter(matlab_path, fileUrl)
        }
    } // FileDialog

    MapDisplayPane
    {
        id: map_display
        anchors.fill: parent

        Connections
        {
            target: vehicle_manager
            onVehicleAdded:
            {
                map_display.add_vehicle_graphics(vehicle)
            }
        }

    } // MapDisplayPane

    VehiclesOverviewWindow { id: vehicles_overview_window; visible: false }
    VehicleDetailsWindow { id: vehicle_details_window }

} // ApplicationWindow
