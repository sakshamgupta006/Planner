//==============================================================================
// Autonomous Vehicle Library
//
// Description: Pane containing mission control buttons.
//==============================================================================

// Qt imports
import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

// Custom imports
import Avl 1.0
import "qrc:///qml/"

//==============================================================================
//                              QML DEFINITION
//==============================================================================

Popup
{

    anchors.centerIn: Overlay.overlay
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    margins: 0

    property var task;

    onVisibleChanged:
    {
        task = mission_data_model.get_selected_task()
    }

    Section
    {
        title: "Task Details"
        body_content:

            ColumnLayout
            {
                RowLayout
                {
                    TaskSettingsSection {}
                    TaskPointsSection {}
                }
                TaskPrimitiveSection {}
            } // ColumnLayout

    } // Section

} // Popup
