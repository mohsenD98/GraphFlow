#include "GraphController.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QRect>

GraphController::GraphController( QObject *parent )
  : QObject( parent )
{
  mFlowNodeModel = new FlowNodeModel( this );
  mFlowLinkModel = new FlowLinkModel( this );
}

void GraphController::runFlow()
{
  // run
}

void GraphController::removeNodes( const QList<QString> &nodeIdsToRemove )
{
  if ( nodeIdsToRemove.isEmpty() )
    return;

  for ( int i = mFlowLinkModel->rowCount() - 1; i >= 0; --i )
  {
    QString fromId = mFlowLinkModel->data( mFlowLinkModel->index( i ), FlowLinkModel::FromNodeRole ).toString();
    QString toId = mFlowLinkModel->data( mFlowLinkModel->index( i ), FlowLinkModel::ToNodeRole ).toString();

    for ( const QString &nodeId : nodeIdsToRemove )
    {
      if ( fromId == nodeId || toId == nodeId )
      {
        mFlowLinkModel->removeLink( i );
        break; // لینک حذف شد، نیازی به بررسی بقیه نیست
      }
    }
  }
  // بعد خود نودها رو حذف کنیم
  for ( const QString &nodeId : nodeIdsToRemove )
  {
    mFlowNodeModel->removeNode( nodeId );
  }
}

void GraphController::removeLink( const QString &fromNode, int fromAttr, const QString &toNode, int toAttr )
{
  for ( int i = 0; i < mFlowLinkModel->rowCount(); ++i )
  {
    QString fNode = mFlowLinkModel->data( mFlowLinkModel->index( i ), FlowLinkModel::FromNodeRole ).toString();
    int fAttr = mFlowLinkModel->data( mFlowLinkModel->index( i ), FlowLinkModel::FromAttributeRole ).toInt();
    QString tNode = mFlowLinkModel->data( mFlowLinkModel->index( i ), FlowLinkModel::ToNodeRole ).toString();
    int tAttr = mFlowLinkModel->data( mFlowLinkModel->index( i ), FlowLinkModel::ToAttributeRole ).toInt();

    if ( fNode == fromNode && fAttr == fromAttr && tNode == toNode && tAttr == toAttr )
    {
      mFlowLinkModel->removeLink( i );
      break;
    }
  }
}

void GraphController::clearGraph()
{
  mFlowNodeModel->clear();
  mFlowLinkModel->clear();
}

void GraphController::selectNode( const QString &id )
{
  if ( !mSelectedNodes.contains( id ) )
  {
    mSelectedNodes.append( id );
    mFlowNodeModel->setNodeSelected( id, true );
    emit selectedNodesChanged();
  }
}

void GraphController::deselectNode( const QString &id )
{
  if ( mSelectedNodes.removeOne( id ) )
  {
    mFlowNodeModel->setNodeSelected( id, false );
    emit selectedNodesChanged();
  }
}

void GraphController::clearSelection()
{
  for ( const QString &id : mSelectedNodes )
  {
    deselectNode( id );
  }
  mSelectedNodes.clear();
  emit selectedNodesChanged();
}


QList<QString> GraphController::selectedNodes() const { return mSelectedNodes; }

bool GraphController::saveFlow( const QString &path )
{
  QJsonObject root;

  // Nodes
  QJsonArray nodesArray;
  for ( int i = 0; i < mFlowNodeModel->rowCount(); ++i )
  {
    FlowNodeData node = mFlowNodeModel->getNode( i );

    QJsonObject n;
    n["name"] = node.name;
    n["type"] = node.type;
    n["x"] = node.x;
    n["y"] = node.y;
    n["color"] = node.color;
    n["id"] = node.id;

    QJsonArray attrs;
    for ( const auto &attr : node.attributes )
    {
      QJsonObject a;
      a["name"] = attr.name;
      a["hasInput"] = attr.hasInput;
      a["hasOutput"] = attr.hasOutput;
      attrs.append( a );
    }
    n["attributes"] = attrs;

    nodesArray.append( n );
  }
  root["nodes"] = nodesArray;

  // Links
  QJsonArray linksArray;
  for ( int i = 0; i < mFlowLinkModel->rowCount(); ++i )
  {
    QModelIndex idx = mFlowLinkModel->index( i, 0 );
    QJsonObject l;
    l["fromNode"] = mFlowLinkModel->data( idx, FlowLinkModel::FromNodeRole ).toString();
    l["fromAttr"] = mFlowLinkModel->data( idx, FlowLinkModel::FromAttributeRole ).toInt();
    l["toNode"] = mFlowLinkModel->data( idx, FlowLinkModel::ToNodeRole ).toString();
    l["toAttr"] = mFlowLinkModel->data( idx, FlowLinkModel::ToAttributeRole ).toInt();
    linksArray.append( l );
  }
  root["links"] = linksArray;

  QFile file( path );
  if ( !file.open( QIODevice::WriteOnly ) )
  {
    qWarning() << "Cannot write file:" << path;
    return false;
  }

  file.write( QJsonDocument( root ).toJson( QJsonDocument::Indented ) );
  return true;
}

bool GraphController::loadFlow( const QString &path )
{
  QFile file( path );
  if ( !file.open( QIODevice::ReadOnly ) )
  {
    qWarning() << "Cannot read file:" << path;
    return false;
  }

  QByteArray data = file.readAll();
  QJsonDocument doc = QJsonDocument::fromJson( data );
  if ( !doc.isObject() )
  {
    qWarning() << "Invalid JSON";
    return false;
  }

  clearGraph();

  QJsonObject root = doc.object();

  // Load nodes
  QJsonArray nodesArray = root["nodes"].toArray();
  for ( auto v : nodesArray )
  {
    QJsonObject n = v.toObject();
    QString name = n["name"].toString();
    QString color = n["color"].toString();
    QString type = n["type"].toString();
    QString id = n["id"].toString();
    int x = n["x"].toInt();
    int y = n["y"].toInt();

    mFlowNodeModel->addNode( name, type, x, y, color, id );

    QJsonArray attrs = n["attributes"].toArray();
    for ( auto aVal : attrs )
    {
      QJsonObject a = aVal.toObject();
      mFlowNodeModel->addAttribute(
        id,
        a["name"].toString(),
        a["hasInput"].toBool(),
        a["hasOutput"].toBool() );
    }
  }

  // Load links
  QJsonArray linksArray = root["links"].toArray();
  for ( auto v : linksArray )
  {
    QJsonObject l = v.toObject();
    mFlowLinkModel->addLink(
      l["fromNode"].toString(),
      l["fromAttr"].toInt(),
      l["toNode"].toString(),
      l["toAttr"].toInt() );
  }

  return true;
}
