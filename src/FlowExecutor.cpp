#include "FlowExecutor.h"

#include <QDebug>
#include <QQueue>

FlowExecutor::FlowExecutor( FlowNodeModel *nodes, FlowLinkModel *links, QObject *parent )
  : QObject( parent ), mNodes( nodes ), mLinks( links ) {}

void FlowExecutor::run()
{
  ExecutionContext ctx;
  QList<IRuntimeNode *> runtimeNodes;

  int N = mNodes->rowCount();
  if ( N == 0 )
    return;

  // ساخت runtime node ها
  for ( int i = 0; i < N; ++i )
  {
    FlowNodeData node = mNodes->getNode( i );
    QString nodeId = QString( "n%1-%2" ).arg( i ).arg( node.type );

    if ( node.type == "Input" )
    {
      // مقدار رو از attribute بخون
      QVariant val;
      for ( const auto &attr : node.attributes )
      {
        if ( attr.hasOutput )
        {
          val = attr.value;
          break;
        }
      }
      runtimeNodes << new ConstRuntimeNode( nodeId, val.isValid() ? val.toInt() : 0 );
    }
    else if ( node.type == "Add" )
    {
      runtimeNodes << new AddRuntimeNode( nodeId, "", "" );
    }
    else if ( node.type == "Output" )
    {
      runtimeNodes << new PrintRuntimeNode( nodeId, "" );
    }
    else
    {
      qWarning() << "Unknown node type:" << node.type;
      runtimeNodes << nullptr;
    }
  }

  // ساخت گراف adjacency
  QVector<QVector<int>> adj( N );
  QVector<int> indeg( N, 0 );

  for ( int i = 0; i < mLinks->rowCount(); ++i )
  {
    QModelIndex idx = mLinks->index( i, 0 );

    int fromNode = mLinks->data( idx, FlowLinkModel::FromNodeRole ).toInt();
    int fromAttr = mLinks->data( idx, FlowLinkModel::FromAttributeRole ).toInt();
    int toNode = mLinks->data( idx, FlowLinkModel::ToNodeRole ).toInt();
    int toAttr = mLinks->data( idx, FlowLinkModel::ToAttributeRole ).toInt();

    adj[fromNode].append( toNode );
    indeg[toNode]++;

    // کلید خروجی از fromNode
    QString fromKey = QString( "n%1-%2.%3" )
                        .arg( fromNode )
                        .arg( mNodes->getNode( fromNode ).type )
                        .arg( mNodes->getNode( fromNode ).attributes[fromAttr].name );

    qDebug() << "Link:" << fromNode << fromAttr << "->" << toNode << toAttr << "  key=" << fromKey;

    // وصل کردن ورودی‌ها
    if ( auto *add = dynamic_cast<AddRuntimeNode *>( runtimeNodes[toNode] ) )
    {
      if ( toAttr == 0 )
        add->inA = fromKey;
      else if ( toAttr == 1 )
        add->inB = fromKey;
    }
    else if ( auto *pr = dynamic_cast<PrintRuntimeNode *>( runtimeNodes[toNode] ) )
    {
      pr->in = fromKey;
    }
  }

  // topo sort
  QQueue<int> q;
  for ( int i = 0; i < N; ++i )
    if ( indeg[i] == 0 )
      q.enqueue( i );

  QList<int> topoOrder;
  while ( !q.isEmpty() )
  {
    int u = q.dequeue();
    topoOrder << u;
    for ( int v : adj[u] )
    {
      indeg[v]--;
      if ( indeg[v] == 0 )
        q.enqueue( v );
    }
  }

  if ( topoOrder.size() != N )
  {
    qWarning() << "Cycle detected in flow graph!";
    qDeleteAll( runtimeNodes );
    return;
  }

  // اجرای نودها
  for ( int idx : topoOrder )
  {
    if ( runtimeNodes[idx] )
      runtimeNodes[idx]->execute( ctx );
  }

  // debug context
  qDebug() << "=== Execution Context ===";
  for ( auto it = ctx.values.begin(); it != ctx.values.end(); ++it )
  {
    qDebug() << it.key() << ":" << it.value();
  }

  qDeleteAll( runtimeNodes );
}
