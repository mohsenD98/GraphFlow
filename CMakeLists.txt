cmake_minimum_required(VERSION 3.16)

project(GraphFlow VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick QuickControls2)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(GraphFlow
    src/main.cpp
    src/FlowNodeModel.h src/FlowNodeModel.cpp
    src/LinkModel.h src/LinkModel.cpp
    src/GraphController.h src/GraphController.cpp
    src/NodeLibraryModel.h src/NodeLibraryModel.cpp
)

qt_add_qml_module(GraphFlow
    URI graphFlow
    VERSION 1.0
    QML_FILES
       ui/Main.qml
       ui/Theme.qml
       ui/GraphFlowItem/Attribute.qml
       ui/GraphFlowItem/Cable.qml
       ui/GraphFlowItem/Link.qml
       ui/GraphFlowItem/Node.qml
       ui/GraphFlowItem/Graph.qml
       ui/GraphFlowItem/SelectionRect.qml
       ui/GraphFlowItem/Socket.qml
       ui/GraphFlowItem/Background.qml
       ui/GraphFlowItem/GraphFlowScene.qml

       ui/NodeList.qml

    RESOURCES
       ui/qmldir
       ui/data/nodes.json

    RESOURCES
       ui/data/icons/browse_activity_24dp.svg
       ui/data/icons/filter_tilt_shift_24dp.svg
       ui/data/icons/add_24dp.svg
       ui/data/icons/calculate_24dp.svg
       ui/data/icons/database_24dp.svg

)

target_link_libraries(GraphFlow
    PRIVATE Qt6::Quick Qt6::QuickControls2
)
target_link_libraries(GraphFlow PRIVATE Qt6::Core)
target_link_libraries(GraphFlow PRIVATE Qt6::Core)
target_link_libraries(GraphFlow PRIVATE Qt6::Core)
