import QtQuick
import QtQuick.Controls
import "GraphFlowItem"

ApplicationWindow {
  id: root
  width: 1500
  height: 850
  visible: true
  color: Theme.bgColor

  Component.onCompleted: {
    graphController.loadFlow("lastFlow.json");
  }

  Component.onDestruction: {
    graphController.saveFlow("lastFlow.json");
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

  Row {
    anchors.top: parent.top
    anchors.right: parent.right
    spacing: 8

    Text {
      text: "Dark theme"
      color: Theme.mainTextColor
      anchors.verticalCenter: parent.verticalCenter
    }

    Switch {
      anchors.margins: 8
      anchors.verticalCenter: parent.verticalCenter

      onCheckedChanged: {
        Theme.isDarkTheme = checked;
      }
    }
  }
}
