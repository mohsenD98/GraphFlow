#include "FlowNodeModel.h"
#include "LinkModel.h"
#include "src/GraphController.h"
#include "src/NodeLibraryModel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

int main( int argc, char *argv[] )
{
  QGuiApplication app( argc, argv );
  QQuickStyle::setStyle( "Material" );

  GraphController controller;

  qmlRegisterType<FlowNodeModel>( "GraphFlow", 1, 0, "FlowNodeModel" );
  qmlRegisterType<FlowLinkModel>( "GraphFlow", 1, 0, "LinkModel" );
  qmlRegisterType<NodeLibraryModel>( "GraphFlow", 1, 0, "NodeLibraryModel" );
  qmlRegisterType<NodeLibraryProxyModel>( "GraphFlow", 1, 0, "NodeLibraryProxyModel" );

  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty( "graphController", &controller );

  const QUrl url( QStringLiteral( "qrc:/graphFlow/ui/Main.qml" ) );
  QObject::connect(
    &engine,
    &QQmlApplicationEngine::objectCreationFailed,
    &app,
    []() { QCoreApplication::exit( -1 ); },
    Qt::QueuedConnection );
  engine.load( url );

  return app.exec();
}
