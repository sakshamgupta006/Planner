//==============================================================================
// Autonomous Vehicle Library
//
// Description: An abstract table model class that provides data to a TableView
//              for displaying a list ov points.
//==============================================================================
#include "geofence_data_model.h"

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//------------------------------------------------------------------------------
// Name:        PointsDataModel
// Description: Constructor for the Model
//------------------------------------------------------------------------------
GeofenceDataModel::GeofenceDataModel(QObject *parent) : QAbstractTableModel(parent)
{

}

//------------------------------------------------------------------------------
// Name:        columnCount
// Description: Returns the number of rows in the model.
//------------------------------------------------------------------------------
int GeofenceDataModel::rowCount(const QModelIndex&) const
{
    // The number of rows is the number of missions in the mission
    if (current_geofence != nullptr)
        return current_geofence->size();
    else
        return 0;
}

//------------------------------------------------------------------------------
// Name:        columnCount
// Description: Returns the number of columns in the model.
//------------------------------------------------------------------------------
int GeofenceDataModel::columnCount(const QModelIndex&) const
{
    return 3;
}

//------------------------------------------------------------------------------
// Name:        data
// Description: Returns the data to be displayed at the given index.
// Arguments:   - index: index of data to return
//              - role: role of returned data
//------------------------------------------------------------------------------
QVariant GeofenceDataModel::data(const QModelIndex &index, int role) const
{
    // Return the correct data depending on the data role
    switch (role)
    {

        case Qt::DisplayRole:
        {

            if (current_geofence != nullptr && current_geofence->size() > 0)
            {
                QPointF current_geofence_point = current_geofence->get(index.row());
                switch (index.column())
                {
                    case 0: return index.row();
                    case 1: return QString::number(current_geofence_point.y(), 'f', 6);
                    case 2: return QString::number(current_geofence_point.x(), 'f', 6);
                }
            }

            break;
        }

        default:
            break;

    }

    return QVariant();
}

//------------------------------------------------------------------------------
// Name:        setData
// Description: sets the data at given index in the model
// Arguments:   - index: the index pointing to the position in the data model
//              - value: the updated value
//              - role: Edit Role
//------------------------------------------------------------------------------
bool GeofenceDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value)
    {
        emit dataChanged(this->index(index.row(), 0),
                         this->index(index.row(), columnCount() - 1));
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
// Name:        flags
// Description: sets item flags in the model
//------------------------------------------------------------------------------
Qt::ItemFlags GeofenceDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable;
}

//------------------------------------------------------------------------------
// Name:        roleNames
// Description: Returns a list of role names.
//------------------------------------------------------------------------------
QHash<int, QByteArray> GeofenceDataModel::roleNames() const
{
    return { {Qt::DisplayRole, "display"} };
}

//------------------------------------------------------------------------------
// Name:        set_task
// Description: Sets the task whose data should be displayed.
// Arguments:   - task_ptr: pointer to task whose data should be displayed
//------------------------------------------------------------------------------
void GeofenceDataModel::set_task(Task* task_ptr)
{
    task = task_ptr;
    redraw();
}

//------------------------------------------------------------------------------
// Name:        redraw
// Description: Redraws the mission data model.
//------------------------------------------------------------------------------
void GeofenceDataModel::redraw()
{
    beginResetModel();
    endResetModel();
    emit geofencepointsChanged(current_geofence->get_all());
}

//------------------------------------------------------------------------------
// Name:        move_point_up
// Description: Moves the given point up in the table.
// Arguments:   - row: row (point number) to move up
//------------------------------------------------------------------------------
void GeofenceDataModel::move_point_up(const int row)
{
    if (row > 0)
    {
        beginResetModel();
        task->move_point_up(row);
        endResetModel();
        emit pointsChanged();
    }
}

//------------------------------------------------------------------------------
// Name:        move_point_down
// Description: Moves the given point down in the table.
// Arguments:   - row: row (point number) to move down
//------------------------------------------------------------------------------
void GeofenceDataModel::move_point_down(const int row)
{
    if (row < rowCount() - 1)
    {
        beginResetModel();
        task->move_point_down(row);
        endResetModel();
        emit pointsChanged();
    }
}

