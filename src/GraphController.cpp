#include "GraphController.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QRect>

GraphController::GraphController( QObject *parent )
  : QObject( parent )
{
  mFlowNodeModel = new FlowNodeModel( this );
  mLinkModel = new LinkModel( this );
}

void GraphController::removeNode( int index )
{
  if ( index < 0 || index >= mFlowNodeModel->rowCount() )
    return;

  QList<int> linksToRemove;
  for ( int i = 0; i < mLinkModel->rowCount(); ++i )
  {
    const int link = mLinkModel->data( mLinkModel->index( i ), LinkModel::FromNodeRole ).toInt();
    const int toNode = mLinkModel->data( mLinkModel->index( i ), LinkModel::ToNodeRole ).toInt();
    if ( link == index || toNode == index )
    {
      linksToRemove.append( i );
    }
  }
  std::sort( linksToRemove.begin(), linksToRemove.end(), std::greater<int>() );
  for ( int idx : linksToRemove )
    mLinkModel->removeLink( idx );

  mFlowNodeModel->removeNode( index );
}

void GraphController::removeNodes( const QList<int> &nodesToRemove )
{
  if ( nodesToRemove.isEmpty() )
    return;

  for ( int i = mLinkModel->rowCount() - 1; i >= 0; --i )
  {
    int from = mLinkModel->data( mLinkModel->index( i ), LinkModel::FromNodeRole ).toInt();
    int to = mLinkModel->data( mLinkModel->index( i ), LinkModel::ToNodeRole ).toInt();
    for ( int nodeIndex : nodesToRemove )
    {
      if ( from == nodeIndex || to == nodeIndex )
      {
        mLinkModel->removeLink( i );
        break; // لینک حذف شد، نیازی به بررسی بقیه nodesToRemove نیست
      }
    }
  }

  QList<int> sortedNodes = nodesToRemove;
  std::sort( sortedNodes.begin(), sortedNodes.end(), std::greater<int>() );
  for ( int idx : sortedNodes )
  {
    if ( idx >= 0 && idx < mFlowNodeModel->rowCount() )
    {
      mFlowNodeModel->removeNode( idx );
    }
  }
}

void GraphController::clearGraph()
{
  mFlowNodeModel->clear();
  mLinkModel->clear();
}


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
  for ( int i = 0; i < mLinkModel->rowCount(); ++i )
  {
    QModelIndex idx = mLinkModel->index( i, 0 );
    QJsonObject l;
    l["fromNode"] = mLinkModel->data( idx, LinkModel::FromNodeRole ).toInt();
    l["fromAttr"] = mLinkModel->data( idx, LinkModel::FromAttributeRole ).toInt();
    l["toNode"] = mLinkModel->data( idx, LinkModel::ToNodeRole ).toInt();
    l["toAttr"] = mLinkModel->data( idx, LinkModel::ToAttributeRole ).toInt();
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
    int type = n["type"].toInt();
    int x = n["x"].toInt();
    int y = n["y"].toInt();

    mFlowNodeModel->addNode( name, type, x, y, color );
    int nodeIndex = mFlowNodeModel->rowCount() - 1;

    QJsonArray attrs = n["attributes"].toArray();
    for ( auto aVal : attrs )
    {
      QJsonObject a = aVal.toObject();
      mFlowNodeModel->addAttribute(
        nodeIndex,
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
    mLinkModel->addLink(
      l["fromNode"].toInt(),
      l["fromAttr"].toInt(),
      l["toNode"].toInt(),
      l["toAttr"].toInt() );
  }

  return true;
}
