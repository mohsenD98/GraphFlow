import QtQuick
import QtQuick.Shapes
import ".."

Shape {
  id: shape
  width: 200
  height: 200

  property real c1x: 0
  property real c1y: 0
  property real c2x: 0
  property real c2y: 0
  property real arrowSize: 8
  property real endX: width - arrowSize
  property real endY: height
  property bool updating: false

  Component.onCompleted: updatePoints()
  onWidthChanged: updatePoints()
  onHeightChanged: updatePoints()

  function updatePoints() {
    c1x = Math.max(width / 8 * 7, Math.min(Math.abs(height), 100));
    c1y = height / 8;
    c2x = Math.min(width / 8, width - Math.min(Math.abs(height), 100));
    c2y = height / 8 * 7;
  }

  ShapePath {
    strokeWidth: 1
    strokeColor: updating ? Theme.selectionColor : Theme.linkColor
    fillColor: "transparent"
    capStyle: ShapePath.RoundCap

    PathCubic {
      x: endX
      y: endY
      control1X: c1x
      control1Y: c1y
      control2X: c2x
      control2Y: c2y
    }
  }

  ShapePath {
    strokeWidth: 1
    strokeColor: updating ? Theme.selectionColor : Theme.linkColor
    fillColor: updating ? Theme.selectionColor : Theme.linkColor

    PathMove {
      x: width
      y: height
    }
    PathLine {
      x: width - arrowSize
      y: height - arrowSize / 2
    }
    PathLine {
      x: width - arrowSize
      y: height + arrowSize / 2
    }
    PathLine {
      x: width
      y: height
    }
  }

  function begin(socket, pos) {
    clearSelection();
    if (socket.isInput && socket.attribute.boundInputs.length === 1) {
      const to = socket.attribute.node;
      const from = socket.attribute.boundInputs[0];
      GraphController.removeLink(from[0], from[1], to.uuid, socket.attribute.attrIndex);
      socket.attribute.boundInputs.pop();
      let tmp = socket;
      socket = nodesRepeater.getNodeById(from[0]).attRepeater.itemAt(from[1]).output;
      pos = mapToItem(socket, mapFromItem(tmp, pos));
    }
    visible = true;
    draggable.Drag.keys = [100];
    draggable.parent = socket;
    draggable.x = pos.x;
    draggable.y = pos.y;
    update();
  }

  function update() {
    updating = true;
    let socket = draggable.parent;
    if (!socket)
      return;
    let p1 = parent.mapFromItem(draggable, -draggable.x, -draggable.y + socket.height / 2);
    let p2 = parent.mapFromItem(draggable, 0, 0);
    // snap to target
    if (draggable.target) {
      p2 = parent.mapFromItem(draggable.target, 0, socket.height / 2);
    }

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
    updating = false;
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