//------------------------------------------------------------------------------
// Name:        delete_point
// Description: Removes the given point from the table.
// Arguments:   - row: row (point number) to delete
//------------------------------------------------------------------------------
void GeofenceDataModel::delete_point(const int row)
{
    beginResetModel();
    task->remove_point(row);
    endResetModel();
    emit pointsChanged();
}

//------------------------------------------------------------------------------
// Name:        clear_points
// Description: Clears all points from the task.
//------------------------------------------------------------------------------
void GeofenceDataModel::clear_points()
{
    beginResetModel();
    current_geofence->clear();
    endResetModel();
    emit geofencepointsChanged(current_geofence->get_all());
}

//------------------------------------------------------------------------------
// Name:        edit_point
// Description: Edits a point's value.
// Arguments:   - row: row (point number) to edit
//              - column: column (point parameter) to edit
//              - value: new value
//------------------------------------------------------------------------------
void GeofenceDataModel::edit_point(const int row, const int column, const QVariant value)
{

    // Only edit the row if it exists
    if(row < rowCount())
    {

        switch (column)
        {
            // case 0: point number
            case 1:  task->set_point_lat(row, value.toDouble()); break;
            case 2:  task->set_point_lon(row, value.toDouble()); break;
        }

        emit dataChanged(this->index(row, column),
                         this->index(row, column));
        emit pointsChanged();

    }

}

//------------------------------------------------------------------------------
// Name:        reset_value
// Description: Resets a mission task parameter value to NaN.
// Arguments:   - row: row (task number) to reset
//              - column: column (task parameter) to reset
//------------------------------------------------------------------------------
void GeofenceDataModel::reset_value(const int row, const int column)
{

    // Only edit the row if it exists
    if(row < rowCount())
    {

        switch (column)
        {
            // case 0: task number
            case 1:  task->set_point_lat(row, std::nan("")); break;
            case 2:  task->set_point_lat(row, std::nan("")); break;
        }

        emit dataChanged(this->index(row, column),
                         this->index(row, column));

        emit pointsChanged();
    }

}

//------------------------------------------------------------------------------
// Name:        append_point
// Description: Appends a point to the selected point.
// Arguments:   - lat: latitude of point in degrees
//              - lon: longitude of point in degrees
//------------------------------------------------------------------------------
void GeofenceDataModel::append_point(double lat, double lon)
{
    int new_row_number = rowCount();
    beginInsertRows(QModelIndex(), new_row_number, new_row_number);
    current_geofence->append(lon, lat);
    endInsertRows();
    emit dataChanged(this->index(rowCount(), 0),
                     this->index(rowCount(), 3));
    qDebug()<<"Emitted the signal"<<endl;
    emit geofencepointsChanged(current_geofence->get_all());
 }


//------------------------------------------------------------------------------
// Name:        get_points
// Description: Gets the vector of geofence waypoints.
// Returns:     Vector of geofence waypoints.
//------------------------------------------------------------------------------
QVector<QPointF> GeofenceDataModel::get_points()
{
    return current_geofence->get_all();
}

//--------------------------------------------------------------------------
// Name:        save_geofence
// Description: save geofence points in xml format.
// Arguments:   - filepath: path where the xml will be stored
//--------------------------------------------------------------------------
void GeofenceDataModel::save_geofence(QString filepath)
{
    using namespace pugi;
    xml_document doc;
    xml_node geofence_node = doc.append_child("geofence");
    QVector<QPointF> current_geofence_points = current_geofence->get_all();
    for(auto point : current_geofence_points)
    {
        xml_node point_node = geofence_node.append_child("point");
        point_node.append_attribute("latitude") = point.y();
        point_node.append_attribute("longitude") = point.x();
    }

    doc.save_file(filepath.toStdString().c_str());
}

//--------------------------------------------------------------------------
// Name:        load_geofence
// Description: load geofence points in from xml format and updates the map
// Arguments:   - filepath: path where the xml is stored
//--------------------------------------------------------------------------
void GeofenceDataModel::load_geofence(QString filepath)
{
    using namespace pugi;
    xml_document doc;
    xml_parse_result result = doc.load_file(filepath.toStdString().c_str());

    if(result)
    {
        current_geofence->clear();

        for(xml_node point_node : doc.child("geofence").children("point"))
        {
            append_point(point_node.attribute("latitude").as_double(),
                         point_node.attribute("longitude").as_double());
        }

    }
    else
        qDebug() << "failed to load geofence file (" << result.description() << ")";
}

