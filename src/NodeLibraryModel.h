#pragma once
#include <QAbstractListModel>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>

struct NodeItem
{
    QString type;
    QString name;
    QString description;
    QString icon;
    QString color;
    QVariantList attributes;
    QVariantMap values;
};

class NodeLibraryModel : public QAbstractListModel
{
    Q_OBJECT
  public:
    enum NodeRoles
    {
      TypeRole = Qt::UserRole + 1,
      NameRole,
      DescriptionRole,
      IconRole,
      ColorRole,
      AttributesRole,
      ValuesRole
    };


    explicit NodeLibraryModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE bool loadFromJson( const QString &path );
    Q_INVOKABLE QVariantMap nodeByName( const QString &name ) const;

  private:
    QList<NodeItem> m_nodes;
};


#include <QSortFilterProxyModel>

class NodeLibraryProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY( QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged )

  public:
    explicit NodeLibraryProxyModel( QObject *parent = nullptr )
      : QSortFilterProxyModel( parent ) {}

    QString filterText() const { return m_filterText; }

    void setFilterText( const QString &text );

  signals:
    void filterTextChanged();

  protected:
    bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

  private:
    QString m_filterText;
};
