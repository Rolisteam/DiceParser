#ifndef PARENTHESESNODE_H
#define PARENTHESESNODE_H

#include "executionnode.h"

class ParenthesesNode : public ExecutionNode
{
public:
    ParenthesesNode();
    virtual void run(ExecutionNode* previous = NULL)=0;

    void setInternelNode(ExecutionNode* node);
private:
    ExecutionNode* m_internalNode;
};

#endif // PARENTHESESNODE_H
