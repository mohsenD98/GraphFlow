import QtQuick
import QtQuick.Controls
import "GraphFlowItem"

ApplicationWindow {
  id: root
  width: 1200
  height: 800
  visible: true
  color: Theme.bgColor

  Component.onCompleted: {
    GraphController.loadFlow("lastFlow.json");
  }

  Component.onDestruction: {
    GraphController.saveFlow("lastFlow.json");
  }

  NodeList {
    id: controlPanel
    width: 200
    height: parent.height
    color: Theme.bgColorDark

    onAddNodeById: function (node) {
      flow.addNode(node);
    }
  }

  GraphFlowScene {
    id: flow
    anchors.left: controlPanel.right
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    clip: true
  }
}
