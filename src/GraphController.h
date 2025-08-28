#pragma once
#include "FlowNodeModel.h"
#include "LinkModel.h"

#include <QList>
#include <QObject>

class GraphController : public QObject
{
    Q_OBJECT
    Q_PROPERTY( FlowNodeModel *nodeModel READ nodeModel CONSTANT )
    Q_PROPERTY( LinkModel *linkModel READ linkModel CONSTANT )

  public:
    explicit GraphController( QObject *parent = nullptr );

    FlowNodeModel *nodeModel() const { return mFlowNodeModel; }
    LinkModel *linkModel() const { return mLinkModel; }

    Q_INVOKABLE void removeNode( int index );
    Q_INVOKABLE void removeNodes( const QList<int> &nodesToRemove );
    Q_INVOKABLE void clearGraph();

    Q_INVOKABLE bool saveFlow( const QString &path );
    Q_INVOKABLE bool loadFlow( const QString &path );

  private:
    FlowNodeModel *mFlowNodeModel;
    LinkModel *mLinkModel;
};
