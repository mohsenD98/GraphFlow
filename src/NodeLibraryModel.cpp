#include "NodeLibraryModel.h"

#include <QDebug>

NodeLibraryModel::NodeLibraryModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

int NodeLibraryModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  return m_nodes.count();
}

QVariant NodeLibraryModel::data( const QModelIndex &index, int role ) const
{
  if ( !index.isValid() || index.row() >= m_nodes.size() )
    return QVariant();

  const NodeItem &item = m_nodes.at( index.row() );

  switch ( role )
  {
    case TypeRole:
      return item.type;
    case NameRole:
      return item.name;
    case DescriptionRole:
      return item.description;
    case IconRole:
      return item.icon;
    case ColorRole:
      return item.color;
    case AttributesRole:
      return item.attributes;
    case ValuesRole:
      return item.values;
    default:
      return QVariant();
  }
}

QHash<int, QByteArray> NodeLibraryModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[TypeRole] = "type";
  roles[NameRole] = "name";
  roles[DescriptionRole] = "description";
  roles[IconRole] = "icon";
  roles[ColorRole] = "color";
  roles[AttributesRole] = "attributes";
  roles[ValuesRole] = "values";
  return roles;
}

bool NodeLibraryModel::loadFromJson( const QString &path )
{
  QFile file( path );
  if ( !file.open( QIODevice::ReadOnly ) )
  {
    qWarning() << "Cannot open JSON file:" << path;
    return false;
  }

  QByteArray data = file.readAll();
  QJsonDocument doc = QJsonDocument::fromJson( data );
  if ( !doc.isArray() )
    return false;

  QJsonArray arr = doc.array();
  beginResetModel();
  m_nodes.clear();
  for ( const QJsonValue &v : arr )
  {
    if ( !v.isObject() )
      continue;
    QJsonObject obj = v.toObject();
    NodeItem item;
    item.type = obj["type"].toString();
    item.name = obj["name"].toString();
    item.description = obj["description"].toString();
    item.icon = obj["icon"].toString();
    item.color = obj["color"].toString();

    // attributes
    item.attributes.clear();
    if ( obj.contains( "attributes" ) && obj["attributes"].isArray() )
    {
      QJsonArray attrArr = obj["attributes"].toArray();
      for ( const QJsonValue &attrVal : attrArr )
      {
        if ( attrVal.isObject() )
        {
          item.attributes.append( attrVal.toObject().toVariantMap() );
        }
      }
    }

    // values
    if ( obj.contains( "values" ) && obj["values"].isObject() )
    {
      item.values = obj["values"].toObject().toVariantMap();
    }
    else
    {
      item.values.clear();
    }

    m_nodes.append( item );
  }
  endResetModel();
  return true;
}


QVariantMap NodeLibraryModel::nodeByName( const QString &name ) const
{
  for ( const NodeItem &item : m_nodes )
  {
    if ( item.name == name )
    {
      QVariantMap map;
      map["type"] = item.type;
      map["name"] = item.name;
      map["description"] = item.description;
      map["icon"] = item.icon;
      map["color"] = item.color;
      map["attributes"] = item.attributes;
      map["values"] = QVariant::fromValue( item.values );
      return map;
    }
  }
  return {};
}

void NodeLibraryProxyModel::setFilterText( const QString &text )
{
  if ( m_filterText != text )
  {
    m_filterText = text;
    invalidateFilter();
    emit filterTextChanged();
  }
}

bool NodeLibraryProxyModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const
{
  if ( m_filterText.isEmpty() )
    return true;

  QModelIndex index = sourceModel()->index( source_row, 0, source_parent );
  QString name = index.data( Qt::DisplayRole ).toString();
  QString description = index.data( NodeLibraryModel::DescriptionRole ).toString();

  return name.contains( m_filterText, Qt::CaseInsensitive ) || description.contains( m_filterText, Qt::CaseInsensitive );
}
