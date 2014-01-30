#ifndef EXECUTIONNODE_H
#define EXECUTIONNODE_H

#include "result.h"
#include <QDebug>
class ExecutionNode
{
public:
    ExecutionNode();
    virtual ~ExecutionNode();
    virtual void run(ExecutionNode* previous = NULL)=0;
    Result* getResult();
    void setNextNode(ExecutionNode*);
    ExecutionNode* getNextNode();
    virtual QString toString()const=0;
    virtual qint64 getPriority() const=0;
protected:
    Result* m_result;
    ExecutionNode* m_nextNode;
};

#endif // EXECUTIONNODE_H
