import QtQuick 2.12

Rectangle {
  id: root
  property alias name: text.text
  property int nodeIndex: -1 // اندیس Node در NodeModel
  property var user
  property bool selected
  property alias attRepeater: attributesRepeater
  property string type

  property color nodeHeaderColor

  color: selected ? "#555" : "#444"
  border.color: selected ? "#FC0" : "#222"
  radius: 8

  Component.onCompleted: {
    updateHeight();
  }

  function intersects(rect) {
    return !(rect.x > x + width || rect.y > y + height || rect.x + rect.width < x || rect.y + rect.height < y);
  }

  function updateHeight() {
    root.height = 30;
    if (attributesRepeater.count > 0) {
      const last = attributesRepeater.itemAt(attributesRepeater.count - 1);
      root.height = last.y + last.height + 4;
    }
  }

  Rectangle {
    id: header
    anchors.left: parent.left
    anchors.right: parent.right
    height: 25
    radius: root.radius
    color: nodeHeaderColor
    border.color: root.border.color

    Rectangle {
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.margins: 1
      height: 5
      anchors.bottom: parent.bottom
      color: parent.color
    }
    Rectangle {
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.bottom: parent.bottom
      anchors.margins: 0
      height: 1
      color: root.border.color
    }
  }

  Text {
    id: text
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: 8
    elide: Text.ElideRight
    x: 6
    y: 4
    color: "#FFF"
    font.bold: true
    font.pointSize: 10
  }

  Repeater {
    id: attributesRepeater
    delegate: Attribute {
      x: 0
      y: 25 + index * 25
      width: parent.width
      height: 24
      text: modelData.name
      input.visible: modelData.hasInput
      output.visible: modelData.hasOutput

      property int attrIndex: index
    }
  }

  Connections {
    target: nodeModel
    function onAttributesChanged(index) {
      if (index == nodeIndex) {
        attributesRepeater.model = nodeModel.getAttributes(nodeIndex);
        updateHeight();
      }
    }
  }
}
