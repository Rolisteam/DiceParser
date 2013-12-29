#ifndef EXECUTIONNODE_H
#define EXECUTIONNODE_H

#include "diceresult.h"

class ExecutionNode
{
public:
    ExecutionNode();
    virtual ~ExecutionNode();
    virtual void run(ExecutionNode* previous = NULL)=0;
    DiceResult* getResult();
    void setNextNode(ExecutionNode*);
    ExecutionNode* getNextNode();
protected:
    DiceResult m_result;
    ExecutionNode* m_nextNode;
};

#endif // EXECUTIONNODE_H
