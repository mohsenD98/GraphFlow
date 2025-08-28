import QtQuick

Item {
  id: graph
  property var nodeModel: GraphController.nodeModel
  property var linkModel: GraphController.linkModel
  property alias nodes: nodesRepeater
  property var selectedNodes: []
  property int topZ: 1

  Repeater {
    id: nodesRepeater
    model: nodeModel
    delegate: Node {
      id: node
      width: 150
      height: 30 + attributesList.count * 25
      name: model.name
      type: model.type
      nodeIndex: index
      attributesList: nodeModel.getAttributes(index)
      selected: false
      nodeHeaderColor: model.color

      onXChanged: {
        if (loaded)
          GraphController.nodeModel.setNodePosition(nodeIndex, x, y);
      }
      onYChanged: {
        if (loaded)
          GraphController.nodeModel.setNodePosition(nodeIndex, x, y);
      }

      function intersects(rect) {
        return !(rect.x > x + width || rect.y > y + height || rect.x + rect.width < x || rect.y + rect.height < y);
      }

      property bool loaded: false

      Component.onCompleted: {
        z = topZ++;
        x = model.x;
        y = model.y;
        loaded = true;
      }
    }
  }

  Repeater {
    id: linksRepeater
    model: linkModel
    delegate: Link {
      from: nodesRepeater.itemAt(model.fromNode).attRepeater.itemAt(model.fromAttr)
      to: nodesRepeater.itemAt(model.toNode).attRepeater.itemAt(model.toAttr)
    }
  }

  // -------------------- Selection & Drag --------------------
  Item {
    id: draggable
    property Socket target: null

    width: 1
    height: 1
    Drag.active: true

    Rectangle {
      visible: false
      color: 'transparent'
      border.color: 'red'
      anchors.fill: parent
      anchors.margins: -5
      radius: 5
    }
  }

  Item {
    id: dragSelectedNodes
    property bool active: false
    property point prev

    function begin() {
      active = true;
      draggable.parent = graph;
      prev = Qt.point(draggable.x, draggable.y);
    }

    function update() {
      if (!active)
        return;
      const dx = draggable.x - prev.x;
      const dy = draggable.y - prev.y;
      for (let s of selectedNodes) {
        s.x += dx;
        s.y += dy;
      }
      prev = Qt.point(draggable.x, draggable.y);
    }

    function end() {
      active = false;
    }
  }

  SelectionRect {
    id: selectionRect
    visible: false
    z: 100000
    property point start

    function begin(pos) {
      visible = true;
      start = pos;
      draggable.parent = graph;
      draggable.x = pos.x;
      draggable.y = pos.y;
      update();
    }

    function update() {
      const rect = getRect();
      x = rect.x;
      y = rect.y;
      width = rect.width;
      height = rect.height;
    }

    function end() {
      if (visible) {
        visible = false;
        draggable.parent = null;
        const rect = getRect();
        selectNodesInRect(rect);
      }
    }

    function getRect() {
      const x1 = Math.min(draggable.x, start.x);
      const x2 = Math.max(draggable.x, start.x);
      const y1 = Math.min(draggable.y, start.y);
      const y2 = Math.max(draggable.y, start.y);
      return Qt.rect(x1, y1, x2 - x1, y2 - y1);
    }
  }

  // -------------------- Drag Cable --------------------
  Cable {
    id: draggedCable
    visible: false
    z: 1000000

    function begin(socket, pos) {
      clearSelection();
      if (socket.isInput && socket.attribute.boundInputs.length == 1) {
        // unplug from output socket
        const to = socket.attribute;
        const from = to.boundInputs[0];
        removeLink(from, to);
        socket = from.output;
        pos = mapToItem(socket, mapFromItem(to.input, pos));
      }
      visible = true;
      draggable.Drag.keys = [100];
      draggable.parent = socket;
      draggable.x = pos.x;
      draggable.y = pos.y;
      update();
    }

    function update() {
      let socket = draggable.parent;
      if (!socket)
        return;
      let p1 = parent.mapFromItem(draggable, -draggable.x, -draggable.y + socket.height / 2);
      let p2 = parent.mapFromItem(draggable, 0, 0);

      // snap to target
      if (draggable.target)
        p2 = parent.mapFromItem(draggable.target, 0, socket.height / 2);

      // swap when connecting input
      if (socket.isInput) {
        let tmp = p1;
        p1 = p2;
        p2 = tmp;
      }
      p1.x += socket.width;
      x = p1.x;
      y = p1.y;
      width = p2.x - p1.x;
      height = p2.y - p1.y;
    }

    function end() {
      if (draggable.target) {
        let from = draggable.parent.attribute;
        let to = draggable.target.attribute;
        if (draggable.parent.isInput) {
          let tmp = to;
          to = from;
          from = tmp;
        }
        addLink(from, to);
      }
      draggable.parent = null;
      visible = false;
    }
  }

  // -------------------- Mouse Area --------------------
  MouseArea {
    anchors.fill: parent
    drag.target: draggable
    drag.smoothed: false
    drag.threshold: 5

    onPressed: function (mouse) {
      const node = getNodeAtPosition(Qt.point(mouseX, mouseY));
      const control = mouse.modifiers & Qt.ControlModifier;
      if (node && control)
        toggleNodeSelection(node);
      else if (node) {
        if (!node.selected)
          clearSelection();
        selectNode(node);
        dragSelectedNodes.begin();
      } else {
        if (!control)
          clearSelection();
        selectionRect.begin(Qt.point(mouseX, mouseY));
      }
    }

    onPositionChanged: {
      dragSelectedNodes.update();
      selectionRect.update();
    }

    onReleased: {
      dragSelectedNodes.end();
      selectionRect.end();
    }
  }

  // -------------------- Functions --------------------
  function selectNode(node) {
    if (!node.selected) {
      node.selected = true;
      selectedNodes.push(node);
    }
    node.z = topZ++;
  }

  function deselectNode(node) {
    if (node.selected) {
      node.selected = false;
      selectedNodes.splice(selectedNodes.indexOf(node), 1);
    }
  }

  function toggleNodeSelection(node) {
    if (node.selected)
      deselectNode(node);
    else {
      selectNode(node);
    }
  }

  function clearSelection() {
    for (let s of selectedNodes)
      s.selected = false;
    selectedNodes.length = 0;
  }

  function selectNodesInRect(rect) {
    for (var i = 0; i < nodesRepeater.count; ++i) {
      const node = nodesRepeater.itemAt(i);
      if (node.intersects(rect)) {
        selectNode(node);
      }
    }
  }

  function getNodeAtPosition(point) {
    let rect = Qt.rect(point.x, point.y, 0, 0);
    let topNode = null;
    let topZ = -1;
    for (var i = 0; i < nodesRepeater.count; ++i) {
      let node = nodesRepeater.itemAt(i);
      if (node.intersects(rect) && node.z > topZ) {
        topNode = node;
        topZ = node.z;
      }
    }
    return topNode;
  }

  function addLink(from, to) {
    const fromIndex = from.node.nodeIndex;
    const toIndex = to.node.nodeIndex;
    linkModel.addLink(fromIndex, from.attrIndex, toIndex, to.attrIndex);
  }
}
