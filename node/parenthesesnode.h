#ifndef PARENTHESESNODE_H
#define PARENTHESESNODE_H

#include "executionnode.h"

class ParenthesesNode : public ExecutionNode
{
public:
    ParenthesesNode();
    virtual void run(ExecutionNode* previous = NULL);

    void setInternelNode(ExecutionNode* node);
    virtual QString toString()const;
    virtual qint64 getPriority() const;
private:
    ExecutionNode* m_internalNode;
};

#endif // PARENTHESESNODE_H
