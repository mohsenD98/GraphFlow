import QtQuick
import ".."

Item {
  property point start
  property var draggable

  Rectangle {
    anchors.fill: parent
    color: Theme.selectionColor
    opacity: 0.1
  }

  Rectangle {
    anchors.fill: parent
    color: "transparent"
    border.color: Theme.selectionColor
    opacity: 0.5
  }

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

  function selectNodesInRect(rect) {
    for (var i = 0; i < nodesRepeater.count; ++i) {
      const node = nodesRepeater.itemAt(i);
      if (node.intersects(rect)) {
        graphController.selectNode(node.uuid);
        node.z = topZ++;
      }
    }
  }
}
