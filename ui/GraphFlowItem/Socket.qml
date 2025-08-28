import QtQuick

Rectangle {
  id: socket
  property Item attribute: parent
  property bool isInput: false

  color: isInput ? "#2196F3" : "#4CAF50"
  border.color: "#222"
  radius: width

  DropArea {
    id: dropArea
    anchors.fill: parent
    anchors.margins: -4
    keys: [100]
    onEntered: drag => {
      let fromSocket = drag.source.parent;
      if (isInput == fromSocket.isInput)
        return;
      if (!canLink(isInput ? fromSocket.attribute : attribute, isInput ? attribute : fromSocket.attribute))
        return;
      drag.source.target = socket;
      draggedCable.update();
    }
    onExited: {
      if (drag.source.target == socket) {
        drag.source.target = null;
        draggedCable.update();
      }
    }
  }

  MouseArea {
    id: mouseArea
    anchors.fill: parent
    drag.target: draggable
    drag.smoothed: false
    drag.threshold: 0

    onPressed: {
      draggedCable.begin(socket, Qt.point(mouseX, mouseY));
    }
    onReleased: {
      draggedCable.end();
    }
    onPositionChanged: {
      draggedCable.update();
    }
  }

  function canLink(fromAttribute, toAttribute) {
    return (fromAttribute && toAttribute && fromAttribute.output.visible && toAttribute.input.visible && toAttribute.boundInputs.length == 0 && fromAttribute.node != toAttribute.node);
  }
}
