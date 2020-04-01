//==============================================================================
// Autonomous Vehicle Library
//
// Description: An abstract table model class that provides data to a TableView
//              for displaying mission information.
//==============================================================================

#include "mission_data_model.h"

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//------------------------------------------------------------------------------
// Name:        MissionDataModel
// Description: Constructor for the Model
//------------------------------------------------------------------------------
MissionDataModel::MissionDataModel(PointsDataModel* points_data_model, QObject *parent):
    QAbstractTableModel(parent), points_data_model(points_data_model)
{

}

//------------------------------------------------------------------------------
// Name:        columnCount
// Description: Returns the number of rows in the model.
//------------------------------------------------------------------------------
int MissionDataModel::rowCount(const QModelIndex&) const
{
    // The number of rows is the number of missions in the mission
    if (mission != nullptr)
        return mission->size();
    else
        return 0;
}

//------------------------------------------------------------------------------
// Name:        columnCount
// Description: Returns the number of columns in the model.
//------------------------------------------------------------------------------
int MissionDataModel::columnCount(const QModelIndex&) const
{
    return 15;
}

//------------------------------------------------------------------------------
// Name:        data
// Description: Returns the data to be displayed at the given index.
// Arguments:   - index: index of data to return
//              - role: role of returned data
//------------------------------------------------------------------------------
QVariant MissionDataModel::data(const QModelIndex &index, int role) const
{

    // Return the correct data depending on the data role
    switch (role)
    {
        case Qt::DisplayRole:
        {

            if (mission != nullptr && mission->size() > 0)
            {
                Task* task = mission->get(index.row());
                switch (index.column())
                {
                    case 0: return index.row();
                    case 1: return task->get_duration();
                    case 2: return QString::fromStdString(TaskType::to_string(task->get_type()));
                    case 3: return task->get_points().size();
                    case 4: return task->get_roll();
                    case 5: return task->get_pitch();
                    case 6: return task->get_yaw();
                    case 7: return task->get_vx();
                    case 8: return task->get_vy();
                    case 9: return task->get_vz();
                    case 10: return task->get_depth();
                    case 11: return task->get_height();
                    case 12: return task->get_rpm();
                    case 13: return task->get_dive();
                    case 14: return QString::fromStdString(ActionType::to_string(task->get_command()));
                    case 15: return task->get_swath();
                    case 16: return task->get_angle();
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
bool MissionDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
Qt::ItemFlags MissionDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable;
}

//------------------------------------------------------------------------------
// Name:        roleNames
// Description: Returns a list of role names.
//------------------------------------------------------------------------------
QHash<int, QByteArray> MissionDataModel::roleNames() const
{
    return { {Qt::DisplayRole, "display"} };
}

//------------------------------------------------------------------------------
// Name:        redraw
// Description: Redraws the mission data model.
//------------------------------------------------------------------------------
void MissionDataModel::redraw()
{
    beginResetModel();
    endResetModel();
    points_data_model->redraw();
}

//------------------------------------------------------------------------------
// Name:        append_task
// Description: Appends a default task.
//------------------------------------------------------------------------------
void MissionDataModel::append_task()
{

    int new_row_number = rowCount();
    beginInsertRows(QModelIndex(), new_row_number, new_row_number);
    mission->append();
    endInsertRows();

}

//--------------------------------------------------------------------------
// Name:        append_path_task
// Description: Appends a path type task.
//--------------------------------------------------------------------------
void MissionDataModel::append_path_task()
{

    int new_row_number = rowCount();
    beginInsertRows(QModelIndex(), new_row_number, new_row_number);
    Task* path_task = new Task();
    path_task->set_type(TaskType::TASK_PATH);
    mission->append(path_task);
    endInsertRows();

}

//------------------------------------------------------------------------------
// Name:        select_task
// Description: Selects a task as the task for new points to be added to.
// Arguments:   - index: index of task to select
//------------------------------------------------------------------------------
void MissionDataModel::select_task(int index)
{
    selected_task = index;
    points_data_model->set_task(get_selected_task());
    connect(points_data_model, SIGNAL(pointsChanged()),
            this,              SLOT(points_changed()));
}

//------------------------------------------------------------------------------
// Name:        get_selected_task
// Description: Gets the currently selected task.
// Returns:     Index of currently selected task.
//------------------------------------------------------------------------------
Task* MissionDataModel::get_selected_task()
{
    return mission->get(selected_task);
}

//--------------------------------------------------------------------------
// Name:        get_next_selected_task
// Description: Gets a pointer to the next task to the
//              currently selected task.
// Returns:     Pointer to the next task of the currently selected task.
//--------------------------------------------------------------------------
Task* MissionDataModel::get_next_selected_task()
{
    return mission->get(selected_task + 1);
}

//------------------------------------------------------------------------------
// Name:        get_selected_task_index
// Description: Gets the index of the currently selected task.
// Returns:     Index of the currently selected task.
//------------------------------------------------------------------------------
int MissionDataModel::get_selected_task_index()
{
    return selected_task;
}

//------------------------------------------------------------------------------
// Name:        move_task_up
// Description: Moves the given task up in the table.
// Arguments:   - row: row (task number) to move up
//------------------------------------------------------------------------------
void MissionDataModel::move_task_up(const int row)
{
    if (row > 0)
    {
        beginResetModel();
        mission->move_up(row);
        endResetModel();
    }
}

//------------------------------------------------------------------------------
// Name:        move_task_down
// Description: Moves the given task down in the table.
// Arguments:   - row: row (task number) to move down
//------------------------------------------------------------------------------
void MissionDataModel::move_task_down(const int row)
{
    if (row < rowCount() - 1)
    {
        beginResetModel();
        mission->move_down(row);
        endResetModel();
    }
}

//------------------------------------------------------------------------------
// Name:        delete_task
// Description: Removes the given task from the table.
// Arguments:   - row: row (task number) to delete
//------------------------------------------------------------------------------
void MissionDataModel::delete_task(const int row)
{
    beginResetModel();
    mission->remove(row);
    endResetModel();
}

//------------------------------------------------------------------------------
// Name:        clear_mission
// Description: Clears all trajectories from the mission.
//------------------------------------------------------------------------------
void MissionDataModel::clear_mission()
{
    beginResetModel();
    mission->clear();
    endResetModel();
}

//------------------------------------------------------------------------------
// Name:        edit_mission
// Description: Edits a mission task parameter value.
// Arguments:   - row: row (task number) to edit
//              - column: column (task parameter) to edit
//              - value: new parameter value
//------------------------------------------------------------------------------
void MissionDataModel::edit_mission(const int row, const int column, const QVariant value)
{

    // Only edit the row if it exists
    if(row < rowCount())
    {

        // Get a pointer to the task to be edited
        Task* task = mission->get(row);

        switch (column)
        {
            // case 0: task number
            case 1: task->set_duration(value.toDouble()); break;
            case 2: // Generates a new path task for zone missions
                    // which will be edited as the survey zone is
                    // edited
                    if(value.toInt() == TaskType::TASK_ZONE)
                        append_path_task();
                    task->set_type(value.toInt());
                    break;

            // case 3: points
            case 4: task->set_roll(value.toDouble()); break;
            case 5: task->set_pitch(value.toDouble()); break;
            case 6: task->set_yaw(value.toDouble()); break;
            case 7: task->set_vx(value.toDouble()); break;
            case 8: task->set_vy(value.toDouble()); break;
            case 9: task->set_vz(value.toDouble()); break;
            case 10: task->set_depth(value.toDouble()); break;
            case 11: task->set_height(value.toDouble()); break;
            case 12: task->set_rpm(value.toDouble()); break;
            case 13: task->set_dive(value.toBool()); break;
            case 14: task->set_command(ActionType::from_string(value.toString())); break;
            case 15: task->set_swath(value.toDouble()); break;
            case 16: task->set_angle(value.toDouble()); break;
        }

        emit dataChanged(this->index(row, column),
                         this->index(row, column));
    }

}

//------------------------------------------------------------------------------
// Name:        reset_value
// Description: Resets a mission task parameter value to NaN.
// Arguments:   - row: row (task number) to reset
//              - column: column (task parameter) to reset
//------------------------------------------------------------------------------
void MissionDataModel::reset_value(const int row, const int column)
{

    // Only edit the row if it exists
    if(row < rowCount())
    {

        // Get a pointer to the task to be edited
        Task* task = mission->get(row);

        switch (column)
        {
            // case 0: task number
            case 1:  task->set_duration(std::nan("")); break;
            // case 2: type
            // case 3: points
            case 4:  task->set_roll(std::nan("")); break;
            case 5:  task->set_pitch(std::nan("")); break;
            case 6:  task->set_yaw(std::nan("")); break;
            case 7:  task->set_vx(std::nan("")); break;
            case 8:  task->set_vy(std::nan("")); break;
            case 9:  task->set_vz(std::nan("")); break;
            case 10:  task->set_depth(std::nan("")); break;
            case 11:  task->set_height(std::nan("")); break;
            case 12:  task->set_rpm(std::nan("")); break;
            case 13:  task->set_dive(false); break;
            //case 14: task->set_command(ActionType::ACTION_NO_ACTION); break;
            case 15:  task->set_swath(std::nan("")); break;
            case 16:  task->set_angle(std::nan("")); break;
        }

        emit dataChanged(this->index(row, column),
                         this->index(row, column));
    }

}

//------------------------------------------------------------------------------
// Name:        append_point
// Description: Appends a point to the selected task.
// Arguments:   - lat: latitude of point in degrees
//              - lon: longitude of point in degrees
//------------------------------------------------------------------------------
void MissionDataModel::append_point(double lat, double lon)
{
        points_data_model->append_point(lat, lon);
        points_data_model->redraw();
        emit dataChanged(this->index(selected_task, 3),
                         this->index(selected_task, 3));

}

//------------------------------------------------------------------------------
// Name:        points_changed
// Description: Slot that is called when the points in a task change. Emits
//              a dataChanged signal to update column indicating the number
//              of points in a task.
//------------------------------------------------------------------------------
void MissionDataModel::points_changed()
{
    emit dataChanged(this->index(selected_task, 3),
                     this->index(selected_task, 3));
}
