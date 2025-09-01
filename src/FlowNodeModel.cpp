#include "FlowNodeModel.h"

#include <QUuid>

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
    case IdRole:
      return node.id;
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
  roles[IdRole] = "id";
  return roles;
}

QString FlowNodeModel::addNode( const QString &name, QString type, int x, int y, const QString &color, const QString &uuid )
{
  beginInsertRows( QModelIndex(), mNodes.size(), mNodes.size() );
  FlowNodeData node;
  if ( uuid.isEmpty() )
  {
    node.id = QUuid::createUuid().toString( QUuid::WithoutBraces );
  }
  else
  {
    node.id = uuid;
  }
  node.name = name;
  node.type = type;
  node.x = x;
  node.y = y;
  node.color = color;
  mNodes.append( node );
  endInsertRows();

  return node.id;
}

void FlowNodeModel::removeNode( const QString &id )
{
  int index = -1;

  for ( int i = 0; i < mNodes.size(); ++i )
  {
    if ( mNodes[i].id == id )
    {
      index = i;
      break;
    }
  }

  if ( index < 0 )
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

FlowNodeData FlowNodeModel::getNode( const QString &id ) const
{
  for ( const auto &node : mNodes )
  {
    if ( node.id == id )
      return node;
  }
  return FlowNodeData();
}

FlowNodeData FlowNodeModel::getNode( int index ) const
{
  if ( index < 0 || index >= mNodes.size() )
    return FlowNodeData();
  return mNodes[index];
}

void FlowNodeModel::setNodePosition( const QString &id, int x, int y )
{
  for ( int i = 0; i < mNodes.size(); ++i )
  {
    auto &node = mNodes[i];
    if ( node.id == id )
    {
      if ( node.x == x && node.y == y )
        return;

      node.x = x;
      node.y = y;

      QModelIndex modelIndex = this->index( i );
      emit dataChanged( modelIndex, modelIndex, { XRole, YRole } );
      return;
    }
  }
}

void FlowNodeModel::addAttribute( const QString &id, const QString &name, bool hasInput, bool hasOutput )
{
  for ( int i = 0; i < mNodes.size(); ++i )
  {
    if ( mNodes[i].id == id )
    {
      Attribute attr;
      attr.name = name;
      attr.hasInput = hasInput;
      attr.hasOutput = hasOutput;
      mNodes[i].attributes.append( attr );
      emit attributesChanged( id );
      return;
    }
  }
}

QVariantList FlowNodeModel::getAttributes( const QString &id ) const
{
  QVariantList list;
  for ( const auto &node : mNodes )
  {
    if ( node.id == id )
    {
      for ( const Attribute &attr : node.attributes )
      {
        QVariantMap map;
        map["name"] = attr.name;
        map["hasInput"] = attr.hasInput;
        map["hasOutput"] = attr.hasOutput;
        list.append( map );
      }
      break;
    }
  }
  return list;
}

void FlowNodeModel::setAttributeValue( const QString &id, const QString &attrName, const QVariant &val )
{
  for ( int i = 0; i < mNodes.size(); ++i )
  {
    if ( mNodes[i].id == id )
    {
      for ( auto &attr : mNodes[i].attributes )
      {
        if ( attr.name == attrName )
        {
          attr.value = val;
          emit attributesChanged( id );
          return;
        }
      }
    }
  }
}

QVariant FlowNodeModel::getAttributeValue( const QString &id, const QString &attrName ) const
{
  for ( const auto &node : mNodes )
  {
    if ( node.id == id )
    {
      for ( const auto &attr : node.attributes )
      {
        if ( attr.name == attrName )
        {
          return attr.value;
        }
      }
    }
  }
  return QVariant();
}
