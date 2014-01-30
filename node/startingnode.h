#ifndef STARTINGNODE_H
#define STARTINGNODE_H

#include "executionnode.h"

class StartingNode : public ExecutionNode
{
public:
    StartingNode();

    virtual void run(ExecutionNode*);
    virtual QString toString()const;
    virtual qint64 getPriority() const;
};

#endif // STARTINGNODE_H
