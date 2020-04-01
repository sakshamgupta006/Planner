//==============================================================================
// Autonomous Vehicle Library
//
// Description: QML accessible enumeration of task types.
//==============================================================================

#ifndef ACTION_TYPE_H
#define ACTION_TYPE_H

// QObject base class
#include <QObject>

// QML registering
#include <QQmlEngine>

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

class ActionType : public QObject
{

    Q_OBJECT

public:

    // Action types enumeration
    enum Value
    {
         ACTION_PING =                     0x00,
         ACTION_EMERGENCY_STOP =           0x01,
         ACTION_POWER_CYCLE =              0x02,
         ACTION_RESTART_ROS =              0x03,
         ACTION_RESET_SAFETY =             0x04,
         ACTION_SET_MODE =                 0x05,
         ACTION_ENABLE_MAG_STREAM =        0x06,
         ACTION_DISABLE_MAG_STREAM =       0x07,
         ACTION_TARE_PRESSURE =            0x08,
         ACTION_START_LBL_PINGS =          0x09,
         ACTION_START_OWTT_PINGS =         0x0A,
         ACTION_STOP_ACOUSTIC_PINGS =      0x0B,
         ACTION_ENABLE_BACK_SEAT_DRIVER =  0x0C,
         ACTION_DISABLE_BACK_SEAT_DRIVER = 0x0D,
         ACTION_SET_GEOFENCE =             0x0E,
         ACTION_ENABLE_STROBE =            0x0F,
         ACTION_DISABLE_STROBE =           0x10,
         ACTION_ENABLE_SONAR =             0x11,
         ACTION_DISABLE_SONAR =            0x12,
         ACTION_START_SONAR_RECORDING =    0x13,
         ACTION_STOP_SONAR_RECORDING =     0x14,
         ACTION_NO_ACTION =                0x15,
    };
    Q_ENUM(Value)

public:

    //--------------------------------------------------------------------------
    // Name:        TaskType constructor
    // Description: Default constructor.
    //--------------------------------------------------------------------------
    ActionType() : QObject()
    {

    }

    //--------------------------------------------------------------------------
    // Name:        declare_qml
    // Description: Registers types with the QML engine.
    //--------------------------------------------------------------------------
    static void declare_qml()
    {
        qmlRegisterType<ActionType>("Avl", 1, 0, "ActionType");
        qRegisterMetaType<ActionType::Value>("ActionType.Value");
    }

    //--------------------------------------------------------------------------
    // Name:        declare_qml
    // Description: Registers types with the QML engine.
    //--------------------------------------------------------------------------
    static std::string to_string(Value type)
    {
        switch (type)
        {
            case ACTION_PING:               return std::string("Ping");
            case ACTION_EMERGENCY_STOP:     return std::string("Emergency Stop");
            case ACTION_POWER_CYCLE:        return std::string("Power Cycle");
            case ACTION_RESTART_ROS:        return std::string("Restart ROS");
            case ACTION_RESET_SAFETY:       return std::string("Reset Safety Node");
            case ACTION_SET_MODE:           return std::string("Set Mode");
            case ACTION_ENABLE_MAG_STREAM:  return std::string("Enable Magnetometer Stream");
            case ACTION_DISABLE_MAG_STREAM: return std::string("Disable Magnetometer Stream");
            case ACTION_TARE_PRESSURE:     return std::string("Zero Pressure Sensor");
            case ACTION_START_LBL_PINGS:    return std::string("Start LBL Pings");
            case ACTION_START_OWTT_PINGS:   return std::string("Start OWTT Pings");
            case ACTION_STOP_ACOUSTIC_PINGS:return std::string("Stop Acoustic Pings");
            case ACTION_SET_GEOFENCE:       return std::string("Set Geofence");
            case ACTION_ENABLE_STROBE:      return std::string("Enable Strobe");
            case ACTION_DISABLE_STROBE:     return std::string("Disable Strobe");
            case ACTION_ENABLE_BACK_SEAT_DRIVER:  return std::string("Enable Back Seat Driver");
            case ACTION_DISABLE_BACK_SEAT_DRIVER: return std::string("Disable Back Steat Driver");
            case ACTION_ENABLE_SONAR:             return std::string("Enable Sonar");
            case ACTION_DISABLE_SONAR:            return std::string("Disable Sonar");
            case ACTION_START_SONAR_RECORDING:    return std::string("Start Sonar Recording");
            case ACTION_STOP_SONAR_RECORDING:     return std::string("Stop Sonar Recording");
            case ACTION_NO_ACTION:                return std::string("No Command");
        }
    }

    //--------------------------------------------------------------------------
    // Name:        from_string
    // Description: Registers types with the QML engine.
    //--------------------------------------------------------------------------
    static Value from_string(QString type)
    {
        if(type == "No Command")
            return ACTION_NO_ACTION;
        else if (type == "Ping")
            return ACTION_PING;
        else if (type == "Emergency Stop")
            return ACTION_EMERGENCY_STOP;
        else if (type == "Power Cycle")
            return ACTION_POWER_CYCLE;
        else if(type == "Restart ROS")
            return ACTION_RESTART_ROS;
        else if(type == "Reset Safety Node")
            return ACTION_RESET_SAFETY;
        else if(type == "Set Mode")
            return ACTION_SET_MODE;
        else if(type == "Enable Magnetometer Stream")
            return ACTION_ENABLE_MAG_STREAM;
        else if(type == "Disable Magnetometer Stream")
            return ACTION_DISABLE_MAG_STREAM;
        else if(type == "Zero Pressure Sensor")
            return ACTION_TARE_PRESSURE;
        else if(type == "Start LBL Pings")
            return ACTION_START_LBL_PINGS;
        else if(type == "Start OWTT Pings")
            return ACTION_START_OWTT_PINGS;
        else if(type == "Stop Acoustic Pings")
            return ACTION_STOP_ACOUSTIC_PINGS;
        else if(type == "Set Geofence")
            return ACTION_SET_GEOFENCE;
        else if(type == "Enable Strobe")
            return ACTION_ENABLE_STROBE;
        else if(type == "Disable Strobe")
            return ACTION_DISABLE_STROBE;
        else if(type == "Enable Back Seat Driver")
            return ACTION_ENABLE_BACK_SEAT_DRIVER;
        else if(type == "Disable Back Seat Driver")
            return ACTION_DISABLE_BACK_SEAT_DRIVER;
        else if(type == "Enable Sonar")
            return ACTION_ENABLE_SONAR;
        else if(type == "Disable Sonar")
            return ACTION_DISABLE_SONAR;
        else if(type == "Start Sonar Recording")
            return ACTION_START_SONAR_RECORDING;
        else
            return ACTION_STOP_SONAR_RECORDING;
    }

};

#endif // ACTION_TYPE_H
