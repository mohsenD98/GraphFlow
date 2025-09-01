#pragma once
#include <QAbstractListModel>

struct LinkData
{
    QString fromNodeId;
    int fromAttributeIndex;
    QString toNodeId;
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

    Q_INVOKABLE void addLink( const QString &fromNodeId, int fromAttr, const QString &toNodeId, int toAttr );
    Q_INVOKABLE void removeLink( int index );
    Q_INVOKABLE void clear();

  private:
    QList<LinkData> mLinks;
};
