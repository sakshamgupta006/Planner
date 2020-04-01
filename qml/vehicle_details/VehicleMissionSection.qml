//==============================================================================
// Autonomous Vehicle Library
//
// Description: A section containing vehicle status displays.
//==============================================================================

// Qt imports
import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

// Custom QML imports
import "qrc:///qml"

//==============================================================================
//                              QML DEFINITION
//==============================================================================

Section
{

    title: "Mission"

    body_content:
        MissionTable
        {
           Layout.fillWidth: true
           Layout.fillHeight: true
        }

    footer_padding: 10
    footer_color: dark_theme_enabled ? "black" : "white"
    footer_content:
        MissionControlsPane
        {

        }

} // Section
