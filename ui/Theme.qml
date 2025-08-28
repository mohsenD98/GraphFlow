pragma Singleton
import QtQuick

// ! Light theme needs better colors!
Item {
  property bool isDarkTheme: true
  property color bgColor: isDarkTheme ? "#252724" : "white"
  property color bgColorDark: isDarkTheme ? "#333" : "white"
  property color mainTextColor: isDarkTheme ? "white" : "black"
  property color captionTextColor: isDarkTheme ? "#efefef" : "black"
  property color bgLineColor: isDarkTheme ? "#20211f" : "black"
  property color selectionColor: "#FFC107"
  property color linkColor: isDarkTheme ? "#E0E0E0" : "black"
}
