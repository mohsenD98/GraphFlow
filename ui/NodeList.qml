import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQml.Models
import GraphFlow 1.0

Rectangle {
  signal addNodeById(var node)

  ColumnLayout {
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: 4
    spacing: 4

    TextField {
      Layout.fillWidth: true
      placeholderText: "Search in name or description"
      Layout.alignment: Qt.AlignTop
      onTextChanged: {
        nodeProxy.filterText = text;
      }
    }

    Repeater {
      model: NodeLibraryProxyModel {
        id: nodeProxy
        sourceModel: NodeLibraryModel {
          id: nodeLibraryModel
          Component.onCompleted: {
            loadFromJson(":/graphFlow/ui/data/nodes.json");
          }
        }
      }

      delegate: Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 60
        radius: 4
        color: Theme.bgColor

        Rectangle {
          id: nodeIcon
          width: 40
          height: width
          radius: 4
          anchors.verticalCenter: parent.verticalCenter
          anchors.left: parent.left
          anchors.leftMargin: 5
          color: model.color

          Image {
            source: "qrc:/graphFlow/" + icon
            anchors.centerIn: parent
          }
        }

        Text {
          id: nodeName
          text: name
          anchors.top: nodeIcon.top
          anchors.left: nodeIcon.right
          anchors.leftMargin: 8
          color: Theme.mainTextColor
          font.pixelSize: 13
        }
        Text {
          text: description
          anchors.top: nodeName.bottom
          anchors.left: nodeIcon.right
          anchors.right: parent.right
          anchors.topMargin: 4
          anchors.leftMargin: 8
          anchors.rightMargin: 8
          color: Theme.captionTextColor
          font.pixelSize: 9
          wrapMode: Text.WordWrap
        }

        MouseArea {
          anchors.fill: parent
          cursorShape: Qt.PointingHandCursor
          onClicked: {
            addNodeById(nodeLibraryModel.nodeByName(name));
          }
        }
      }
    }
  }
}
