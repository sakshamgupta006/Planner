//==============================================================================
// Autonomous Vehicle Library
//
// Description: A class representing a mission on a vehicle. Contains a vector
//              of tasks and provides functions to edit it. Also provides
//              functions to get the graphics required to visualize the mission
//              for display on an ArcGIS map.
//==============================================================================
#include "param.h"

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

//------------------------------------------------------------------------------
// Name:        Params constructor
// Description: Default constructor.
//------------------------------------------------------------------------------
Params::Params(QObject* parent) : QObject(parent)
{

}

//------------------------------------------------------------------------------
// Name:        Params destructor
// Description: Default destructor.
//------------------------------------------------------------------------------
Params::~Params()
{

}

//--------------------------------------------------------------------------
// Name:        size
// Description: Gets the number of parameters in the parameter list.
// Returns:     Number of tasks in the mission.
//--------------------------------------------------------------------------
int Params::size()
{
    return param_list.size();
}


//--------------------------------------------------------------------------
// Name:        append
// Description: Appends a parameter to the parameter list.
//--------------------------------------------------------------------------
void Params::append(std::string name, std::string type, QVariant value)
{
    param_list.push_back({name, type, value});
//    Task* new_task = new Task(this);
//    task_list.append(new_task);
//    connect(new_task, SIGNAL(taskChanged()), this, SLOT(task_changed()));
//    emit missionChanged();


}

//--------------------------------------------------------------------------
// Name:        clear
// Description: Removes all parameters from the parameter list.
//--------------------------------------------------------------------------
void Params::clear()
{
    param_list.clear();
}

//--------------------------------------------------------------------------
// Name:        get_all
// Description: Gets a copy of the vector of params in the parameter list.
// Returns:     Copy of the vector of parametes in the parameter list.
//--------------------------------------------------------------------------
QVector<param> Params::get_all()
{
    return param_list;
}

//--------------------------------------------------------------------------
// Name:        get
// Description: Gets a copy of the param at the given index
// Returns:     Copy of the param at the given index
//--------------------------------------------------------------------------
param* Params::get(int index)
{
    return &param_list[index];
}

//--------------------------------------------------------------------------
// Name:        get_params
// Description: generates a vector of packets stored in the packet list
// Returns:     vector of packets in format to be returned to vehicle
//--------------------------------------------------------------------------
std::vector<avl::Packet> Params::get_params()
{
    std::vector<avl::Packet> parameter_packets;
    for(int i =0; i < param_list.size(); i++)
    {
        param* current_param = get(i);
        avl::Packet parameter_packet = to_parameter_packet(current_param);

        if(parameter_packet.has_field(PARAMETER_TYPE_DESC))
            parameter_packets.push_back(parameter_packet);
    }
    return parameter_packets;
}

//--------------------------------------------------------------------------
// Name:        to_parameter_packet
// Description: convert the parameter into a packet
// Returns:     packet formed from the given param
//--------------------------------------------------------------------------
avl::Packet Params::to_parameter_packet(param *parameter)
{
    avl::Packet parameter_packet = PARAMETER_PACKET();
    parameter_packet.add_field(PARAMETER_NAME(parameter->name));

    // IMPLEMENT IN A BETTER WAY IN FUTURE (ENUM)
    if(parameter->type == "bool")
    {
        parameter_packet.add_field(PARAMETER_TYPE("bool"));
        //bool value = static_cast<bool>(parameter->value);
        parameter_packet.add_field(PARAMETER_VALUE(parameter->value));
    }
    else if(parameter->type == "int")
    {
        parameter_packet.add_field(PARAMETER_TYPE("int"));
        //int value = static_cast<int>(parameter->value);
        parameter_packet.add_field(PARAMETER_VALUE(parameter->value));
    }
    else if(parameter->type == "double")
    {
        parameter_packet.add_field(PARAMETER_TYPE("double"));
        //double value = static_cast<double>(parameter->value);
        parameter_packet.add_field(PARAMETER_VALUE(parameter->value));
    }
    else if(parameter->type == "string")
    {
        parameter_packet.add_field(PARAMETER_TYPE("string"));
        //string value = static_cast<std::string>(parameter->value);
        parameter_packet.add_field(PARAMETER_VALUE(parameter->value));
    }

    return parameter_packet;
}
