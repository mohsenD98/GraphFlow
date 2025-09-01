import QtQuick
import ".."

Rectangle {
  id: root
  height: content.height + header.height

  property alias name: text.text
  property string uuid: ""
  property bool selected
  property alias attRepeater: attributesRepeater
  property string type
  property color nodeHeaderColor

  color: selected ? "#555" : "#444"
  border.color: selected ? Theme.selectionColor : "#222"
  radius: 8

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

    Text {
      id: text
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.verticalCenter: parent.verticalCenter
      anchors.margins: 8
      elide: Text.ElideRight
      color: Theme.mainTextColor
      font.bold: true
      font.pointSize: 10
    }
  }

  Column {
    id: content
    width: parent.width
    anchors.top: header.bottom

    Repeater {
      id: attributesRepeater
      delegate: Attribute {
        width: parent.width
        height: 24
        text: modelData.name
        input.visible: modelData.hasInput
        output.visible: modelData.hasOutput
        maxNumberOfInputs: modelData.maxNumberOfInputs

        property int attrIndex: index
      }
    }
  }

  Connections {
    target: nodeModel
    function onAttributesChanged(id) {
      if (id === uuid) {
        attributesRepeater.model = nodeModel.getAttributes(id);
      }
    }
  }

  function intersects(rect) {
    return !(rect.x > x + width || rect.y > y + height || rect.x + rect.width < x || rect.y + rect.height < y);
  }
}
