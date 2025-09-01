#include "LinkModel.h"

FlowLinkModel::FlowLinkModel( QObject *parent )
  : QAbstractListModel( parent ) {}

int FlowLinkModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  return mLinks.size();
}

QVariant FlowLinkModel::data( const QModelIndex &index, int role ) const
{
  if ( !index.isValid() || index.row() >= mLinks.size() )
    return QVariant();

  const LinkData &link = mLinks[index.row()];

  switch ( role )
  {
    case FromNodeRole:
      return link.fromNodeId;
    case FromAttributeRole:
      return link.fromAttributeIndex;
    case ToNodeRole:
      return link.toNodeId;
    case ToAttributeRole:
      return link.toAttributeIndex;
    default:
      return QVariant();
  }
}

QHash<int, QByteArray> FlowLinkModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[FromNodeRole] = "fromNode";
  roles[FromAttributeRole] = "fromAttr";
  roles[ToNodeRole] = "toNode";
  roles[ToAttributeRole] = "toAttr";
  return roles;
}

void FlowLinkModel::addLink( const QString &fromNodeId, int fromAttr, const QString &toNodeId, int toAttr )
{
  beginInsertRows( QModelIndex(), mLinks.size(), mLinks.size() );
  mLinks.append( { fromNodeId, fromAttr, toNodeId, toAttr } );
  endInsertRows();
}

void FlowLinkModel::removeLink( int index )
{
  if ( index < 0 || index >= mLinks.size() )
    return;
  beginRemoveRows( QModelIndex(), index, index );
  mLinks.removeAt( index );
  endRemoveRows();
}

void FlowLinkModel::clear()
{
  beginResetModel();
  mLinks.clear();
  endResetModel();
}
