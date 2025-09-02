import QtQuick
import ".."

Item {
  property Item node: parent.parent
  property alias text: text.text
  property alias input: inputSocket
  property alias output: outputSocket
  property var boundInputs: []
  property int socketSize: 12
  property int maxNumberOfInputs: 1

  Socket {
    id: inputSocket
    isInput: true
    width: socketSize
    height: socketSize
    anchors.verticalCenter: parent.verticalCenter
    anchors.left: parent.left
    anchors.leftMargin: -width / 2
    z: 1
  }

  Socket {
    id: outputSocket
    isInput: false
    width: socketSize
    height: socketSize
    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    anchors.rightMargin: -width / 2
    z: 1
  }

  Text {
    id: text
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.verticalCenter: inputSocket.verticalCenter
    anchors.margins: 10
    horizontalAlignment: inputSocket.visible ? (outputSocket.visible ? Text.AlignHCenter : Text.AlignLeft) : Text.AlignRight
    elide: Text.ElideRight
    color: Theme.mainTextColor
    font.pointSize: 10
  }
}
