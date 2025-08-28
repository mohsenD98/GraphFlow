import QtQuick
import ".."

Canvas {
  id: gridBackground
  anchors.fill: parent
  property int gridSize: 20
  property color lineColor: Theme.bgLineColor

  onPaint: {
    var ctx = getContext("2d");
    ctx.clearRect(0, 0, width, height);
    ctx.strokeStyle = lineColor;
    ctx.lineWidth = 1;

    // Vertical lines
    for (var x = 0; x < width; x += gridSize) {
      ctx.beginPath();
      ctx.moveTo(x, 0);
      ctx.lineTo(x, height);
      ctx.stroke();
    }

    // Horizontal lines
    for (var y = 0; y < height; y += gridSize) {
      ctx.beginPath();
      ctx.moveTo(0, y);
      ctx.lineTo(width, y);
      ctx.stroke();
    }
  }

  onWidthChanged: requestPaint()
  onHeightChanged: requestPaint()
}
