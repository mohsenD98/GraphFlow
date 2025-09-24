import QtQuick
import ".."

Canvas {
  id: gridBackground
  anchors.fill: parent
  property int gridSize: 20
  property color lineColor: Theme.bgLineColor

  onPaint: {
    var ctx = getContext("2d");
    var dpr = Canvas.devicePixelRatio;
    gridBackground.width = width * dpr;
    gridBackground.height = height * dpr;
    ctx.scale(dpr, dpr);
    ctx.clearRect(0, 0, width, height);
    ctx.strokeStyle = lineColor;
    ctx.lineWidth = 1;
    for (var x = 0; x < width; x += gridSize) {
      ctx.beginPath();
      ctx.moveTo(x + 0.5, 0);
      ctx.lineTo(x + 0.5, height);
      ctx.stroke();
    }
    for (var y = 0; y < height; y += gridSize) {
      ctx.beginPath();
      ctx.moveTo(0, y + 0.5);
      ctx.lineTo(width, y + 0.5);
      ctx.stroke();
    }
  }

  onWidthChanged: requestPaint()
  onHeightChanged: requestPaint()

  Connections {
    target: Theme
    function onIsDarkThemeChanged() {
      requestPaint();
    }
  }
}
