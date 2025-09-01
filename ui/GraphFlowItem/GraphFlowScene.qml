import QtQuick
import QtQuick.Effects
import QtQuick.Controls

Item {
  layer.enabled: true
  layer.samples: 4
  focus: true
  Keys.onPressed: function (event) {
    if (event.key === Qt.Key_Delete) {
      graphController.removeNodes(graphController.selectedNodes);
    }
  }

  Background {
    anchors.fill: parent
  }

  MouseArea {
    anchors.fill: parent

    property real minScale: 0.3
    property real maxScale: 5.0
    property real zoomStep: 0.1

    onWheel: function (event) {
      if (event.modifiers & Qt.ControlModifier) {
        event.accepted = true;
        let newScale = graph.scaleFactor;
        if (event.angleDelta.y > 0) {
          newScale += zoomStep;
        } else {
          newScale -= zoomStep;
        }
        newScale = Math.min(Math.max(newScale, minScale), maxScale);
        graph.scaleFactor = newScale;
      }
    }
  }

  Graph {
    id: graph
    width: Math.max(8000, root.width)
    height: Math.max(4000, root.height)
    focus: true
    x: 0
    y: 0

    property real scaleFactor: 1.0

    transform: Scale {
      origin.x: 0
      origin.y: 0
      xScale: graph.scaleFactor
      yScale: graph.scaleFactor
    }
  }

  function addNode(nodeData) {
    const currentCount = graph.nodeModel.rowCount();
    const x = nodeData.x !== undefined ? nodeData.x : (10 + (currentCount * 300)) % root.width;
    const y = nodeData.y !== undefined ? nodeData.y : 10;
    const id = graph.nodeModel.addNode(nodeData.name, nodeData.type, x, y, nodeData.color);
    if (nodeData.attributes) {
      for (var i = 0; i < nodeData.attributes.length; i++) {
        const attr = nodeData.attributes[i];
        graph.nodeModel.addAttribute(id, attr.name, attr.hasInput, attr.hasOutput, attr.maxNumberOfInputs);
      }
    }
  }
}
