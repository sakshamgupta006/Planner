//==============================================================================
// Autonomous Vehicle Library
//
// Description: Mission waypoints list QML file.
//==============================================================================

// Qt imports
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.13
import QtQuick.Layouts 1.13

//==============================================================================
//                                QML DEFINITION
//==============================================================================

ItemDelegate
{

    implicitWidth: 175
    implicitHeight: 50

    ComboBox
    {
        anchors.fill: parent
        editable: false
        currentIndex: 22
        model: ["Emergency Stop",
                             "Enable Helm Mode",
                             "Disable Helm Mode",
                             "Start Mission",
                             "Advance Mission",
                             "Stop Mission",
                             "Clear Mission",
                             "Read Mission",
                             "Enable Magnetometer Stream",
                             "Disable Magnetometer Stream",
                             "Zero Pressure Sensor",
                             "Reset Safety Node",
                             "Start LBL Pings",
                             "Start OWTT Pings",
                             "Stop Acoustic Pings",
                             "Set Geofence",
                             "Enable Strobe",
                             "Disable Strobe",
                             "Enable Sonar",
                             "Disable Sonar",
                             "Start Sonar Recording",
                             "Stop Sonar Recording",
                             "No Command"]

        font.pointSize: 12

        onActivated: points_data_model.edit_point(row, column, currentText)

        //Component.onCompleted: points_data_model.

    } // ComboBox

} // ItemDelegate
