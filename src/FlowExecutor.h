#pragma once
#include "FlowNodeModel.h"
#include "LinkModel.h"

#include <QObject>

struct ExecutionContext
{
    QHash<QString, QVariant> values;
};

class IRuntimeNode
{
  public:
    virtual ~IRuntimeNode() {}
    virtual void execute( ExecutionContext &ctx ) = 0;
    QString id;
};

class ConstRuntimeNode : public IRuntimeNode
{
  public:
    QVariant value;
    ConstRuntimeNode( const QString &nid, QVariant v )
    {
      id = nid;
      value = v;
    }
    void execute( ExecutionContext &ctx ) override
    {
      ctx.values[id + ".output"] = value;
    }
};

class AddRuntimeNode : public IRuntimeNode
{
  public:
    QString inA, inB;
    AddRuntimeNode( const QString &nid, QString a, QString b )
    {
      id = nid;
      inA = a;
      inB = b;
    }
    void execute( ExecutionContext &ctx ) override
    {
      int A = ctx.values.value( inA ).toInt();
      int B = ctx.values.value( inB ).toInt();
      ctx.values[id + ".output"] = A + B;
    }
};

class PrintRuntimeNode : public IRuntimeNode
{
  public:
    QString in;
    PrintRuntimeNode( const QString &nid, QString input )
    {
      id = nid;
      in = input;
    }
    void execute( ExecutionContext &ctx ) override
    {
      qDebug() << "Print from" << id << "=>" << ctx.values[in];
    }
};

class FlowExecutor : public QObject
{
    Q_OBJECT
  public:
    FlowExecutor( FlowNodeModel *nodes, FlowLinkModel *links, QObject *parent = nullptr );

    void run();

  private:
    FlowNodeModel *mNodes;
    FlowLinkModel *mLinks;
};
