//==============================================================================
// Autonomous Vehicle Library
//
// Description: QML definition of a list of points in a table view.
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
    flickableDirection: Flickable.HorizontalAndVerticalFlick
    //ScrollBar.vertical: ScrollBar { }

    width: 500
    implicitWidth: 500

    Connections
    {
        target: points_data_model
        onPointsChanged:
        {
            console.log("points changed")
            footer.y = (points_data_model.rowCount()) * 55
        }
    }

    // Data model to draw data from
    model: points_data_model

//    columnWidthProvider:
//        function get_column_width(column)
//        {
//            var num_columns = param_data_model.columnCount();
//            var total_spacing = (num_columns - 1) * columnSpacing;
//            var total_column_width = table_view.width - total_spacing;
//            var column_width = parseInt(Math.round(total_column_width / num_columns));
//            if(column_width !== 0)
//                return column_width;
//            else
//                return 50;
//        }
    onWidthChanged:
    {
        if (points_data_model.rowCount() > 0)
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
                model: [ "", "Latitude", "Longitude", "Yaw", "Command"]
                Rectangle
                {

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
        y: (points_data_model.rowCount()-1) * 55
        z: 1
        color: "transparent"
        width: table_view.contentWidth
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

            onPressed: mission_data_model.append_point(NaN, NaN);

        } // Button

    } // Pane

    // Delegate for each table view item. Switches delegates based
    // on the table column
    delegate:

        Loader
        {
            source:
            {
                switch(column)
                {
                    case 0: return "PointNumberDelegate.qml"
                    case 4: return "PointCommandDelegate.qml"
                    default: return "PointDataDelegate.qml"
                }
            }
        } // Loader

} // TableView



