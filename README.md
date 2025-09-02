# GraphFlow

GraphFlow is a **node-based graph editor** built with Qt/QML and C++. It allows you to visually create, connect, and manage computational flows using a flexible, interactive UI. This is an early version providing core graph editing features, with extensibility for custom node types and logic.

## Features

- **Node-Based Graph Editing**
  - Drag-and-drop nodes from a searchable sidebar.
  - Connect nodes with cables to represent data or logic flow.
  - Select, move, and delete nodes individually or in groups.
  - Zoom and pan the graph canvas for large flows.

- **Custom Node Types**
  - Node definitions loaded from JSON (`ui/data/nodes.json`).
  - Extendable library with attributes, icons, and colors.
  - Example node types: Input, Add, Multiply, Threshold, Output.

- **Interactive UI**
  - Modern UI with smooth selection, snapping, and multi-selection rectangle.
  - Visual feedback for connections, selection, and actions.
  - Sidebar node list with live filtering/search.

- **Persistence**
  - Automatically saves graph state to `lastFlow.json`.
  - Reloads previous graph on startup.

- **Code Structure & Extensibility**
  - **C++ Models:** `FlowNodeModel`, `FlowLinkModel`, `NodeLibraryModel` for data management.
  - **GraphController:** Glue for node/link operations, selection, and serialization.
  - **QML Components:** Modular files for nodes, attributes, links, scene, and controls.
  - **Pre-commit Hooks:** Code quality enforced for C++, CMake, QML, JSON, etc.

## Quick Start

1. **Build the Project**
    - Requires Qt 6.4+ and C++17.
    - Use CMake:
      ```sh
      mkdir build && cd build
      cmake ..
      make
      ```
2. **Run the Application**
    - Launch the built binary (`GraphFlow`).
    - The main window opens with the graph editor and node sidebar.

3. **Usage**
    - **Add Nodes:** Browse or search in the left sidebar; click to add to the graph.
    - **Edit Graph:** Drag to move nodes, drag sockets to connect with cables, select multiple nodes with a rectangle, delete with `Del`.
    - **Save/Load:** Graph state is auto-saved/loaded in `lastFlow.json`.

## File Structure

```
GraphFlow/
├── src/
│   ├── FlowNodeModel.*         # Node data model
│   ├── LinkModel.*             # Link/connection model
│   ├── GraphController.*       # Main graph logic
│   ├── NodeLibraryModel.*      # Node type definitions (JSON)
│   └── main.cpp
├── ui/
│   ├── Main.qml                # Main window
│   ├── GraphFlowItem/          # Node, Link, Cable, etc. QML components
│   ├── NodeList.qml            # Sidebar node library
│   ├── Theme.qml               # Color palette and theming
│   ├── data/
│   │   ├── nodes.json          # Example node definitions
│   │   └── icons/              # SVG icons for node types
│   └── qmldir
├── CMakeLists.txt
├── .pre-commit-config.yaml     # Code style and linting setup
├── .clang-format               # C++ formatting
├── .cmake-format.yaml
├── .qmlformat.ini
└── .gitignore
```

## Extending GraphFlow

- **Add Node Types:** Edit or add to `ui/data/nodes.json` (see included examples).
- **Custom Logic:** Extend `GraphController` and node models in C++.
- **UI Customization:** Modify QML components for new visuals or interactions.

## Roadmap

- Node execution logic and data flow (currently not implemented).
- More node types and better extensibility.
- Undo/redo, import/export, and advanced editing features.
- Unit tests and documentation.

---

**GraphFlow is in early development. Contributions and feedback are welcome!**
