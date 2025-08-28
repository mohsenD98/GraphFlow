#include "FlowNodeModel.h"

FlowNodeModel::FlowNodeModel( QObject *parent )
  : QAbstractListModel( parent ) {}

int FlowNodeModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  return mNodes.size();
}

QVariant FlowNodeModel::data( const QModelIndex &index, int role ) const
{
  if ( !index.isValid() || index.row() < 0 || index.row() >= mNodes.size() )
    return QVariant();

  const auto &node = mNodes.at( index.row() );

  switch ( role )
  {
    case NameRole:
      return node.name;
    case TypeRole:
      return node.type;
    case XRole:
      return node.x;
    case YRole:
      return node.y;
    case ColorRole:
      return node.color;
    default:
      return QVariant();
  }
}

QHash<int, QByteArray> FlowNodeModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[NameRole] = "name";
  roles[TypeRole] = "type";
  roles[XRole] = "x";
  roles[YRole] = "y";
  roles[ColorRole] = "color";
  return roles;
}


void FlowNodeModel::addNode( const QString &name, int type, int x, int y, const QString &color )
{
  beginInsertRows( QModelIndex(), mNodes.size(), mNodes.size() );
  FlowNodeData node;
  node.name = name;
  node.type = type;
  node.x = x;
  node.y = y;
  node.color = color;
  mNodes.append( node );
  endInsertRows();
}

void FlowNodeModel::removeNode( int index )
{
  if ( index < 0 || index >= mNodes.size() )
    return;
  beginRemoveRows( QModelIndex(), index, index );
  mNodes.removeAt( index );
  endRemoveRows();
}

void FlowNodeModel::clear()
{
  beginResetModel();
  mNodes.clear();
  endResetModel();
}

FlowNodeData FlowNodeModel::getNode( int index ) const
{
  if ( index < 0 || index >= mNodes.size() )
    return FlowNodeData();
  return mNodes[index];
}

void FlowNodeModel::setNodePosition( int index, int x, int y )
{
  if ( index < 0 || index >= mNodes.size() )
    return;

  auto &node = mNodes[index];
  if ( node.x == x && node.y == y )
    return;

  node.x = x;
  node.y = y;

  QModelIndex modelIndex = this->index( index );
  emit dataChanged( modelIndex, modelIndex, { XRole, YRole } );
}

void FlowNodeModel::addAttribute( int nodeIndex, const QString &name, bool hasInput, bool hasOutput )
{
  if ( nodeIndex < 0 || nodeIndex >= mNodes.size() )
    return;
  Attribute attr;
  attr.name = name;
  attr.hasInput = hasInput;
  attr.hasOutput = hasOutput;
  mNodes[nodeIndex].attributes.append( attr );
  emit attributesChanged( nodeIndex );
}

QVariantList FlowNodeModel::getAttributes( int nodeIndex ) const
{
  QVariantList list;
  if ( nodeIndex < 0 || nodeIndex >= mNodes.size() )
    return list;
  for ( const Attribute &attr : mNodes[nodeIndex].attributes )
  {
    QVariantMap map;
    map["name"] = attr.name;
    map["hasInput"] = attr.hasInput;
    map["hasOutput"] = attr.hasOutput;
    list.append( map );
  }
  return list;
}
