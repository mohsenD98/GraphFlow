import QtQuick
import QtQuick.Shapes
import ".."

Shape {
  id: shape
  width: 200
  height: 200

  property real c1x: 0
  property real c1y: 0
  property real c2x: 0
  property real c2y: 0

  property real arrowSize: 8

  // نقطه انتهای منحنی، ابتدای قاعده مثلث
  property real endX: width - arrowSize
  property real endY: height

  Component.onCompleted: updatePoints()
  onWidthChanged: updatePoints()
  onHeightChanged: updatePoints()

  function updatePoints() {
    c1x = Math.max(width / 8 * 7, Math.min(Math.abs(height), 100));
    c1y = height / 8;
    c2x = Math.min(width / 8, width - Math.min(Math.abs(height), 100));
    c2y = height / 8 * 7;
  }

  // منحنی اصلی (تا ابتدای قاعده مثلث)
  ShapePath {
    strokeWidth: 1
    strokeColor: Theme.linkColor
    fillColor: "transparent"
    capStyle: ShapePath.RoundCap

    PathCubic {
      x: endX
      y: endY
      control1X: c1x
      control1Y: c1y
      control2X: c2x
      control2Y: c2y
    }
  }

  // فلش انتهای منحنی به صورت مثلث توپر (نوک فلش در انتهای شکل)
  ShapePath {
    strokeWidth: 1
    strokeColor: Theme.linkColor
    fillColor: Theme.linkColor

    PathMove {
      x: width
      y: height
    }
    PathLine {
      x: width - arrowSize
      y: height - arrowSize / 2
    }
    PathLine {
      x: width - arrowSize
      y: height + arrowSize / 2
    }
    PathLine {
      x: width
      y: height
    }
  }
}
