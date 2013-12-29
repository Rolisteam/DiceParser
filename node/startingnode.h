#ifndef STARTINGNODE_H
#define STARTINGNODE_H

#include "executionnode.h"

class StartingNode : public ExecutionNode
{
public:
    StartingNode();

    virtual void run(ExecutionNode*);
};

#endif // STARTINGNODE_H
