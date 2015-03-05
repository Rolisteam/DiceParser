#ifndef NUMBERNODE_H
#define NUMBERNODE_H

#include "node/executionnode.h"
#include "result/scalarresult.h"

class NumberNode : public ExecutionNode
{
public:
    NumberNode();
    void run(ExecutionNode* previous);
    void setNumber(qint64);
virtual QString toString()const;
    virtual qint64 getPriority() const;
private:
    qint64 m_number;
    ScalarResult* m_scalarResult;
};

#endif // NUMBERNODE_H
