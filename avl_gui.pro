#===============================================================================
# Autonomous Vehicle Library
#
# Description: AVL User Interface project file.
#===============================================================================

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Configure project dependencies

TEMPLATE = app

QT += core gui opengl network positioning sensors qml quick
QT += quickcontrols2
QT += serialport
QT += gamepad
QT += charts

CONFIG += c++11

TARGET = avl_gui

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Ensure the correct Qt version is installed

equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 13) {
        error("$$TARGET requires Qt 5.13.0")
    }
    equals(QT_MINOR_VERSION, 13) : lessThan(QT_PATCH_VERSION, 0) {
        error("$$TARGET requires Qt 5.13.0")
    }
}

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Include ArcGIS Runtime

ARCGIS_RUNTIME_VERSION = 100.5
include($$PWD/arcgisruntime.pri)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Set up file includes

HEADERS += \
    include/action_type.h \
    include/avl_map_display.h \
    include/comms/avl_commands.h \
    include/comms/field.h \
    include/comms/packet.h \
    include/comms_channel.h \
    include/geofence.h \
    include/graphics.h \
    include/mission.h \
    include/mission_data_model.h \
    include/points_data_model.h \
    include/pugiconfig.hpp \
    include/pugixml.hpp \
    include/task.h \
    include/task_type.h \
    include/trajectory.h \
    include/util/byte.h \
    include/util/vector.h \
    include/vehicle.h \
    include/vehicle_connection.h \
    include/vehicle_data_model.h \
    include/vehicle_manager.h \
    include/vehicle_status.h \
    include/vehicle_type.h \
    include/waypoint.h \
    include/geofence_data_model.h \
    include/param_data_model.h \
    include/param.h

SOURCES += \
    src/avl_map_display.cpp \
    src/comms/avl_commands.cpp \
    src/comms/field.cpp \
    src/comms/packet.cpp \
    src/geofence.cpp \
    src/geofence_data_model.cpp \
    src/graphics.cpp \
    src/main.cpp \
    src/mission.cpp \
    src/mission_data_model.cpp \
    src/param.cpp \
    src/param_data_model.cpp \
    src/points_data_model.cpp \
    src/pugixml.cpp \
    src/task.cpp \
    src/vehicle.cpp \
    src/vehicle_connection.cpp \
    src/vehicle_data_model.cpp \
    src/vehicle_manager.cpp \
    src/vehicle_status.cpp

RESOURCES += \
    qml/qml.qrc \
    resources/resources.qrc

INCLUDEPATH += $$PWD/include
