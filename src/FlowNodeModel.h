#pragma once

#include <QAbstractListModel>

struct Attribute
{
    QString name;
    bool hasInput;
    bool hasOutput;
    QVariant value = 3;
};

struct FlowNodeData
{
    QString name;
    QString type;
    int x;
    int y;
    QString color;
    QList<Attribute> attributes;
};

class FlowNodeModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    enum NodeRoles
    {
      NameRole = Qt::UserRole + 1,
      TypeRole,
      XRole,
      YRole,
      ColorRole
    };

    explicit FlowNodeModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addNode( const QString &name, QString type, int x, int y, const QString &color );
    Q_INVOKABLE void removeNode( int index );
    Q_INVOKABLE void clear();
    Q_INVOKABLE FlowNodeData getNode( int index ) const;
    Q_INVOKABLE void setNodePosition( int index, int x, int y );

    Q_INVOKABLE void addAttribute( int nodeIndex, const QString &name, bool hasInput, bool hasOutput );
    Q_INVOKABLE QVariantList getAttributes( int nodeIndex ) const;
    Q_INVOKABLE void setAttributeValue( int nodeIndex, const QString &attrName, const QVariant &val );
    Q_INVOKABLE QVariant getAttributeValue( int nodeIndex, const QString &attrName ) const;

  signals:
    void attributesChanged( int nodeIndex );

  private:
    QList<FlowNodeData> mNodes;
};
