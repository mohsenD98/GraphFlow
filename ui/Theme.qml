pragma Singleton
import QtQuick

// ! Light theme needs better colors!
Item {
  property bool isDarkTheme: false
  property color bgColor: isDarkTheme ? "#252724" : "#f2f0f5"
  property color bgColorDark: isDarkTheme ? "#333" : "white"
  property color mainTextColor: isDarkTheme ? "white" : "black"
  property color captionTextColor: isDarkTheme ? "#efefef" : "black"
  property color bgLineColor: isDarkTheme ? "#20211f" : "#2e20211f"
  property color selectionColor: isDarkTheme ? "#FFC107" : "#DFA000"
  property color linkColor: isDarkTheme ? "#E0E0E0" : "#626668"
  property color nodeNormal: isDarkTheme ? "#444" : "#d5d5d5"
  property color nodeSelected: isDarkTheme ? "#555" : "#c0c0c0"

  onIsDarkThemeChanged: {
    Qt.styleHints.colorScheme = isDarkTheme ? Qt.Dark : Qt.Light;
  }

  Component.onCompleted: {
    Qt.styleHints.colorScheme = isDarkTheme ? Qt.Dark : Qt.Light;
  }
}
