#pragma once
#include "FlowNodeModel.h"
#include "LinkModel.h"

#include <QList>
#include <QObject>

class GraphController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( FlowNodeModel *nodeModel READ nodeModel CONSTANT )
    Q_PROPERTY( FlowLinkModel *linkModel READ linkModel CONSTANT )
    Q_PROPERTY( QList<QString> selectedNodes READ selectedNodes NOTIFY selectedNodesChanged )

  public:
    explicit GraphController( QObject *parent = nullptr );

    FlowNodeModel *nodeModel() const { return mFlowNodeModel; }
    FlowLinkModel *linkModel() const { return mFlowLinkModel; }

    Q_INVOKABLE void removeNodes( const QList<QString> &nodesToRemove );
    Q_INVOKABLE void removeLink( const QString &fromNode, int fromAttr, const QString &toNode, int toAttr );
    Q_INVOKABLE void clearGraph();
    Q_INVOKABLE void runFlow();

    Q_INVOKABLE bool saveFlow( const QString &path );
    Q_INVOKABLE bool loadFlow( const QString &path );

    // Selected nodes
    Q_INVOKABLE void selectNode( const QString &id );
    Q_INVOKABLE void deselectNode( const QString &id );
    Q_INVOKABLE void clearSelection();
    QList<QString> selectedNodes() const;

  signals:
    void selectedNodesChanged();

  private:
    FlowNodeModel *mFlowNodeModel;
    FlowLinkModel *mFlowLinkModel;
    QList<QString> mSelectedNodes;
};
