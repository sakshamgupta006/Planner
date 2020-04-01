//==============================================================================
// Autonomous Vehicle Library
//
// Description: An abstract table model class that provides data to a TableView
//              for displaying a list ov points.
//==============================================================================

#include "points_data_model.h"

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//------------------------------------------------------------------------------
// Name:        PointsDataModel
// Description: Constructor for the Model
//------------------------------------------------------------------------------
PointsDataModel::PointsDataModel(QObject *parent) : QAbstractTableModel(parent)
{

}

//------------------------------------------------------------------------------
// Name:        columnCount
// Description: Returns the number of rows in the model.
//------------------------------------------------------------------------------
int PointsDataModel::rowCount(const QModelIndex&) const
{
    // The number of rows is the number of points in the task
    if (task != nullptr)
        return task->get_points().size();
    else
        return 0;
}

//------------------------------------------------------------------------------
// Name:        columnCount
// Description: Returns the number of columns in the model.
//------------------------------------------------------------------------------
int PointsDataModel::columnCount(const QModelIndex&) const
{
    return 5;
}

//------------------------------------------------------------------------------
// Name:        data
// Description: Returns the data to be displayed at the given index.
// Arguments:   - index: index of data to return
//              - role: role of returned data
//------------------------------------------------------------------------------
QVariant PointsDataModel::data(const QModelIndex &index, int role) const
{
    // Return the correct data depending on the data role
    switch (role)
    {
        case Qt::DisplayRole:
        {

            if (task != nullptr && task->get_points().size() > 0)
            {

                // Get the list of points in the task
                QVector<std::pair<QPointF, ActionType::Value>> points = task->get_points();

                switch (index.column())
                {
                    case 0: return index.row();
                    case 1: return QString::number(points.at(index.row()).first.y(), 'f', 6);
                    case 2: return QString::number(points.at(index.row()).first.x(), 'f', 6);
                    case 3: return 0;
                    case 4: return QVariant::fromValue(QString::fromStdString(ActionType::to_string(points.at(index.row()).second)));
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
bool PointsDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
Qt::ItemFlags PointsDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable;
}

//------------------------------------------------------------------------------
// Name:        roleNames
// Description: Returns a list of role names.
//------------------------------------------------------------------------------
QHash<int, QByteArray> PointsDataModel::roleNames() const
{
    return { {Qt::DisplayRole, "display"} };
}

//------------------------------------------------------------------------------
// Name:        set_task
// Description: Sets the task whose data should be displayed.
// Arguments:   - task_ptr: pointer to task whose data should be displayed
//------------------------------------------------------------------------------
void PointsDataModel::set_task(Task* task_ptr)
{
    task = task_ptr;
    redraw();
}

//------------------------------------------------------------------------------
// Name:        redraw
// Description: Redraws the mission data model.
//------------------------------------------------------------------------------
void PointsDataModel::redraw()
{
    beginResetModel();
    endResetModel();
    emit pointsChanged();
}

//------------------------------------------------------------------------------
// Name:        move_point_up
// Description: Moves the given point up in the table.
// Arguments:   - row: row (point number) to move up
//------------------------------------------------------------------------------
void PointsDataModel::move_point_up(const int row)
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
void PointsDataModel::move_point_down(const int row)
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
void PointsDataModel::delete_point(const int row)
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
void PointsDataModel::clear_points()
{
    beginResetModel();
    task->clear_points();
    endResetModel();
    emit pointsChanged();
}

//------------------------------------------------------------------------------
// Name:        edit_point
// Description: Edits a point's value.
// Arguments:   - row: row (point number) to edit
//              - column: column (point parameter) to edit
//              - value: new value
//------------------------------------------------------------------------------
void PointsDataModel::edit_point(const int row, const int column, const QVariant value)
{

    // Only edit the row if it exists
    if(row < rowCount())
    {

        switch (column)
        {
            // case 0: point number
            case 1:  task->set_point_lat(row, value.toDouble()); break;
            case 2:  task->set_point_lon(row, value.toDouble()); break;
            case 3:  qDebug() << "point heading not yet supported"; break;
            case 4:  task->set_point_command(row, ActionType::from_string(value.toString())); break;
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
void PointsDataModel::reset_value(const int row, const int column)
{

    // Only edit the row if it exists
    if(row < rowCount())
    {

        switch (column)
        {
            // case 0: task number
            case 1:  task->set_point_lat(row, std::nan("")); break;
            case 2:  task->set_point_lat(row, std::nan("")); break;
            case 4:  task->set_point_command(row, ActionType::ACTION_NO_ACTION);
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
void PointsDataModel::append_point(double lat, double lon)
{
    if(!task)
        return;
    task->add_point(QPointF(lon, lat));
    emit dataChanged(this->index(rowCount(), 4),
                     this->index(rowCount(), 4));
    emit pointsChanged();
}


//--------------------------------------------------------------------------
// Name:        append_point_command
// Description: Appends a point to the selected point with a command
// Arguments:   - lat: latitude of point in degrees
//              - lon: longitude of point in degrees
//              - command: command to be given at the point
//--------------------------------------------------------------------------
void PointsDataModel::append_point(double lat, double lon, ActionType::Value command)
{
    if(!task)
        return;
    task->add_point(QPointF(lat, lon), command);
    emit dataChanged(this->index((rowCount()), 4),
                     this->index(rowCount(), 4));
    emit pointsChanged();
}
