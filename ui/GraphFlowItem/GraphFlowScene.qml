import QtQuick
import QtQuick.Effects
import QtQuick.Controls

Item {
  layer.enabled: true
  layer.samples: 4
  focus: true
  Keys.onPressed: function (event) {
    if (event.key === Qt.Key_Delete) {
      let nodes = [];
      for (let s in graph.selectedNodes) {
        nodes.push(graph.selectedNodes[s].nodeIndex);
      }
      GraphController.removeNodes(nodes);
    }
  }
  Background {
    anchors.fill: parent
  }

  property real scaleFactor: 1.0

  MouseArea {
    anchors.fill: parent

    property real minScale: 0.3
    property real maxScale: 5.0
    property real zoomStep: 0.1

    onWheel: function (event) {
      if (event.modifiers & Qt.ControlModifier) {
        event.accepted = true;
        let newScale = scaleFactor;
        if (event.angleDelta.y > 0)
          newScale += zoomStep;
        else
          newScale -= zoomStep;
        newScale = Math.min(Math.max(newScale, minScale), maxScale);
        scaleFactor = newScale;
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

    transform: Scale {
      origin.x: 0
      origin.y: 0
      xScale: scaleFactor
      yScale: scaleFactor
    }
  }

  // MultiEffect {
  //   id: quickMultiEffect
  //   anchors.fill: scaleFactor == 1 ? graph : undefined
  //   enabled: scaleFactor == 1 && source !== undefined
  //   source: scaleFactor == 1 ? graph : undefined
  //   shadowEnabled: scaleFactor == 1
  //   shadowOpacity: .7
  //   shadowBlur: 0.7
  //   shadowHorizontalOffset: 5
  //   shadowVerticalOffset: 2
  //   shadowColor: "black"
  //   shadowScale: 1
  // }
  Button {
    text: "Run Flow"
    onClicked: GraphController.runFlow()
  }

  function addNode(nodeData) {
    const currentCount = graph.nodeModel.rowCount();
    let x = nodeData.x !== undefined ? nodeData.x : (10 + (currentCount * 300)) % root.width;
    let y = nodeData.y !== undefined ? nodeData.y : 10;
    graph.nodeModel.addNode(nodeData.name, nodeData.type, x, y, nodeData.color);
    if (nodeData.attributes) {
      for (var i = 0; i < nodeData.attributes.length; i++) {
        const attr = nodeData.attributes[i];
        graph.nodeModel.addAttribute(currentCount, attr.name, attr.hasInput, attr.hasOutput);
      }
    }
  }
}
