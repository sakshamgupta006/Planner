#include "param_data_model.h"


//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//------------------------------------------------------------------------------
// Name:        MissionDataModel
// Description: Constructor for the Model
//------------------------------------------------------------------------------
ParamDataModel::ParamDataModel()
{

}

//------------------------------------------------------------------------------
// Name:        columnCount
// Description: Returns the number of rows in the model.
//------------------------------------------------------------------------------
int ParamDataModel::rowCount(const QModelIndex&) const
{
    // The number of rows is the number of missions in the mission
    if (current_params != nullptr)
        return current_params->size();
    else
        return 0;
}

//------------------------------------------------------------------------------
// Name:        columnCount
// Description: Returns the number of columns in the model.
//------------------------------------------------------------------------------
int ParamDataModel::columnCount(const QModelIndex&) const
{
    return 4;
}

//------------------------------------------------------------------------------
// Name:        data
// Description: Returns the data to be displayed at the given index.
// Arguments:   - index: index of data to return
//              - role: role of returned data
//------------------------------------------------------------------------------
QVariant ParamDataModel::data(const QModelIndex &index, int role) const
{

    // Return the correct data depending on the data role
    switch (role)
    {
        case Qt::DisplayRole:
        {

            if (current_params != nullptr && current_params->size() > 0)
            {
                param* current_param = current_params->get(index.row());
                switch (index.column())
                {
                    case 0: return index.row();
                    case 1: return QString::fromStdString(current_param->name);
                    case 2: return QString::fromStdString(current_param->type);
                    case 3: return current_param->value;
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
bool ParamDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
Qt::ItemFlags ParamDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable;
}

//------------------------------------------------------------------------------
// Name:        roleNames
// Description: Returns a list of role names.
//------------------------------------------------------------------------------
QHash<int, QByteArray> ParamDataModel::roleNames() const
{
    return { {Qt::DisplayRole, "display"} };
}

//------------------------------------------------------------------------------
// Name:        redraw
// Description: Redraws the mission data model.
//------------------------------------------------------------------------------
void ParamDataModel::redraw()
{
    beginResetModel();
    endResetModel();
}

//--------------------------------------------------------------------------
// Name:        append_params
// Description: Appends a list of params task.
//--------------------------------------------------------------------------
void ParamDataModel::append_params()
{
    QVector<param> current = current_params->get_all();

//    int new_row_number = rowCount();
//    beginInsertRows(QModelIndex(), new_row_number, new_row_number);
//    current_params->append();
//    endInsertRows();

}

//------------------------------------------------------------------------------
// Name:        select_task
// Description: Selects a task as the task for new points to be added to.
// Arguments:   - index: index of task to select
//------------------------------------------------------------------------------
//void ParamDataModel::select_task(int index)
//{
//    selected_task = index;
//    points_data_model->set_task(get_selected_task());
//    connect(points_data_model, SIGNAL(pointsChanged()),
//            this,              SLOT(points_changed()));
//}

//------------------------------------------------------------------------------
// Name:        get_selected_task
// Description: Gets the currently selected task.
// Returns:     Index of currently selected task.
//------------------------------------------------------------------------------
//Task* ParamDataModel::get_selected_task()
//{
//    return mission->get(selected_task);
//}


//------------------------------------------------------------------------------
// Name:        get_selected_task_index
// Description: Gets the index of the currently selected task.
// Returns:     Index of the currently selected task.
//------------------------------------------------------------------------------
int ParamDataModel::get_selected_task_index()
{
    return selected_task;
}

//------------------------------------------------------------------------------
// Name:        move_task_up
// Description: Moves the given task up in the table.
// Arguments:   - row: row (task number) to move up
//------------------------------------------------------------------------------
//void ParamDataModel::move_task_up(const int row)
//{
//    if (row > 0)
//    {
//        beginResetModel();
//        mission->move_up(row);
//        endResetModel();
//    }
//}

//------------------------------------------------------------------------------
// Name:        move_task_down
// Description: Moves the given task down in the table.
// Arguments:   - row: row (task number) to move down
//------------------------------------------------------------------------------
//void ParamDataModel::move_task_down(const int row)
//{
//    if (row < rowCount() - 1)
//    {
//        beginResetModel();
//        mission->move_down(row);
//        endResetModel();
//    }
//}

//------------------------------------------------------------------------------
// Name:        delete_task
// Description: Removes the given task from the table.
// Arguments:   - row: row (task number) to delete
//------------------------------------------------------------------------------
//void ParamDataModel::delete_task(const int row)
//{
//    beginResetModel();
//    mission->remove(row);
//    endResetModel();
//}

//------------------------------------------------------------------------------
// Name:        clear_mission
// Description: Clears all trajectories from the mission.
//------------------------------------------------------------------------------
void ParamDataModel::clear_params()
{
    beginResetModel();
    current_params->clear();
    endResetModel();
}

//------------------------------------------------------------------------------
// Name:        edit_mission
// Description: Edits a mission task parameter value.
// Arguments:   - row: row (task number) to edit
//              - column: column (task parameter) to edit
//              - value: new parameter value
//------------------------------------------------------------------------------
void ParamDataModel::edit_param(const int row, const int column, const QVariant value)
{

    // Only edit the row if it exists
    if(row < rowCount())
    {

        // Get a pointer to the task to be edited
        param* current_param = current_params->get(row);

        switch (column)
        {
            case 3: current_param->value = value.toDouble(); break;
        }

        emit dataChanged(this->index(row, column),
                         this->index(row, column));
    }

}


QVector<param> ParamDataModel::get_all()
{
    return current_params->get_all();
}


//------------------------------------------------------------------------------
// Name:        reset_value
// Description: Resets a mission task parameter value to NaN.
// Arguments:   - row: row (task number) to reset
//              - column: column (task parameter) to reset
//------------------------------------------------------------------------------
//void ParamDataModel::reset_value(const int row, const int column)
//{

//    // Only edit the row if it exists
//    if(row < rowCount())
//    {

//        // Get a pointer to the task to be edited
//        Task* task = mission->get(row);

//        switch (column)
//        {
//            // case 0: task number
//            case 1:  task->set_duration(std::nan("")); break;
//            // case 2: type
//            // case 3: points
//            case 4:  task->set_roll(std::nan("")); break;
//            case 5:  task->set_pitch(std::nan("")); break;
//            case 6:  task->set_yaw(std::nan("")); break;
//            case 7:  task->set_vx(std::nan("")); break;
//            case 8:  task->set_vy(std::nan("")); break;
//            case 9:  task->set_vz(std::nan("")); break;
//            case 10:  task->set_depth(std::nan("")); break;
//            case 11:  task->set_height(std::nan("")); break;
//            case 12:  task->set_rpm(std::nan("")); break;
//            case 13:  task->set_dive(false); break;
//        }

//        emit dataChanged(this->index(row, column),
//                         this->index(row, column));
//    }

//}

//------------------------------------------------------------------------------
// Name:        append_point
// Description: Appends a point to the selected task.
// Arguments:   - lat: latitude of point in degrees
//              - lon: longitude of point in degrees
//------------------------------------------------------------------------------
//void ParamDataModel::append_point(double lat, double lon)
//{
//        points_data_model->append_point(lat, lon);
//        points_data_model->redraw();
//        emit dataChanged(this->index(selected_task, 3),
//                         this->index(selected_task, 3));
//}

//------------------------------------------------------------------------------
// Name:        points_changed
// Description: Slot that is called when the points in a task change. Emits
//              a dataChanged signal to update column indicating the number
//              of points in a task.
//------------------------------------------------------------------------------
//void ParamDataModel::points_changed()
//{
//    emit dataChanged(this->index(selected_task, 3),
//                     this->index(selected_task, 3));
//}
