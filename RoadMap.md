# GraphFlow Project TODO List

## Core Features

### 1. Import/Export Project in Multiple Formats
- [ ] Research common formats for project data (JSON, XML, YAML, etc.)
- [ ] Implement export functionality for current project to JSON
- [ ] Implement import functionality from JSON
- [ ] Add support for XML export/import
- [ ] Add support for YAML export/import
- [ ] Create UI buttons for import/export actions
- [ ] Show proper error/warning when loading invalid files

### 2. Project Management System (Multiple Projects)
- [ ] Design Project data model (name, description, nodes, settings, etc.)
- [ ] Implement project list view in the UI
- [ ] Add ability to create new projects
- [ ] Add ability to delete projects
- [ ] Add ability to rename projects
- [ ] Implement switching between projects
- [ ] Allow saving/loading projects from disk
- [ ] Implement autosave for project changes

### 3. Node Execution Manager Class
- [ ] Design the node execution manager class (responsible for executing nodes in order, considering dependencies)
- [ ] Implement basic execution flow for nodes
- [ ] Handle node execution errors and propagate them
- [ ] Add support for parallel execution if needed
- [ ] Add unit tests for node execution manager

### 4. Visualize Node Output
- [ ] Define standard output formats for nodes (text, table, chart, image, etc.)
- [ ] Implement visualization components for each output type
- [ ] Integrate output visualization into node UI card/panel
- [ ] Add an option to toggle output visibility
- [ ] Display error/warning if output is not available

## Additional Suggestions

### 5. Project Settings Panel
- [ ] Design a settings dialog for each project (name, description, output format, etc.)
- [ ] Implement settings panel UI
- [ ] Allow editing and saving project settings

### 6. Undo/Redo System
- [ ] Research best practice for undo/redo in graph editors
- [ ] Implement operation history tracking
- [ ] Add undo/redo buttons to UI
- [ ] Test for edge cases and performance

### 7. Node Plugin/Extension System
- [ ] Design a plugin architecture for user-defined/custom nodes
- [ ] Implement basic plugin loading (from file/folder)
- [ ] Document plugin API for developers
- [ ] Add plugin management UI (list, enable/disable)

### 8. Node Search and Filter
- [ ] Implement search bar for nodes (by name/type)
- [ ] Add filtering options (tags, category, etc.)
- [ ] Integrate results with node palette

### 9. Error and Warning Notifications
- [ ] Add notification system for errors and warnings (input/output issues, execution errors)
- [ ] Display notifications in UI with details

### 10. Drag & Drop Node Editing
- [ ] Implement drag & drop for adding nodes to graph
- [ ] Enable moving and connecting nodes via drag & drop
- [ ] Support keyboard navigation for accessibility

### 11. Output Visualization Themes
- [ ] Design multiple visualization themes (light/dark, chart styles)
- [ ] Implement theme switching in UI
- [ ] Allow user preference saving for themes

### 12. In-App Help / Documentation
- [ ] Create help section in app with guides and examples
- [ ] Link to online documentation and FAQ
- [ ] Add tooltips for important UI elements

## Documentation & Testing

### 13. User Documentation
- [ ] Write user guide for basic usage (project creation, node editing, import/export)
- [ ] Document advanced features (plugins, settings, visualization)
- [ ] Keep documentation up to date

### 14. Developer Documentation
- [ ] Document codebase structure and main components
- [ ] Write contributing guidelines
- [ ] Add examples for custom node/plugin development

### 15. Automated Testing
- [ ] Set up basic unit tests for core modules
- [ ] Add tests for UI functionality
- [ ] Ensure import/export features are covered by tests
