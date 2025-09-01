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
    QString id;
    QString name;
    QString type;
    bool selected = false;
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
      ColorRole,
      IdRole,
      SelectedRole
    };

    explicit FlowNodeModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QString addNode( const QString &name, QString type, int x, int y, const QString &color, const QString &uuid = "" );
    Q_INVOKABLE void removeNode( const QString &id );
    Q_INVOKABLE void clear();
    Q_INVOKABLE FlowNodeData getNode( const QString &id ) const;
    Q_INVOKABLE FlowNodeData getNode( int id ) const;
    Q_INVOKABLE void setNodePosition( const QString &id, int x, int y );
    Q_INVOKABLE void addAttribute( const QString &id, const QString &name, bool hasInput, bool hasOutput );
    Q_INVOKABLE QVariantList getAttributes( const QString &id ) const;
    Q_INVOKABLE void setAttributeValue( const QString &id, const QString &attrName, const QVariant &val );
    Q_INVOKABLE QVariant getAttributeValue( const QString &id, const QString &attrName ) const;
    void setNodeSelected( const QString &id, bool selected );

  signals:
    void attributesChanged( const QString &id );

  private:
    QList<FlowNodeData> mNodes;
};
