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

    title: "Parameters"

    body_content:
        ParameterTable
        {
//           implicitHeight: 50
//           implicitWidth: 50
//           width: 50
//           height: 50
        }

    footer_padding: 10
    footer_color: dark_theme_enabled ? "black" : "white"
    footer_content:
        ParameterControlsPane
        {

        }

} // Section
