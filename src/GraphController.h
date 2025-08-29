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

  public:
    explicit GraphController( QObject *parent = nullptr );

    FlowNodeModel *nodeModel() const { return mFlowNodeModel; }
    FlowLinkModel *linkModel() const { return mFlowLinkModel; }

    Q_INVOKABLE void removeNode( int index );
    Q_INVOKABLE void removeNodes( const QList<int> &nodesToRemove );
    Q_INVOKABLE void clearGraph();
    Q_INVOKABLE void runFlow();

    Q_INVOKABLE bool saveFlow( const QString &path );
    Q_INVOKABLE bool loadFlow( const QString &path );

  private:
    FlowNodeModel *mFlowNodeModel;
    FlowLinkModel *mFlowLinkModel;
};
