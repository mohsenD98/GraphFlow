#pragma once
#include <QAbstractListModel>

struct LinkData
{
    int fromNodeIndex;
    int fromAttributeIndex;
    int toNodeIndex;
    int toAttributeIndex;
};

class FlowLinkModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    enum LinkRoles
    {
      FromNodeRole = Qt::UserRole + 1,
      FromAttributeRole,
      ToNodeRole,
      ToAttributeRole
    };

    explicit FlowLinkModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addLink( int fromNode, int fromAttr, int toNode, int toAttr );
    Q_INVOKABLE void removeLink( int index );
    Q_INVOKABLE void clear();

  private:
    QList<LinkData> mLinks;
};
