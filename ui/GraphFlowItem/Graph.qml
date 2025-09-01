import QtQuick

Item {
  id: graph
  property var nodeModel: graphController.nodeModel
  property var linkModel: graphController.linkModel
  property alias nodes: nodesRepeater
  property int topZ: 1

  Repeater {
    id: nodesRepeater
    model: nodeModel
    delegate: Node {
      id: node
      width: 150
      name: model.name
      type: model.type
      selected: model.selected
      uuid: model.id
      nodeHeaderColor: model.color

      onXChanged: {
        if (loaded)
          graphController.nodeModel.setNodePosition(uuid, x, y);
      }
      onYChanged: {
        if (loaded)
          graphController.nodeModel.setNodePosition(uuid, x, y);
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

    function getNodeById(id) {
      for (let i = 0; i < count; i++) {
        if (itemAt(i).uuid === id) {
          return itemAt(i);
        }
      }
      return null;
    }
  }

  Repeater {
    id: linksRepeater
    model: linkModel
    delegate: Link {
      from: nodesRepeater.getNodeById(model.fromNode).attRepeater.itemAt(model.fromAttr)
      to: nodesRepeater.getNodeById(model.toNode).attRepeater.itemAt(model.toAttr)

      Component.onCompleted: {
        to.boundInputs.push([model.fromNode, model.fromAttr]);
      }

      Component.onDestruction: {
        to.boundInputs.pop();
      }
    }
  }

  Item {
    id: draggable
    property Socket target: null

    width: 1
    height: 1
    Drag.active: true
  }

  QtObject {
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
      for (let s of graphController.selectedNodes) {
        let node = nodesRepeater.getNodeById(s);
        node.x += dx;
        node.y += dy;
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
  }

  Cable {
    id: draggedCable
    visible: false
    z: 1000000
  }

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
          graphController.clearSelection();
        graphController.selectNode(node.uuid);
        dragSelectedNodes.begin();
      } else {
        if (!control)
          graphController.clearSelection();
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

    onDoubleClicked: {
      const node = getNodeAtPosition(Qt.point(mouseX, mouseY));
      if (node) {
        console.log(`double click on node = ${node}`);
      }
    }
  }

  function toggleNodeSelection(node) {
    if (node.selected)
      graphController.deselectNode(node.uuid);
    else {
      graphController.selectNode(node.uuid);
      node.z = topZ++;
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
    linkModel.addLink(from.node.uuid, from.attrIndex, to.node.uuid, to.attrIndex);
  }
}
