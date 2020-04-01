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

TableView
{

    id: table_view
    columnSpacing: 5
    rowSpacing: 5
    clip: true
    topMargin: header.height
    bottomMargin: footer.height
    flickableDirection: Flickable.VerticalFlick

    function select_row(row)
    {
        row_highlight.y = row*50 + row*(rowSpacing+6) + 0
        mission_data_model.select_task(row)
        row_highlight.visible = true
    }

    // Data model to draw data from
    model: mission_data_model

    columnWidthProvider:
        function get_column_width(column)
        {
            var num_columns = mission_data_model.columnCount();
            var total_spacing = (num_columns - 1) * columnSpacing;
            var total_column_width = table_view.width - total_spacing;
            var column_width = parseInt(Math.round(total_column_width / num_columns));
            if(column_width !== 0)
                return column_width;
            else
                return 50;
        }

    onWidthChanged:
    {
        if (mission_data_model.rowCount() > 0)
            table_view.forceLayout();
    }

    // Pane that sits above the table view acting as a header since
    // headers don't seem to be supported by TableView yet
    Pane
    {

        id: header
        padding: 0
        y: table_view.contentY
        z: 2
        width: table_view.parent.width
        height: 50

        background:
            Rectangle
            {
                anchors.fill: parent
                color: dark_theme_enabled ? "black" : "white"
            }

        RowLayout
        {

            anchors.fill: parent

            Repeater
            {
                model: [ "", "Duration", "Type", "Points",
                         "Roll", "Pitch", "Yaw",
                         "Vx", "Vy", "Vz",
                         "Depth", "Height", "RPM",
                         "Dive", "Command"]
                Rectangle
                {

                    Layout.fillWidth: true
                    height: 50
                    color: "transparent"

                    Label
                    {
                        text: modelData
                        font.bold: true
                        font.pointSize: 10
                        padding: 10
                        width: parent.width
                        height: parent.height
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                } // Rectangle

            } // Repeater

        } // RowLayout

    } // Pane

    // A rectangle that sits below the waypoint table with a button to
    // append a new waypoint
    Rectangle
    {

        id: footer
        y: table_view.contentHeight
        z: 1
        color: "transparent"
        width: table_view.parent.width
        height: 50

        Button
        {

            id: add_button
            width: 50
            height: 50+12
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            background:
                Image
                {
                    width: 50
                    height: 50
                    source: dark_theme_enabled? "qrc:///resources/add_button.png" : "qrc:///resources/add_button_black.png"
                    opacity: add_button.hovered ? 1.0 : 0.5
                }

            onPressed:
            {
//                if(vehicle_data_model.rowCount() > 0)
//                {

                    if(table_view.rows == 0)
                    {
                        // The startup error arises since there is no
                        // data model initiated as there is no vehicle
                        // instance
                        row_highlight.visible = true
                        mission_data_model.append_task();
                        select_row(0)
                    }
                    else
                    {
                        mission_data_model.append_task();
                    }
                //}
            }

        } // Button

    } // Pane

    // Rectangle drawn below a row to indicate that it is selected
    Rectangle
    {

        id: row_highlight
        y: -1 + 4
        z: 0
        width: table_view.parent.width
        height: 60
        color: "transparent"
        border.color: Material.accent
        border.width: 3
        visible: false

        Rectangle
        {
            anchors.fill: parent
            color: Material.accent
            opacity: 0.3
        } // Rectangle

    } // Rectangle (row_highlight)

    // Delegate for each table view item. Switches delegates based
    // on the table column
    delegate:

        Loader
        {
            source:
            {
                switch(column)
                {
                    case 0: return "MissionNumberDelegate.qml"
                    case 2: return "TaskInfoDelegate.qml"
                    case 3: return "TaskInfoDelegate.qml"
                    case 13: return "MissionDiveDelegate.qml"
                    case 14: return "TaskInfoDelegate.qml"
                    default: return "MissionDataDelegate.qml"
                }
            }
        } // Loader

} // TableView



