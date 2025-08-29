import QtQuick
import QtQuick.Controls
import ".."

Dialog {
  id: dialog
  property int nodeIndex: -1
  property var attributes: [] // لیست attr از مدل گرفته میشه

  title: "Edit Node Attributes"

  Column {
    width: parent.width
    spacing: 8
    padding: 12

    Repeater {
      model: attributes
      delegate: Row {
        width: parent.width
        height: 40
        spacing: 6
        Text {
          text: modelData.name + ":"
          color: Theme.mainTextColor
        }
        TextField {
          text: model.value !== undefined ? model.value : ""
          onEditingFinished: {
            nodeModel.setAttributeValue(dialog.nodeIndex, modelData.name, text);
          }
        }
      }
    }
  }

  function openForNode(node) {
    nodeIndex = node.nodeIndex;
    attributes = nodeModel.getAttributes(nodeIndex);
    open();
  }
}
