import QtQuick
import ".."

Item {
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
}
